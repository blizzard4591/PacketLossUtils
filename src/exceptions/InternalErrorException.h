#ifndef PACKETLOSSUTILS_EXCEPTIONS_INTERNALERROREXCEPTION_H_
#define PACKETLOSSUTILS_EXCEPTIONS_INTERNALERROREXCEPTION_H_

#include "src/exceptions/BaseException.h"
#include "src/exceptions/ExceptionMacros.h"

PACKETLOSSUTILS_NEW_EXCEPTION(InternalErrorExceptionImpl)
#define InternalErrorException() InternalErrorExceptionImpl(__FILE__, __LINE__)
 
#endif /* PACKETLOSSUTILS_EXCEPTIONS_INTERNALERROREXCEPTION_H_ */
