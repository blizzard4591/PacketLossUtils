#ifndef PACKETLOSSUTILS_UTILITY_EXCEPTIONHANDLINGAPPLICATION_H_
#define PACKETLOSSUTILS_UTILITY_EXCEPTIONHANDLINGAPPLICATION_H_

#include <QCoreApplication>
#include <QObject>
#include <QEvent>

namespace packetlossutils {
	namespace utility {

		class ExceptionHandlingApplication : public QCoreApplication {
		public:
			ExceptionHandlingApplication(int& argc, char** argv);
			virtual ~ExceptionHandlingApplication();

			static void displayExceptionInfo(std::exception* e);
		};

	}
}

#endif
