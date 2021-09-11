#ifndef PACKAGELOSSUTILS_NETWORK_CLOSEMESSAGE_H_
#define PACKAGELOSSUTILS_NETWORK_CLOSEMESSAGE_H_

#include "src/network/Message.h"

namespace packagelossutils {
	namespace network {

		class CloseMessage : public Message {
		public:
			CloseMessage(quint64 uniqueId, quint32 reason);
			virtual ~CloseMessage();

			static CloseMessage fromData(QByteArray const& data);

			virtual MessageType getMessageType() const override;

			quint64 getUniqueId() const;
			quint32 getReason() const;

			constexpr static quint32 MESSAGE_ID = 0x3;

			constexpr static quint32 REASON_QUIT = 0x1ul;
			constexpr static quint32 REASON_TIMEOUT = 0x2ul;
		protected:
			static QByteArray toData(quint64 uniqueId, quint32 reason);
		private:
			quint64 const m_uniqueId;
			quint32 const m_reason;
		};

	}
}

#endif
