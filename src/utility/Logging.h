#ifndef PACKETLOSSUTILS_UTILITY_LOGGING_H_
#define PACKETLOSSUTILS_UTILITY_LOGGING_H_

// Enable debug log level depending on the chosen configuration
#if ((defined(_MSC_VER) && defined(_DEBUG)) || (defined(PACKETLOSSUTILS_CONFIG_DEBUG_FLAG_SET)))
#	ifndef SPDLOG_DEBUG_ON
#		define SPDLOG_DEBUG_ON
#	endif
#	ifndef SPDLOG_ACTIVE_LEVEL
#		define SPDLOG_ACTIVE_LEVEL 0
#	endif
#	define PACKETLOSSUTILS_LOGGING_LEVEL spdlog::level::debug
#else
#	define PACKETLOSSUTILS_LOGGING_LEVEL spdlog::level::info
#endif

#define SPDLOG_HEADER_ONLY
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>

#define PACKETLOSSUTILS_LOGGING_LOGGER_MAIN_NAME "main"
#define PACKETLOSSUTILS_LOGGING_MAX_FILESIZE (5 * 1024 * 1024) // 5 MBytes
#define PACKETLOSSUTILS_LOGGING_MAX_FILECOUNT (3) // 3 Files

#define LOGGER() spdlog::get(PACKETLOSSUTILS_LOGGING_LOGGER_MAIN_NAME)
#define LOGGER_DEBUG(...) do { SPDLOG_LOGGER_DEBUG(LOGGER(), __VA_ARGS__); } while (false)

#endif
