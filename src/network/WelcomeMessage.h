#ifndef PACKAGELOSSUTILS_NETWORK_WELCOMEMESSAGE_H_
#define PACKAGELOSSUTILS_NETWORK_WELCOMEMESSAGE_H_

#include "src/network/Message.h"

namespace packagelossutils {
	namespace network {

		class WelcomeMessage : public Message {
		public:
			WelcomeMessage(quint64 uniqueId, quint32 pingIntervalInMs);
			virtual ~WelcomeMessage();

			static WelcomeMessage fromData(QByteArray const& data);

			virtual MessageType getMessageType() const override;

			quint32 getVersion() const;
			quint64 getUniqueId() const;
			quint32 getPingIntervalInMs() const;

			constexpr static quint32 PROTOCOL_VERSION = 0x1;
			constexpr static quint32 MESSAGE_ID = 0x1;
		protected:
			WelcomeMessage(quint32 version, quint64 uniqueId, quint32 pingIntervalInMs);
			static QByteArray toData(quint32 version, quint64 uniqueId, quint32 pingIntervalInMs);
		private:
			quint32 const m_version;
			quint64 const m_uniqueId;
			quint32 const m_pingIntervalInMs;
		};

	}
}

#endif
