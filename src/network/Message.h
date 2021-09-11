#ifndef PACKAGELOSSUTILS_NETWORK_MESSAGE_H_
#define PACKAGELOSSUTILS_NETWORK_MESSAGE_H_

#include <QByteArray>

namespace packagelossutils {
	namespace network {

		class Message {
		public:
			Message(QByteArray const& data);
			virtual ~Message();

			QByteArray const& getData() const;

			static QByteArray prependMessageId(QByteArray const& data, quint32 messageId);
			static QByteArray extractMessageId(QByteArray const& data, quint32& messageId, bool& ok);

			enum class MessageType {
				MSGTYPE_WELCOME,
				MSGTYPE_PING,
				MSGTYPE_QUIT,
			};

			virtual MessageType getMessageType() const = 0;
		protected:
			//
		private:
			QByteArray const m_data;
		};

	}
}

#endif
