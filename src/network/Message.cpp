#include "src/network/Message.h"

#include <QString>

namespace packagelossutils {
	namespace network {

		Message::Message(QByteArray const& data) : m_data(data) {
			//
		}

		Message::~Message() {
			//
		}

		QByteArray const& Message::getData() const {
			return m_data;
		}

		Message Message::buildStartMessage(quint64 key) {
			QByteArray data;
			data.append(QStringLiteral("PACKAGELOSSUTILS\nVERSION\n1\n").toUtf8());
			data.append(key);
			return Message(data);
		}

	}
}
