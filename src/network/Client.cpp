#include "src/network/Client.h"

#include "src/exceptions/InternalErrorException.h"
#include "src/exceptions/InvalidPortException.h"
#include "src/utility/Logging.h"

#include "src/network/Message.h"

#include <QRandomGenerator>
#include <QThread>

#include <cstdlib>
#include <iostream>


namespace packagelossutils {
	namespace network {

		Client::Client(QHostAddress const& address, quint16 port, QObject* parent) : NetworkCore(parent), m_serverHostAddress(address), m_serverPort(port), m_uniqueId(QRandomGenerator::global()->generate64()), m_isConnected(false), m_timer(this) {
			//
			LOGGER_DEBUG("Client configured for {}:{} with unique ID {}.", m_serverHostAddress.toString().toStdString(), (int)m_serverPort, m_uniqueId);
		}

		Client::~Client() {
			//
		}

		bool Client::connect() {
			if (m_isConnected) {
				LOGGER()->warn("Was already connected, connect() was called again!");
				return false;
			}

			Message welcomeMessage = Message::buildStartMessage(m_uniqueId);
			m_udpSocket->writeDatagram(welcomeMessage.getData(), m_serverHostAddress, m_serverPort);

			bool receivedReply = false;
			for (int i = 0; i < 10; ++i) {
				QThread::msleep(100);
				if (m_udpSocket->hasPendingDatagrams()) {
					receivedReply = true;
					break;
				}
			}

			if (!receivedReply) {
				LOGGER()->error("Failed to connect to server {}:{}.", m_serverHostAddress.toString().toStdString(), (int)m_serverPort);
				return false;
			}
			LOGGER()->info("Received reply from server...");

			return true;
		}

		void Client::receivedPaket(QNetworkDatagram const& datagram) {
			QByteArray const data = datagram.data();
			LOGGER()->info("Client received datagram of size {} from {}:{}.", data.size(), datagram.senderAddress().toString().toStdString(), datagram.senderPort());
			//
		}

	}
}
