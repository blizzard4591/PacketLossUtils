#ifndef PACKETLOSSUTILS_UTILITY_VERSION_H_
#define PACKETLOSSUTILS_UTILITY_VERSION_H_
#include <string>
#include <sstream>

namespace packetlossutils {
	namespace utility {

		struct Version {
			/// The major version of PacketLossUtils
			const static unsigned versionMajor;
			/// The minor version of PacketLossUtils
			const static unsigned versionMinor;
			/// The patch version of PacketLossUtils
			const static unsigned versionPatch;
			/// The short hash of the git commit this build is bases on
			const static std::string gitRevisionHash;
			/// How many commits passed since the last tag was set
			const static unsigned commitsAhead;
			/// 0 iff no files were modified in the checkout, 1 else
			const static unsigned dirty;
			/// The system which has compiled PacketLossUtils
			const static std::string systemName;
			/// The size of a pointer of the system that has compiled PacketLossUtils
			const static std::string systemPtrSize;
			/// The system version which has compiled PacketLossUtils
			const static std::string systemVersion;
			/// The build type that was used to build PacketLossUtils
			const static std::string buildType;
			/// The compiler version that was used to build PacketLossUtils
			const static std::string cxxCompiler;

			static std::string versionWithTagString() {
				std::stringstream sstream;
				sstream << versionMajor << "." << versionMinor << "." << versionPatch;
				if (commitsAhead != 0) {
					sstream << "+" << commitsAhead;
				}
				sstream << "-" << gitRevisionHash;
				if (dirty == 1) {
					sstream << "-DIRTY";
				}

				return sstream.str();
			}

			static std::string shortVersionString() {
				std::stringstream sstream;
				sstream << "PackageLossUtility " << versionMajor << "." << versionMinor << "." << versionPatch;
				return sstream.str();
			}

			static std::string longVersionString() {
				std::stringstream sstream;
				sstream << "Version: " << versionMajor << "." << versionMinor << "." << versionPatch;
				if (commitsAhead != 0) {
					sstream << " (+" << commitsAhead << " commits)";
				}
				sstream << " build from revision " << gitRevisionHash;
				if (dirty == 1) {
					sstream << " (DIRTY)";
				}
				sstream << ".";
				return sstream.str();
			}

			static std::string buildInfo() {
				std::stringstream sstream;
				sstream << "Compiled on " << systemName << " " << systemVersion << " ";
				sstream << "using compiler version " << cxxCompiler;
				if (buildType != "") {
					sstream << " with " << buildType << " flags";
				}
				sstream << ".";
				return sstream.str();
			}
		};

	}
}

#endif
