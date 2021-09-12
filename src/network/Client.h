#ifndef PACKETLOSSUTILS_NETWORK_CLIENT_H_
#define PACKETLOSSUTILS_NETWORK_CLIENT_H_

#include "src/network/NetworkCore.h"

#include <QHostAddress>
#include <QTimer>

namespace packetlossutils {
	namespace network {
		class Client : public NetworkCore {
			Q_OBJECT
		public:
			Client(QHostAddress const& address, quint16 port, quint32 interval, QObject* parent = 0);
			virtual ~Client();

			bool connect();

			void setDebugPingStats(bool doDebug);
		public slots:
			virtual void onPingTimer() override;
			virtual void onTimeoutTimer() override;
			void onStatsTimer();
		protected:
			QHostAddress const m_serverHostAddress;
			quint16 const m_serverPort;
			quint64 const m_uniqueId;
			quint32 const m_interval;

			bool m_isConnected;
			QTimer m_timerStats;

			quint64 m_messageCounter;
			quint64 m_messageCounterOtherNextExpected;
			qint64 m_timeOfLastMessageFromOther;

			PingStats m_otherStats;
			PingStats m_clientStats;

			qint64 m_nextPingTime;

			bool m_doPingStatDebug;

			virtual void receivedMessage(QHostAddress const& address, quint16 port, std::shared_ptr<Message> const& message) override;
		};

	}
}

#endif
