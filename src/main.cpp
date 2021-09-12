#include <QCoreApplication>
#include <QtCore>
#include <QCommandLineParser>
#include <QHostInfo>

#include "Init.h"
#include "src/network/Client.h"
#include "src/network/Server.h"
#include "src/utility/Version.h"

#include <cstdlib>
#include <cstdint>

int main(int argc, char *argv[]) {
    if (!initializeLogging(PACKETLOSSUTILS_LOGGING_MAX_FILESIZE, PACKETLOSSUTILS_LOGGING_MAX_FILECOUNT)) {
        return -2;
    }

	// Set encoding
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

	// Check for MacOSX Plugin directories
	QDirIterator pluginDirIt("/usr/local/Cellar/qt5-sqlcipher");
	QStringList pluginVersions;
	while (pluginDirIt.hasNext()) {
		QString const pluginDir = pluginDirIt.next();
		QString const fileName(pluginDirIt.fileName());
		if ((fileName.compare(QStringLiteral(".")) == 0) || (fileName.compare(QStringLiteral("..")) == 0)) {
			continue;
		}
		pluginVersions.append(pluginDir);
	}
	pluginVersions.sort();
	if (!pluginVersions.isEmpty()) {
		QCoreApplication::addLibraryPath(pluginVersions.last());
	}

	LOGGER()->info("Qt library load paths are: {}", QCoreApplication::libraryPaths().join(", ").toStdString());

	LOGGER()->info("Starting PacketLossUtils {}...", packetlossutils::utility::Version::longVersionString());

	int result = 0;
	try {
		QCoreApplication app(argc, argv);
		
		QCommandLineParser parser;
		parser.setApplicationDescription("PackageLoss Utilities");
		parser.addHelpOption();
		parser.addVersionOption();

		QCommandLineOption serverOption(QStringList() << "s" << "server", QCoreApplication::translate("main", "Run in Server mode"));
		QCommandLineOption clientOption(QStringList() << "c" << "client", QCoreApplication::translate("main", "Run in Client mode"), "server:port", "127.0.0.1:12702");
		QCommandLineOption debugOption(QStringList() << "d" << "debug", QCoreApplication::translate("main", "Show additional runtime debug output"));

		if (!parser.addOption(serverOption)) {
			LOGGER()->error("Failed to add 'serverOption' option!");
		}
		if (!parser.addOption(clientOption)) {
			LOGGER()->error("Failed to add 'clientOption' option!");
		}
		if (!parser.addOption(debugOption)) {
			LOGGER()->error("Failed to add 'debugOption' option!");
		}

		// Process the actual command line arguments given by the user
		parser.process(app);

		if (parser.isSet(serverOption)) {
			packetlossutils::network::Server server(12702, &app);

			LOGGER()->debug("Starting event loop.");
			result = app.exec();
		} else if (parser.isSet(clientOption)) {
			QString const optionValue = parser.value(clientOption);
			QString serverAddress = (optionValue.isEmpty()) ? "127.0.0.1" : optionValue;
			quint16 serverPort = 12702;
			int const pos = serverAddress.lastIndexOf(':');
			if (pos != -1) {
				serverPort = serverAddress.mid(pos + 1).toUInt();
				serverAddress = serverAddress.left(pos);
			}

			QHostInfo const hostInfo = QHostInfo::fromName(serverAddress);
			if (!hostInfo.addresses().isEmpty()) {
				QHostAddress address = hostInfo.addresses().first();
				// use the first IP address
				LOGGER_DEBUG("Using server '{}' with port {}. Input: '{}'", address.toString().toStdString(), serverPort, optionValue.toStdString());
				packetlossutils::network::Client client(address, serverPort, 250, &app);
				if (parser.isSet(debugOption)) {
					client.setDebugPingStats(true);
				}
				if (!client.connect()) {
					return -3;
				}

				LOGGER()->debug("Starting event loop.");
				result = app.exec();
			} else {
				LOGGER()->error("Could not resolve given server!");
				result = -2;
			}
		} else {
			LOGGER()->error("Don't know what to do. Good bye.");
		}
	} catch (std::exception& e) {
		std::cerr << "Caught exception: " << e.what() << std::endl;
		result = -1;
	}

	return result;
}
