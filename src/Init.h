#ifndef PACKAGELOSSUTILS_INIT_H_
#define PACKAGELOSSUTILS_INIT_H_

#include <cstdint>
#include <iostream>
#include <vector>

#include <QByteArray>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QStringList>

#include "src/utility/Logging.h"

#include <QSet>
#include <QSharedPointer>

bool initializeLogging(std::size_t maxLogfileSize, std::size_t maxFileCount) {
	// Get logging directory
	//QStandardPaths::setTestModeEnabled(isTesting);
	
	
#if defined(QT_VERSION) && (QT_VERSION >= QT_VERSION_CHECK(5, 4, 0))
	QStandardPaths::StandardLocation const location = QStandardPaths::AppLocalDataLocation;
#else
	QStandardPaths::StandardLocation const location = QStandardPaths::DataLocation;
#endif
	QString const writableLocationString = QStandardPaths::writableLocation(location);
	
	QString logFileName = "packetLossUtils.log";
	if (writableLocationString.isNull() || writableLocationString.isEmpty()) {
		std::cerr << "Qt StandardPaths returned no writable location, defaulting to local." << std::endl;
	} else {
		QDir folder(writableLocationString);		
		if (!folder.mkpath(writableLocationString)) {
			std::cerr << "Could not create directory for log files \"" << writableLocationString.toStdString() << "\", defaulting to local." << std::endl;
		} else {
			logFileName = folder.absoluteFilePath("packetLossUtils.log");
		}
	}
	
	QFile logFile(logFileName);
	if (!logFile.open(QIODevice::WriteOnly | QIODevice::Append)) {
		std::cerr << "Error: The log file \"" << logFileName.toStdString() << "\" is not writable!" << std::endl;
		return false;
	}
	logFile.close();	
	std::cout << "Logging location: " << logFileName.toStdString() << std::endl;
	
	try {
		std::vector<spdlog::sink_ptr> sinks;
		sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_st>());
		sinks.push_back(std::make_shared<spdlog::sinks::rotating_file_sink_mt>(logFileName.toStdString(), maxLogfileSize, maxFileCount));
		auto combined_logger = std::make_shared<spdlog::logger>(PACKAGELOSSUTILS_LOGGING_LOGGER_MAIN_NAME, begin(sinks), end(sinks));

		// Set log level depending on Build Type
		combined_logger->set_level(PACKAGELOSSUTILS_LOGGING_LEVEL);

		spdlog::register_logger(combined_logger);
	} catch (const spdlog::spdlog_ex& ex) {
		std::cerr << "Setting up the logger failed: " << ex.what() << std::endl;
		return false;
	}

	return true;
}

#endif
