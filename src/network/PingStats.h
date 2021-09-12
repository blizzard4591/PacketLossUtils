#ifndef PACKETLOSSUTILS_NETWORK_PINGSTATS_H_
#define PACKETLOSSUTILS_NETWORK_PINGSTATS_H_

#include <QBitArray>
#include <QDataStream>
#include <QObject>
#include <QString>

#include "src/network/Message.h"

namespace packetlossutils {
	namespace network {
		class PingStats {
		public:
			virtual ~PingStats();

			constexpr static int MAX_BIT_COUNT = 2000;

			void addResult(bool didArrive);

			// Count a packet send on our end
			void addPacketSent();

			// Cound a packet received from the other end
			void addPacketReceived();

			double getPercentReceivedOverTime(int intervals) const;

			static PingStats initialize();
			static PingStats fromDataStream(QDataStream& stream);

			QBitArray const& getBitArray() const;
			int getCurrentPosition() const;
			int getStoredBitCount() const;

			quint64 getPacketsSent() const;
			quint64 getPacketsReceived() const;

			QString toString() const;
		protected:
			PingStats(QBitArray const& bitArray, int currentPosition, int storedBitCount);
			PingStats(QBitArray const& bitArray, int currentPosition, int storedBitCount, quint64 packetsSent, quint64 packetsReceived);
		private:
			QBitArray m_bitArray;
			int m_currentPosition;
			int m_storedBitCount;

			quint64 m_packetsSent;
			quint64 m_packetsReceived;
		};
	}
}

QDataStream& operator<< (QDataStream& stream, const packetlossutils::network::PingStats& t);
//QDataStream& operator>> (QDataStream& stream, packetlossutils::network::PingStats& t);

#endif
