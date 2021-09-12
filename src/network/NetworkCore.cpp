#include "src/network/NetworkCore.h"

#include "src/exceptions/InternalErrorException.h"
#include "src/exceptions/InvalidPortException.h"
#include "src/exceptions/ProtocolErrorException.h"
#include "src/utility/Logging.h"

#include <cstdlib>
#include <iostream>

#include "src/network/WelcomeMessage.h"
#include "src/network/PingMessage.h"
#include "src/network/CloseMessage.h"

#include <QDateTime>
#include <QNetworkDatagram>

namespace packetlossutils {
	namespace network {

		NetworkCore::NetworkCore(QObject* parent) : QObject(parent),
			m_timerPing(this), m_timerTimeout(this) {
			//
			m_udpSocket = std::make_unique<QUdpSocket>();
			if (!QObject::connect(m_udpSocket.get(), SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()))) {
				throw packetlossutils::exceptions::InternalErrorException() << "Failed to bind readyRead() on UDP Socket!";
			}

			if (!QObject::connect(&m_timerPing, SIGNAL(timeout()), this, SLOT(onPingTimer()))) {
				throw packetlossutils::exceptions::InternalErrorException() << "Failed to bind onPingTimer() on Timer!";
			}
			if (!QObject::connect(&m_timerTimeout, SIGNAL(timeout()), this, SLOT(onTimeoutTimer()))) {
				throw packetlossutils::exceptions::InternalErrorException() << "Failed to bind onTimeoutTimer() on Timer!";
			}
		}

		NetworkCore::~NetworkCore() {
			m_timerPing.stop();
			m_timerTimeout.stop();

			m_udpSocket->close();
		}

		void NetworkCore::readPendingDatagrams() {
			while (m_udpSocket->hasPendingDatagrams()) {
				QNetworkDatagram datagram = m_udpSocket->receiveDatagram();
				QByteArray const data = datagram.data();
				if (data.size() == 0) {
					LOGGER()->warn("Received empty message, ignoring.");
				} else {
					std::shared_ptr<Message> const message = parseIncomingMessage(data);
					if (message != nullptr) {
						receivedMessage(datagram.senderAddress(), datagram.senderPort(), message);
					}
				}
			}
		}

		std::shared_ptr<Message> NetworkCore::parseIncomingMessage(QByteArray const& data) {
			quint32 messageId = 0;
			bool ok = false;
			QByteArray const message = Message::extractMessageId(data, messageId, ok);
			if (!ok) {
				return nullptr;
			}

			if (messageId == WelcomeMessage::MESSAGE_ID) {
				return std::make_shared<WelcomeMessage>(WelcomeMessage::fromData(message));
			} else if (messageId == PingMessage::MESSAGE_ID) {
				return std::make_shared<PingMessage>(PingMessage::fromData(message));
			} else if (messageId == CloseMessage::MESSAGE_ID) {
				return std::make_shared<CloseMessage>(CloseMessage::fromData(message));
			} else {
				LOGGER()->error("Unhandled message type: {}. Dropping.", messageId);
				return nullptr;
			}
		}

		
	}
}
