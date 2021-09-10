#ifndef PACKAGELOSSUTILS_NETWORK_NETWORKCORE_H_
#define PACKAGELOSSUTILS_NETWORK_NETWORKCORE_H_

#include <QNetworkDatagram>
#include <QObject>
#include <QUdpSocket>

namespace packagelossutils {
	namespace network {
		class NetworkCore : public QObject {
			Q_OBJECT
		public:
			NetworkCore(QObject* parent = 0);
			virtual ~NetworkCore();
		public slots:
			void readPendingDatagrams();
		protected:
			std::unique_ptr<QUdpSocket> m_udpSocket;

			virtual void receivedPaket(QNetworkDatagram const& datagram) {}
		};
	}
}

#endif
