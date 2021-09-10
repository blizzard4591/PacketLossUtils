#ifndef PACKAGELOSSUTILS_NETWORK_CLIENT_H_
#define PACKAGELOSSUTILS_NETWORK_CLIENT_H_

#include "src/network/NetworkCore.h"

#include <QHostAddress>
#include <QTimer>

namespace packagelossutils {
	namespace network {
		class Client : public NetworkCore {
			Q_OBJECT
		public:
			Client(QHostAddress const& address, quint16 port, QObject* parent = 0);
			virtual ~Client();

			bool connect();
		protected:
			QHostAddress const m_serverHostAddress;
			quint16 const m_serverPort;
			quint64 const m_uniqueId;

			bool m_isConnected;
			QTimer m_timer;

			virtual void receivedPaket(QNetworkDatagram const& datagram) override;
		};

	}
}

#endif
