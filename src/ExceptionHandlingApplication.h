#ifndef PACKETLOSSUTILS_UTILITY_EXCEPTIONHANDLINGAPPLICATION_H_
#define PACKETLOSSUTILS_UTILITY_EXCEPTIONHANDLINGAPPLICATION_H_

#include <QCoreApplication>
#include <QObject>
#include <QEvent>

#include "src/utility/SignalHandler.h"

namespace packetlossutils {
	namespace utility {

		class ExceptionHandlingApplication : public QCoreApplication, public SignalHandler {
			Q_OBJECT
		public:
			ExceptionHandlingApplication(int& argc, char** argv);
			virtual ~ExceptionHandlingApplication();

			static void displayExceptionInfo(std::exception* e);

			virtual bool handleSignal(int signal) override;
		signals:
			void onSigInt();
		};

	}
}

#endif
