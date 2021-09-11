#include "src/network/Client.h"

#include "src/exceptions/InternalErrorException.h"
#include "src/exceptions/InvalidPortException.h"
#include "src/utility/Logging.h"

#include "src/network/WelcomeMessage.h"
#include "src/network/PingMessage.h"
#include "src/network/CloseMessage.h"

#include <QDateTime>
#include <QRandomGenerator>
#include <QThread>

#include <cstdlib>
#include <iostream>


namespace packagelossutils {
	namespace network {

		Client::Client(QHostAddress const& address, quint16 port, quint32 interval, QObject* parent) : NetworkCore(parent), m_serverHostAddress(address), m_serverPort(port), 
			m_uniqueId(QRandomGenerator::global()->generate64()), m_interval(interval), m_isConnected(false), 
			m_timerStats(this),
			m_messageCounter(0), m_messageCounterOtherNextExpected(0), m_timeOfLastMessageFromOther(-1),
			m_otherStats(PingStats::initialize()), m_clientStats(PingStats::initialize()), m_nextPingTime(0)
		{
			//
			if (!QObject::connect(&m_timerStats, SIGNAL(timeout()), this, SLOT(onStatsTimer()))) {
				throw packagelossutils::exceptions::InternalErrorException() << "Failed to bind onStatsTimer() on Timer!";
			}

			LOGGER_DEBUG("Client configured for {}:{} with unique ID {} and interval {}.", m_serverHostAddress.toString().toStdString(), m_serverPort, m_uniqueId, m_interval);
		}

		Client::~Client() {
			m_timerStats.stop();
			if (m_isConnected) {
				CloseMessage closeMessage(m_uniqueId, CloseMessage::REASON_QUIT);
				m_udpSocket->writeDatagram(closeMessage.getData(), m_serverHostAddress, m_serverPort);
			}
		}

		bool Client::connect() {
			if (m_isConnected) {
				LOGGER()->warn("Was already connected, connect() was called again!");
				return false;
			}

			WelcomeMessage welcomeMessage(m_uniqueId, m_interval);
			m_udpSocket->writeDatagram(welcomeMessage.getData(), m_serverHostAddress, m_serverPort);

			bool receivedReply = false;
			for (int i = 0; i < 10; ++i) {
				QThread::msleep(100);
				if (m_udpSocket->hasPendingDatagrams() && (m_udpSocket->pendingDatagramSize() > 0)) {
					receivedReply = true;
					break;
				}
			}

			if (!receivedReply) {
				LOGGER()->error("Failed to connect to server {}:{}.", m_serverHostAddress.toString().toStdString(), m_serverPort);
				return false;
			}
			m_timeOfLastMessageFromOther = QDateTime::currentMSecsSinceEpoch();
			LOGGER()->info("Received reply from server...");

			// Set up ping timer
			m_timerPing.setInterval(m_interval);
			m_timerPing.start();
			m_nextPingTime = QDateTime::currentMSecsSinceEpoch() + m_interval;

			// Set up timeout timer
			m_timerTimeout.setInterval(100);
			m_timerTimeout.start();

			// Set up stats timer
			m_timerStats.setInterval(2000);
			m_timerStats.start();

			return true;
		}

		void Client::onPingTimer() {
			qint64 const currentMSecsSinceEpoch = QDateTime::currentMSecsSinceEpoch();
			bool const skipMessage = currentMSecsSinceEpoch > (m_nextPingTime + MAX_TIME_LATE_MS);
			if (QDateTime::currentMSecsSinceEpoch() > (m_nextPingTime + MAX_TIME_LATE_MS)) {
				++m_messageCounter;
				m_nextPingTime += m_interval;

				LOGGER()->warn("PingTimer was delayed, skipping!");
				return;
			}

			PingMessage pingMessage(m_uniqueId, m_messageCounter, m_otherStats);
			++m_messageCounter;

			if (!skipMessage) {
				m_udpSocket->writeDatagram(pingMessage.getData(), m_serverHostAddress, m_serverPort);
				LOGGER_DEBUG("Sent ping #{} to server.", m_messageCounter - 1);
			} else {
				LOGGER()->warn("PingTimer was delayed, skipping ping #{} to server.", m_messageCounter - 1);
			}

			m_nextPingTime += currentMSecsSinceEpoch + m_interval;
		}
		
		void Client::onTimeoutTimer() {
			qint64 const currentMSecsSinceEpoch = QDateTime::currentMSecsSinceEpoch();
			if (currentMSecsSinceEpoch > (m_timeOfLastMessageFromOther + m_interval + MAX_TIME_LATE_MS)) {
				LOGGER_DEBUG("Timeout for incoming message #{}, marking as lost.", m_messageCounterOtherNextExpected);
				m_clientStats.addResult(false);
				++m_messageCounterOtherNextExpected;
				m_timeOfLastMessageFromOther += m_interval;
			}
		}

		void Client::onStatsTimer() {
			LOGGER()->info("------------------------");
			LOGGER()->info("Transmission rate INCOMING (10s/1min/5min): {}% / {}% / {}%)", m_clientStats.getPercentReceivedOverTime((10ul * 1000ul) / m_interval), m_clientStats.getPercentReceivedOverTime((60ul * 1000ul) / m_interval), m_clientStats.getPercentReceivedOverTime((5ul * 60ul * 1000ul) / m_interval));
			LOGGER()->info("Transmission rate OUTGOING (10s/1min/5min): {}% / {}% / {}%)", m_otherStats.getPercentReceivedOverTime((10ul * 1000ul) / m_interval), m_otherStats.getPercentReceivedOverTime((60ul * 1000ul) / m_interval), m_otherStats.getPercentReceivedOverTime((5ul * 60ul * 1000ul) / m_interval));
			//std::cout << m_otherStats.toString().toStdString() << std::endl;
		}

		void Client::receivedMessage(QHostAddress const& address, quint16 port, std::shared_ptr<Message> const& message) {
			Message::MessageType const messageType = message->getMessageType();
			LOGGER_DEBUG("Client received datagram of type {} from {}:{}.", static_cast<int>(messageType), address.toString().toStdString(), port);

			if ((address != m_serverHostAddress) || (port != m_serverPort)) {
				LOGGER()->warn("Received a message from {}:{}, but we are expecting them from {}:{}. Ignoring!", address.toString().toStdString(), port, m_serverHostAddress.toString().toStdString(), m_serverPort);
				return;
			}

			if (messageType == Message::MessageType::MSGTYPE_PING) {
				std::shared_ptr<PingMessage> const pingMessage = std::dynamic_pointer_cast<PingMessage>(message);

				if (m_messageCounterOtherNextExpected > pingMessage->getCounter()) {
					LOGGER()->warn("We already marked message #{} as lost, but we did receive it now. Dropping. We are expecting message #{}", pingMessage->getCounter(), m_messageCounterOtherNextExpected);
					m_timeOfLastMessageFromOther = QDateTime::currentMSecsSinceEpoch();
					return;
				}

				// We might have lost messages, mark them as not seen.
				quint64 lostMessagesCount = 0;
				while (m_messageCounterOtherNextExpected < pingMessage->getCounter()) {
					m_clientStats.addResult(false);
					++m_messageCounterOtherNextExpected;
					++lostMessagesCount;
				}
				if (lostMessagesCount) {
					LOGGER_DEBUG("Skipped {} lost messages.", lostMessagesCount);
				}

				// Mark the current message as seen
				m_clientStats.addResult(true);
				++m_messageCounterOtherNextExpected;

				m_otherStats = pingMessage->getPingStats();
				m_timeOfLastMessageFromOther = QDateTime::currentMSecsSinceEpoch();
				LOGGER_DEBUG("Updated stats from server.");
			} else if (messageType == Message::MessageType::MSGTYPE_QUIT) {
				std::shared_ptr<CloseMessage> const closeMessage = std::dynamic_pointer_cast<CloseMessage>(message);
				quint64 const uniqueId = closeMessage->getUniqueId();
				if (uniqueId != m_uniqueId) {
					LOGGER()->error("Received a close from Server with ID {}, which we do not know.", uniqueId);
					return;
				}

				LOGGER()->info("Received close from Server with reason {}.", closeMessage->getReason());
				m_isConnected = false;
				m_timerPing.stop();
				m_timerTimeout.stop();
				m_timerStats.stop();
			} else if (messageType == Message::MessageType::MSGTYPE_WELCOME) {
				std::shared_ptr<WelcomeMessage> const welcomeMessage = std::dynamic_pointer_cast<WelcomeMessage>(message);
				quint64 const uniqueId = welcomeMessage->getUniqueId();
				if (uniqueId != m_uniqueId) {
					LOGGER()->error("Received a welcome from Server with ID {}, which we do not know.", uniqueId);
					return;
				}

				LOGGER()->info("Received welcome from Server.");
			} else {
				LOGGER()->error("Received unhandled message type: {}", static_cast<int>(messageType));
			}
		}

	}
}
