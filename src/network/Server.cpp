#include "src/network/Server.h"

#include "src/exceptions/InternalErrorException.h"
#include "src/exceptions/InvalidPortException.h"
#include "src/utility/Logging.h"

#include <cstdlib>
#include <iostream>

#include <QCoreApplication>
#include <QDateTime>
#include <QNetworkDatagram>

#include "src/network/PingMessage.h"
#include "src/network/WelcomeMessage.h"
#include "src/network/CloseMessage.h"

namespace packetlossutils {
	namespace network {

		Server::Server(quint16 port, QObject* parent) : NetworkCore(parent), m_nextPingTime(0) {
			if (!m_udpSocket->bind(QHostAddress::Any, port)) {
				LOGGER()->error("Failed to bind to port {}!", port);
				throw packetlossutils::exceptions::InvalidPortException() << "Failed to bind to port " << port << "!";
			}
		}

		Server::~Server() {
			// Sent info to connected clients
			QMapIterator<quint64, std::shared_ptr<ClientInfo>> i(m_clientInfos);
			while (i.hasNext()) {
				i.next();
				std::shared_ptr<ClientInfo> const clientInfo = i.value();
				CloseMessage closeMessage(i.key(), CloseMessage::REASON_QUIT);
				m_udpSocket->writeDatagram(closeMessage.getData(), clientInfo->address, clientInfo->port);
			}
		}

		Server::ClientInfo::ClientInfo(QHostAddress address, quint16 port, quint32 interval) : address(address), port(port), messageCounter(0), messageCounterOtherNextExpected(0), interval(interval), intervalCounter(0), timeOfLastMessageFromOther(-1), timeOfLastRealMessageFromOther(QDateTime::currentMSecsSinceEpoch()), pingStats(PingStats::initialize()) {
			//
		}

		void Server::onPingTimer() {
			qint64 const currentMSecsSinceEpoch = QDateTime::currentMSecsSinceEpoch();
			bool const skipMessage = currentMSecsSinceEpoch > (m_nextPingTime + MAX_TIME_LATE_MS);
			qint64 const numPingsMissed = (std::abs(currentMSecsSinceEpoch - m_nextPingTime) / BASE_TIMER_INTERVAL) + 1;

			for (qint64 i = 0; i < numPingsMissed; ++i) {
				QMapIterator<quint64, std::shared_ptr<ClientInfo>> it(m_clientInfos);
				while (it.hasNext()) {
					it.next();
					std::shared_ptr<ClientInfo> const clientInfo = it.value();
					++clientInfo->intervalCounter;
					if ((clientInfo->intervalCounter * BASE_TIMER_INTERVAL) >= clientInfo->interval) {
						PingMessage pingMessage(it.key(), clientInfo->messageCounter, clientInfo->pingStats);
						++clientInfo->messageCounter;
						clientInfo->intervalCounter = 0;
						if (!skipMessage) {
							m_udpSocket->writeDatagram(pingMessage.getData(), clientInfo->address, clientInfo->port);
							clientInfo->pingStats.addPacketSent();
							LOGGER_DEBUG("Sent ping #{} to client #{}.", clientInfo->messageCounter - 1, it.key());
						} else {
							LOGGER()->warn("PingTimer was delayed, skipping ping #{} to client #{}.", clientInfo->messageCounter - 1, it.key());
						}
					}
				}

				m_nextPingTime = currentMSecsSinceEpoch + BASE_TIMER_INTERVAL;
			}
		}

		void Server::onTimeoutTimer() {
			qint64 const currentMSecsSinceEpoch = QDateTime::currentMSecsSinceEpoch();
			QMapIterator<quint64, std::shared_ptr<ClientInfo>> i(m_clientInfos);
			while (i.hasNext()) {
				i.next();
				std::shared_ptr<ClientInfo> const clientInfo = i.value();
				if ((clientInfo->timeOfLastMessageFromOther != -1) && (currentMSecsSinceEpoch > (clientInfo->timeOfLastMessageFromOther + clientInfo->interval + MAX_TIME_LATE_MS))) {
					LOGGER_DEBUG("Timeout for message #{}, marking as lost.", clientInfo->messageCounterOtherNextExpected);
					clientInfo->pingStats.addResult(false);
					++clientInfo->messageCounterOtherNextExpected;
					clientInfo->timeOfLastMessageFromOther += clientInfo->interval;
				}
				if ((currentMSecsSinceEpoch - clientInfo->timeOfLastRealMessageFromOther) >= CLIENT_TIMEOUT_MS) {
					LOGGER()->warn("Did not hear from client {} in {}ms, assuming dead.", i.key(), (currentMSecsSinceEpoch - clientInfo->timeOfLastRealMessageFromOther));

					CloseMessage closeMessage(i.key(), CloseMessage::REASON_TIMEOUT);
					m_udpSocket->writeDatagram(closeMessage.getData(), clientInfo->address, clientInfo->port);

					m_clientInfos.remove(i.key());
					
					// Since the Iterator may be invalid, restart.
					onTimeoutTimer();

					startOrStopTimersIfNeeded();
					return;
				}
			}
		}

		void Server::startOrStopTimersIfNeeded() {
			if (m_clientInfos.size() > 0) {
				if (!m_timerPing.isActive()) {
					// Set up ping timer
					m_timerPing.setInterval(BASE_TIMER_INTERVAL);
					m_timerPing.start();
					m_nextPingTime = QDateTime::currentMSecsSinceEpoch() + BASE_TIMER_INTERVAL;

					// Set up timeout timer
					m_timerTimeout.setInterval(100);
					m_timerTimeout.start();
				}
			} else {
				m_timerPing.stop();
				m_timerTimeout.stop();
			}
		}

		void Server::onSigIntReceived() {
			LOGGER()->info("Sending disconnect info to clients.");
			QMapIterator<quint64, std::shared_ptr<ClientInfo>> i(m_clientInfos);
			while (i.hasNext()) {
				i.next();
				std::shared_ptr<ClientInfo> const clientInfo = i.value();
				CloseMessage closeMessage(i.key(), CloseMessage::REASON_QUIT);
				m_udpSocket->writeDatagram(closeMessage.getData(), clientInfo->address, clientInfo->port);
			}
			m_clientInfos.clear();
			startOrStopTimersIfNeeded();

			QCoreApplication::exit(0);
		}

		void Server::receivedMessage(QHostAddress const& address, quint16 port, std::shared_ptr<Message> const& message) {
			Message::MessageType const messageType = message->getMessageType();
			LOGGER_DEBUG("Server received datagram of type {} from {}:{}.", static_cast<int>(messageType), address.toString().toStdString(), port);

			if (messageType == Message::MessageType::MSGTYPE_PING) {
				std::shared_ptr<PingMessage> const pingMessage = std::dynamic_pointer_cast<PingMessage>(message);

				quint64 const uniqueId = pingMessage->getUniqueId();
				if (m_clientInfos.contains(uniqueId)) {
					std::shared_ptr<ClientInfo> const clientInfo = m_clientInfos.value(uniqueId);
					if (clientInfo->messageCounterOtherNextExpected > pingMessage->getCounter()) {
						LOGGER()->warn("We already marked message #{} as lost, but we did receive it now. Dropping. We are expecting message #{}.", pingMessage->getCounter(), clientInfo->messageCounterOtherNextExpected);
						clientInfo->timeOfLastMessageFromOther = QDateTime::currentMSecsSinceEpoch();
						clientInfo->timeOfLastRealMessageFromOther = QDateTime::currentMSecsSinceEpoch();
						return;
					}

					// We might have lost messages, mark them as not seen.
					quint64 lostMessagesCount = 0;
					while (clientInfo->messageCounterOtherNextExpected < pingMessage->getCounter()) {
						clientInfo->pingStats.addResult(false);
						++clientInfo->messageCounterOtherNextExpected;
						++lostMessagesCount;
					}
					if (lostMessagesCount > 0) {
						LOGGER_DEBUG("Skipped {} lost messages.", lostMessagesCount);
					}

					// Mark the current message as seen
					clientInfo->pingStats.addResult(true);
					clientInfo->pingStats.addPacketReceived();
					++clientInfo->messageCounterOtherNextExpected;

					clientInfo->timeOfLastMessageFromOther = QDateTime::currentMSecsSinceEpoch();
					clientInfo->timeOfLastRealMessageFromOther = QDateTime::currentMSecsSinceEpoch();
				} else {
					LOGGER()->error("Received a ping from Client {}, which we do not know.", uniqueId);
					CloseMessage closeMessage(uniqueId, CloseMessage::REASON_UNSOLICITED);
					m_udpSocket->writeDatagram(closeMessage.getData(), address, port);
				}
			} else if (messageType == Message::MessageType::MSGTYPE_WELCOME) {
				std::shared_ptr<WelcomeMessage> const welcomeMessage = std::dynamic_pointer_cast<WelcomeMessage>(message);
				quint64 const uniqueId = welcomeMessage->getUniqueId();
				if (m_clientInfos.contains(uniqueId)) {
					LOGGER()->error("Received a ping from Client {}, which we already know.", uniqueId);
					return;
				}

				if ((welcomeMessage->getPingIntervalInMs() % BASE_TIMER_INTERVAL) != 0) {
					LOGGER()->error("Client requested interval {}, but we only support multiples of {}. Ignoring.", welcomeMessage->getPingIntervalInMs(), BASE_TIMER_INTERVAL);
					return;
				}

				m_clientInfos.insert(uniqueId, std::make_shared<ClientInfo>(address, port, welcomeMessage->getPingIntervalInMs()));
				LOGGER()->info("Received welcome from new client {}.", uniqueId);

				// Send answer - currently just a copy of the welcome
				m_udpSocket->writeDatagram(welcomeMessage->getData(), address, port);

				startOrStopTimersIfNeeded();
			} else if (messageType == Message::MessageType::MSGTYPE_QUIT) {
				std::shared_ptr<CloseMessage> const closeMessage = std::dynamic_pointer_cast<CloseMessage>(message);
				quint64 const uniqueId = closeMessage->getUniqueId();
				if (!m_clientInfos.contains(uniqueId)) {
					LOGGER()->error("Received a close from Client {}, which we do not know.", uniqueId);
					return;
				}

				LOGGER()->info("Received close from client {}.", uniqueId);
				m_clientInfos.remove(uniqueId);
			} else {
				LOGGER()->error("Received unhandled message type: {}", static_cast<int>(messageType));
			}
		}
	}
}
