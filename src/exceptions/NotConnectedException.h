#ifndef PACKETLOSSUTILS_EXCEPTIONS_NOTCONNECTEDEXCEPTION_H_
#define PACKETLOSSUTILS_EXCEPTIONS_NOTCONNECTEDEXCEPTION_H_

#include "src/exceptions/BaseException.h"
#include "src/exceptions/ExceptionMacros.h"

PACKETLOSSUTILS_NEW_EXCEPTION(NotConnectedExceptionImpl)
#define NotConnectedException() NotConnectedExceptionImpl(__FILE__, __LINE__)

#endif /* PACKETLOSSUTILS_EXCEPTIONS_NOTCONNECTEDEXCEPTION_H_ */
