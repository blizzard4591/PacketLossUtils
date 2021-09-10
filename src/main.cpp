#include <QCoreApplication>
#include <QtCore>
#include <QCommandLineParser>
#include <QSslSocket>

#include "Init.h"
#include "src/network/Client.h"
#include "src/network/Server.h"
#include "src/utility/Version.h"

#include <cstdlib>
#include <cstdint>

Q_DECLARE_METATYPE(QSslSocket::SslMode);

int main(int argc, char *argv[]) {
    if (!initializeLogging(PACKAGELOSSUTILS_LOGGING_MAX_FILESIZE, PACKAGELOSSUTILS_LOGGING_MAX_FILECOUNT)) {
        return -2;
    }

	qRegisterMetaType<QSslSocket::SslMode>();

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

	LOGGER()->info("Starting PackageLossUtils {}...", packagelossutils::utility::Version::longVersionString());

	int result = 0;
	try {
		QCoreApplication app(argc, argv);
		
		QCommandLineParser parser;
		parser.setApplicationDescription("PackageLoss Utilities");
		parser.addHelpOption();
		parser.addVersionOption();

		QCommandLineOption serverOption(QStringList() << "s" << "server", QCoreApplication::translate("main", "Run in Server mode"));
		QCommandLineOption clientOption(QStringList() << "c" << "client", QCoreApplication::translate("main", "Run in Client mode"));

		if (!parser.addOption(serverOption)) {
			LOGGER()->error("Failed to add 'serverOption' option!");
		}
		if (!parser.addOption(clientOption)) {
			LOGGER()->error("Failed to add 'clientOption' option!");
		}

		// Process the actual command line arguments given by the user
		parser.process(app);

		/*
		QString certificateFileName = "server.crt";
		if (parser.isSet(certOption)) {
			LOGGER_DEBUG("Certificate cmd option is set, using value '{}'", parser.value(certOption).toStdString());
			certificateFileName = parser.value(certOption);
		}

		QString keyFileName = "server.key";
		if (parser.isSet(keyOption)) {
			LOGGER_DEBUG("Key cmd option is set, using value '{}'", parser.value(keyOption).toStdString());
			keyFileName = parser.value(keyOption);
		}

		QString chainFileName = "chain.crt";
		if (parser.isSet(chainOption)) {
			LOGGER_DEBUG("Chain cmd option is set, using value '{}'", parser.value(chainOption).toStdString());
			chainFileName = parser.value(chainOption);
		}
		*/

		if (parser.isSet(serverOption)) {
			packagelossutils::network::Server server(12702, &app);

			LOGGER()->debug("Starting event loop.");
			result = app.exec();
		} else if (parser.isSet(clientOption)) {
			packagelossutils::network::Client client(QHostAddress(QHostAddress::LocalHost), 12702, &app);
			client.connect();

			LOGGER()->debug("Starting event loop.");
			result = app.exec();
		} else {
			LOGGER()->error("Don't know what to do. Good bye.");
		}
	} catch (std::exception& e) {
		std::cerr << "Caught exception: " << e.what() << std::endl;
		result = -1;
	}

	return result;
}
