#ifndef PACKAGELOSSUTILS_NETWORK_MESSAGE_H_
#define PACKAGELOSSUTILS_NETWORK_MESSAGE_H_

#include <QByteArray>

namespace packagelossutils {
	namespace network {

		class Message {
		public:
			Message(QByteArray const& data);
			virtual ~Message();

			static Message buildStartMessage(quint64 key);

			QByteArray const& getData() const;
		protected:
			//
		private:
			QByteArray const m_data;
		};

	}
}

#endif
