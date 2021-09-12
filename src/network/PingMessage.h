#ifndef PACKETLOSSUTILS_NETWORK_PINGMESSAGE_H_
#define PACKETLOSSUTILS_NETWORK_PINGMESSAGE_H_

#include "src/network/Message.h"
#include "src/network/PingStats.h"

namespace packetlossutils {
	namespace network {

		class PingMessage : public Message {
		public:
			PingMessage(quint64 uniqueId, quint64 counter, PingStats const& pingStats);
			virtual ~PingMessage();

			static PingMessage fromData(QByteArray const& data);

			virtual MessageType getMessageType() const override;

			quint64 getUniqueId() const;
			quint64 getCounter() const;
			PingStats const& getPingStats() const;

			constexpr static quint32 MESSAGE_ID = 0x2;
		protected:
			static QByteArray toData(quint64 uniqueId, quint64 counter, PingStats const& pingStats);
		private:
			quint64 const m_uniqueId;
			quint64 const m_counter;
			PingStats const m_pingStats;
		};

	}
}

#endif
