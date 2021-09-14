#include "src/ExceptionHandlingApplication.h"

#include <iostream>
#include <QThread>
#include "src/exceptions/BaseException.h"
#include "src/utility/Logging.h"

namespace packetlossutils {
	namespace utility {

		ExceptionHandlingApplication::ExceptionHandlingApplication(int& argc, char** argv) : QCoreApplication(argc, argv), SignalHandler(SignalHandler::SIG_INT) {
			// Intentionally left empty.
		}

		ExceptionHandlingApplication::~ExceptionHandlingApplication() {
			// Intentionally left empty.
		}

		void ExceptionHandlingApplication::displayExceptionInfo(std::exception* e) {
			packetlossutils::exceptions::BaseException* baseException = dynamic_cast<packetlossutils::exceptions::BaseException*>(e);
			if (baseException != nullptr) {
				LOGGER()->critical("Caught an exception of type {} in the main program loop with cause: {}", baseException->name(), baseException->what());
				std::cerr << "Caught an exception of type " << baseException->name() << " in the main program loop with cause: " << baseException->what() << std::endl;
			} else if (e != nullptr) {
				LOGGER()->critical("Caught an exception in the main program loop with cause: {}", e->what());
				std::cerr << "Caught an exception in the main program loop with cause: " << e->what() << std::endl;
			} else {
				LOGGER()->critical("Caught a NULLPTR exception in the main program loop, this should never happen?!");
				std::cerr << "Caught a NULLPTR exception in the main program loop, this should never happen?!" << std::endl;
			}
		}

		bool ExceptionHandlingApplication::handleSignal(int signal) {
			emit onSigInt();
			return true;
		}

	}
}
