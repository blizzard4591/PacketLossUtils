#include "src/network/Server.h"

#include "src/exceptions/InternalErrorException.h"
#include "src/exceptions/InvalidPortException.h"
#include "src/utility/Logging.h"

#include <cstdlib>
#include <iostream>

#include <QNetworkDatagram>

namespace packagelossutils {
	namespace network {

		Server::Server(quint16 port, QObject* parent) : NetworkCore(parent), m_timer(this) {
			if (!m_udpSocket->bind(QHostAddress::Any, port)) {
				LOGGER()->error("Failed to bind to port {}!", port);
				throw packagelossutils::exceptions::InvalidPortException() << "Failed to bind to port " << port << "!";
			}
		}

		Server::~Server() {
			m_timer.stop();
		}

		void Server::receivedPaket(QNetworkDatagram const& datagram) {
			// Send reply
			QByteArray const data = datagram.data();
			LOGGER()->info("Server received datagram of size {} from {}:{}.", data.size(), datagram.senderAddress().toString().toStdString(), (int)datagram.senderPort());
			m_udpSocket->writeDatagram(QByteArray("HELLO"), datagram.senderAddress(), datagram.senderPort());
		}
	}
}
