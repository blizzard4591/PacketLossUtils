#include "src/network/Message.h"

#include "src/exceptions/ProtocolErrorException.h"

#include <QString>
#include <QtEndian>

namespace packetlossutils {
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

		QByteArray Message::prependMessageId(QByteArray const& data, quint32 messageId) {
			QByteArray result(sizeof(quint32), '\0');
			qToBigEndian(messageId, result.data());
			result.append(data);
			return result;
		}
		
		QByteArray Message::extractMessageId(QByteArray const& data, quint32& messageId, bool& ok) {
			ok = false;
			if (static_cast<std::size_t>(data.size()) < sizeof(quint32)) {
				throw packetlossutils::exceptions::ProtocolErrorException() << "Message had " << data.size() << " Bytes, we expected at least " << sizeof(quint32) << ".";
			}
			messageId = qFromBigEndian<quint32>(data.constData());
			ok = true;
			return data.mid(sizeof(quint32));
		}

	}
}
