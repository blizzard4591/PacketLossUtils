#ifndef PACKAGELOSSUTILS_NETWORK_SERVER_H_
#define PACKAGELOSSUTILS_NETWORK_SERVER_H_

#include "src/network/NetworkCore.h"

#include <QTimer>

namespace packagelossutils {
	namespace network {
		class Server : public NetworkCore {
			Q_OBJECT
		public:
			Server(quint16 port, QObject* parent = 0);
			virtual ~Server();
		protected:
			QTimer m_timer;

			virtual void receivedPaket(QNetworkDatagram const& datagram) override;
		};
	}
}

#endif
