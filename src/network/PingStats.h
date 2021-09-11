#ifndef PACKAGELOSSUTILS_NETWORK_PINGSTATS_H_
#define PACKAGELOSSUTILS_NETWORK_PINGSTATS_H_

#include <QBitArray>
#include <QDataStream>
#include <QObject>
#include <QString>

#include "src/network/Message.h"

namespace packagelossutils {
	namespace network {
		class PingStats {
		public:
			virtual ~PingStats();

			constexpr static int MAX_BIT_COUNT = 2000;

			void addResult(bool didArrive);

			double getPercentReceivedOverTime(int intervals) const;

			static PingStats initialize();
			static PingStats fromDataStream(QDataStream& stream);

			QBitArray const& getBitArray() const;
			int getCurrentPosition() const;
			int getStoredBitCount() const;

			QString toString() const;
		protected:
			PingStats(QBitArray const& bitArray, int currentPosition, int storedBitCount);
		private:
			QBitArray m_bitArray;
			int m_currentPosition;
			int m_storedBitCount;
		};
	}
}

QDataStream& operator<< (QDataStream& stream, const packagelossutils::network::PingStats& t);
//QDataStream& operator>> (QDataStream& stream, packagelossutils::network::PingStats& t);

#endif
