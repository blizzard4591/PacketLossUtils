#include "src/network/PingStats.h"

#include "src/exceptions/InternalErrorException.h"
#include "src/exceptions/InvalidPortException.h"
#include "src/exceptions/ProtocolErrorException.h"
#include "src/utility/Logging.h"

#include <cstdlib>
#include <iostream>

namespace packagelossutils {
	namespace network {

		PingStats::PingStats(QBitArray const& bitArray, int currentPosition, int storedBitCount) : PingStats(bitArray, currentPosition, storedBitCount, 0, 0) {
			//
		}

		PingStats::PingStats(QBitArray const& bitArray, int currentPosition, int storedBitCount, quint64 packetsSent, quint64 packetsReceived) : m_bitArray(bitArray), m_currentPosition(currentPosition), m_storedBitCount(storedBitCount), m_packetsSent(packetsSent), m_packetsReceived(packetsReceived) {
			//
		}

		PingStats::~PingStats() {
			//
		}

		PingStats PingStats::initialize() {
			return PingStats(QBitArray(MAX_BIT_COUNT), -1, 0);
		}

		void PingStats::addResult(bool didArrive) {
			m_currentPosition = (m_currentPosition + 1) % MAX_BIT_COUNT;
			m_bitArray.setBit(m_currentPosition, didArrive);
			if (m_storedBitCount < MAX_BIT_COUNT) {
				++m_storedBitCount;
			}
		}

		void PingStats::addPacketSent() {
			++m_packetsSent;
		}
		
		void PingStats::addPacketReceived() {
			++m_packetsReceived;
		}

		double PingStats::getPercentReceivedOverTime(int intervals) const {
			int const bitsToLookAt = std::min(m_storedBitCount, intervals);
			if (bitsToLookAt <= 0) {
				return 0.0;
			}

			int pos = m_currentPosition;
			int received = 0;
			for (int i = 0; i < bitsToLookAt; ++i) {
				if (m_bitArray.testBit(pos)) {
					++received;
				}
				--pos;
				if (pos < 0) {
					pos += MAX_BIT_COUNT;
				}
			}
			//LOGGER_DEBUG("currentPosition = {}, bitsToLookAt = {}, storedBitCount = {}, received = {}", m_currentPosition, bitsToLookAt, m_storedBitCount, received);
			return (static_cast<double>(received) / static_cast<double>(bitsToLookAt)) * 100.0;
		}

		QBitArray const& PingStats::getBitArray() const {
			return m_bitArray;
		}
		
		int PingStats::getCurrentPosition() const {
			return m_currentPosition;
		}
		
		int PingStats::getStoredBitCount() const {
			return m_storedBitCount;
		}

		quint64 PingStats::getPacketsSent() const {
			return m_packetsSent;
		}

		quint64 PingStats::getPacketsReceived() const {
			return m_packetsReceived;
		}

		PingStats PingStats::fromDataStream(QDataStream& stream) {
			QBitArray bitArray;
			stream >> bitArray;
			int currentPosition;
			stream >> currentPosition;
			int storedBitCount;
			stream >> storedBitCount;
			quint64 packetsSent;
			stream >> packetsSent;
			quint64 packetsReceived;
			stream >> packetsReceived;

			return PingStats(bitArray, currentPosition, storedBitCount, packetsSent, packetsReceived);
		}

		QString PingStats::toString() const {
			QString result;
			result.reserve(4000);
			result.append("PingStats(");
			int pos = m_currentPosition;
			for (int i = 0; i < m_storedBitCount; ++i) {
				if ((i > 0) && ((i % 5) == 0)) {
					result.append(' ');
				}
				if (m_bitArray.testBit(pos)) {
					result.append('1');
				} else {
					result.append('0');
				}

				--pos;
				if (pos < 0) {
					pos += MAX_BIT_COUNT;
				}
			}
			result.append(", currentPosition = ");
			result.append(QString::number(m_currentPosition));
			result.append(", storedBitCount = ");
			result.append(QString::number(m_storedBitCount));
			result.append(", packetsSent = ");
			result.append(QString::number(m_packetsSent));
			result.append(", packetsReceived = ");
			result.append(QString::number(m_packetsReceived));
			result.append(")");
			return result;
		}
	}
}

QDataStream& operator<< (QDataStream& stream, const packagelossutils::network::PingStats& t) {
	stream << t.getBitArray();
	stream << t.getCurrentPosition();
	stream << t.getStoredBitCount();
	stream << t.getPacketsSent();
	stream << t.getPacketsReceived();

	return stream;
}

