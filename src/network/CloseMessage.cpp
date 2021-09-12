#include "src/network/CloseMessage.h"

#include "src/exceptions/ProtocolErrorException.h"

#include <QDataStream>
#include <QString>
#include <QStringList>

namespace packetlossutils {
	namespace network {

		CloseMessage::CloseMessage(quint64 uniqueId, quint32 reason) : Message(toData(uniqueId, reason)), m_uniqueId(uniqueId), m_reason(reason) {
			//
		}

		CloseMessage::~CloseMessage() {
			//
		}

		CloseMessage CloseMessage::fromData(QByteArray const& data) {
			QString const message = QString::fromUtf8(data);
			QStringList const parts = message.split('\n');

			if (parts.size() != 2) {
				throw packetlossutils::exceptions::ProtocolErrorException() << "Close Message from server had " << parts.size() << " parts, we expected 2. Message: '" << message.toStdString() << "'.";
			}

			bool ok = false;
			quint64 const uniqueId = parts.at(0).toULongLong();
			if (!ok) {
				throw packetlossutils::exceptions::ProtocolErrorException() << "Could not parse message from server, could not understand uniqueID '" << parts.at(0).toStdString() << "'. Message: '" << message.toStdString() << "'.";
			}
			quint32 const reason = parts.at(1).toUInt(&ok);
			if (!ok) {
				throw packetlossutils::exceptions::ProtocolErrorException() << "Could not parse message from server, could not understand reason '" << parts.at(1).toStdString() << "'. Message: '" << message.toStdString() << "'.";
			}
			return CloseMessage(uniqueId, reason);
		}

		QByteArray CloseMessage::toData(quint64 uniqueId, quint32 reason) {
			return Message::prependMessageId(QStringLiteral("%1\n%2").arg(uniqueId).arg(reason).toUtf8(), MESSAGE_ID);
		}

		Message::MessageType CloseMessage::getMessageType() const {
			return MessageType::MSGTYPE_QUIT;
		}

		quint64 CloseMessage::getUniqueId() const {
			return m_uniqueId;
		}
		
		quint32 CloseMessage::getReason() const {
			return m_reason;
		}

	}
}
