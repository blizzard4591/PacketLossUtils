#include "src/network/PingMessage.h"

#include "src/exceptions/ProtocolErrorException.h"

#include <QDataStream>
#include <QString>
#include <QStringList>

namespace packagelossutils {
	namespace network {

		PingMessage::PingMessage(quint64 uniqueId, quint64 counter, PingStats const& pingStats) : Message(toData(uniqueId, counter, pingStats)), m_uniqueId(uniqueId), m_counter(counter), m_pingStats(pingStats) {
			//
		}

		PingMessage::~PingMessage() {
			//
		}

		PingMessage PingMessage::fromData(QByteArray const& data) {
			QDataStream stream(data);
			quint64 uniqueId;
			stream >> uniqueId;
			quint64 counter;
			stream >> counter;
			PingStats pingStats = PingStats::fromDataStream(stream);

			return PingMessage(uniqueId, counter, pingStats);
		}

		QByteArray PingMessage::toData(quint64 uniqueId, quint64 counter, PingStats const& pingStats) {
			QByteArray result;
			{
				QDataStream stream(&result, QIODevice::WriteOnly);
				stream << uniqueId;
				stream << counter;
				stream << pingStats;
			}

			return Message::prependMessageId(result, MESSAGE_ID);
		}

		Message::MessageType PingMessage::getMessageType() const {
			return MessageType::MSGTYPE_PING;
		}

		quint64 PingMessage::getUniqueId() const {
			return m_uniqueId;
		}

		quint64 PingMessage::getCounter() const {
			return m_counter;
		}

		PingStats const& PingMessage::getPingStats() const {
			return m_pingStats;
		}

	}
}
