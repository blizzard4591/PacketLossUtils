#ifndef PACKETLOSSUTILS_EXCEPTIONS_ILLEGALFUNCTIONCALLEXCEPTION_H_
#define PACKETLOSSUTILS_EXCEPTIONS_ILLEGALFUNCTIONCALLEXCEPTION_H_

#include "src/exceptions/BaseException.h"
#include "src/exceptions/ExceptionMacros.h"

PACKETLOSSUTILS_NEW_EXCEPTION(IllegalFunctionCallExceptionImpl)
#define IllegalFunctionCallException() IllegalFunctionCallExceptionImpl(__FILE__, __LINE__)

#endif /* PACKETLOSSUTILS_EXCEPTIONS_ILLEGALFUNCTIONCALLEXCEPTION_H_ */
