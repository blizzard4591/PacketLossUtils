#ifndef PACKAGELOSSUTILS_UTILITY_OSDETECTION_H_
#define PACKAGELOSSUTILS_UTILITY_OSDETECTION_H_

#if defined __linux__ || defined __linux
#	define LINUX
#   define NOEXCEPT noexcept
#	define GetCurrentDir getcwd
#elif defined TARGET_OS_MAC || defined __apple__ || defined __APPLE__
#	define MACOSX
#   define NOEXCEPT noexcept
#	define _DARWIN_USE_64_BIT_INODE
#	define GetCurrentDir getcwd
#elif defined _WIN32 || defined _WIN64
#	define WINDOWS
#   define NOEXCEPT noexcept
#	ifndef NOMINMAX
#		define NOMINMAX
#		undef min
#		undef max
#	endif
//#	include <Windows.h>
//#	include <winnt.h>
//#	include <DbgHelp.h>
//#	include <Psapi.h>
//#	include <direct.h>
#	define GetCurrentDir _getcwd

#else
#	error Could not detect Operating System
#endif

namespace packagelossutils {
	namespace utility {
		//
	}
}

#endif
