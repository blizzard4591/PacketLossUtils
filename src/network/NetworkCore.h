#ifndef PACKAGELOSSUTILS_NETWORK_NETWORKCORE_H_
#define PACKAGELOSSUTILS_NETWORK_NETWORKCORE_H_

#include <QNetworkDatagram>
#include <QObject>
#include <QTimer>
#include <QUdpSocket>

#include "src/network/Message.h"
#include "src/network/PingStats.h"

namespace packagelossutils {
	namespace network {
		class NetworkCore : public QObject {
			Q_OBJECT
		public:
			NetworkCore(QObject* parent = 0);
			virtual ~NetworkCore();

			constexpr static quint32 MAX_TIME_LATE_MS = 100;
		public slots:
			void readPendingDatagrams();
			virtual void onPingTimer() = 0;
			virtual void onTimeoutTimer() = 0;
		protected:
			std::unique_ptr<QUdpSocket> m_udpSocket;

			QTimer m_timerPing;
			QTimer m_timerTimeout;
			
			virtual void receivedMessage(QHostAddress const&, quint16, std::shared_ptr<Message> const&) {}

			static std::shared_ptr<Message> parseIncomingMessage(QByteArray const& data);
		};
	}
}

#endif
