#include "src/network/WelcomeMessage.h"

#include "src/exceptions/ProtocolErrorException.h"

#include <QDataStream>
#include <QString>
#include <QStringList>

namespace packagelossutils {
	namespace network {

		WelcomeMessage::WelcomeMessage(quint32 version, quint64 uniqueId, quint32 pingIntervalInMs) : Message(toData(version, uniqueId, pingIntervalInMs)), m_version(version), m_uniqueId(uniqueId), m_pingIntervalInMs(pingIntervalInMs) {
			//
		}

		WelcomeMessage::WelcomeMessage(quint64 uniqueId, quint32 pingIntervalInMs) : WelcomeMessage(PROTOCOL_VERSION, uniqueId, pingIntervalInMs) {
			//
		}

		WelcomeMessage::~WelcomeMessage() {
			//
		}

		WelcomeMessage WelcomeMessage::fromData(QByteArray const& data) {
			QString const message = QString::fromUtf8(data);
			QStringList const parts = message.split('\n');

			if (parts.size() != 7) {
				throw packagelossutils::exceptions::ProtocolErrorException() << "Message from server had " << parts.size() << " parts, we expected 5. Message: '" << message.toStdString() << "'.";
			}

			bool ok = false;
			quint32 const version = parts.at(2).toUInt(&ok);
			if ((!ok) || (version != PROTOCOL_VERSION)) {
				throw packagelossutils::exceptions::ProtocolErrorException() << "Could not parse message from server, could not understand version '" << parts.at(2).toStdString() << "', we expected " << PROTOCOL_VERSION << ". Message: '" << message.toStdString() << "'.";
			}
			quint64 const uniqueId = parts.at(4).toULongLong(&ok);
			if (!ok) {
				throw packagelossutils::exceptions::ProtocolErrorException() << "Could not parse message from server, could not understand uniqueID '" << parts.at(4).toStdString() << "'. Message: '" << message.toStdString() << "'.";
			}
			quint32 const pingIntervalInMs = parts.at(6).toUInt(&ok);
			if (!ok) {
				throw packagelossutils::exceptions::ProtocolErrorException() << "Could not parse message from server, could not understand ping interval '" << parts.at(6).toStdString() << "'. Message: '" << message.toStdString() << "'.";
			}
			return WelcomeMessage(version, uniqueId, pingIntervalInMs);
		}

		QByteArray WelcomeMessage::toData(quint32 version, quint64 uniqueId, quint32 pingIntervalInMs) {
			return Message::prependMessageId(QStringLiteral("PACKAGELOSSUTILS\nVERSION\n%1\nID\n%2\nINTERVAL\n%3").arg(version).arg(uniqueId).arg(pingIntervalInMs).toUtf8(), MESSAGE_ID);
		}

		Message::MessageType WelcomeMessage::getMessageType() const {
			return MessageType::MSGTYPE_WELCOME;
		}

		quint32 WelcomeMessage::getVersion() const {
			return m_version;
		}
		
		quint64 WelcomeMessage::getUniqueId() const {
			return m_uniqueId;
		}
		
		quint32 WelcomeMessage::getPingIntervalInMs() const {
			return m_pingIntervalInMs;
		}

	}
}
