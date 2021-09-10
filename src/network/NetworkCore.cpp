#include "src/network/NetworkCore.h"

#include "src/exceptions/InternalErrorException.h"
#include "src/exceptions/InvalidPortException.h"
#include "src/utility/Logging.h"

#include <cstdlib>
#include <iostream>

#include <QNetworkDatagram>

namespace packagelossutils {
	namespace network {

		NetworkCore::NetworkCore(QObject* parent) : QObject(parent) {
			//
			m_udpSocket = std::make_unique<QUdpSocket>();
			if (!QObject::connect(m_udpSocket.get(), SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()))) {
				throw packagelossutils::exceptions::InternalErrorException() << "Failed to bind readyRead() on UDP Socket!";
			}
		}

		NetworkCore::~NetworkCore() {
			m_udpSocket->close();
		}

		void NetworkCore::readPendingDatagrams() {
			while (m_udpSocket->hasPendingDatagrams()) {
				QNetworkDatagram datagram = m_udpSocket->receiveDatagram();
				receivedPaket(datagram);
			}
		}
	}
}
