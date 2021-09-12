#ifndef PACKETLOSSUTILS_EXCEPTIONS_PROTOCOLERROREXCEPTION_H_
#define PACKETLOSSUTILS_EXCEPTIONS_PROTOCOLERROREXCEPTION_H_

#include "src/exceptions/BaseException.h"
#include "src/exceptions/ExceptionMacros.h"

PACKETLOSSUTILS_NEW_EXCEPTION(ProtocolErrorExceptionImpl)
#define ProtocolErrorException() ProtocolErrorExceptionImpl(__FILE__, __LINE__)

#endif /* PACKETLOSSUTILS_EXCEPTIONS_PROTOCOLERROREXCEPTION_H_ */
