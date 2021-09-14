#ifndef PACKETLOSSUTILS_NETWORK_SERVER_H_
#define PACKETLOSSUTILS_NETWORK_SERVER_H_

#include "src/network/NetworkCore.h"
#include "src/network/PingStats.h"

#include <QMap>
#include <QTimer>

namespace packetlossutils {
	namespace network {
		class Server : public NetworkCore {
			Q_OBJECT
		public:
			Server(quint16 port, QObject* parent = 0);
			virtual ~Server();

			constexpr static quint32 BASE_TIMER_INTERVAL = 250;
			constexpr static qint64 CLIENT_TIMEOUT_MS = 60ll * 1000ll;
		public slots:
			virtual void onPingTimer() override;
			virtual void onTimeoutTimer() override;

			virtual void onSigIntReceived() override;
		protected:

			struct ClientInfo {
				QHostAddress address;
				quint16 port;
				quint64 messageCounter;
				quint64 messageCounterOtherNextExpected;
				quint32 interval;
				quint32 intervalCounter;
				qint64 timeOfLastMessageFromOther;
				qint64 timeOfLastRealMessageFromOther;
				PingStats pingStats;

				ClientInfo(QHostAddress address, quint16 port, quint32 interval);
			};

			QMap<quint64, std::shared_ptr<ClientInfo>> m_clientInfos;
			qint64 m_nextPingTime;

			virtual void receivedMessage(QHostAddress const& address, quint16 port, std::shared_ptr<Message> const& message) override;

			void startOrStopTimersIfNeeded();
		};
	}
}

#endif
