#ifndef PACKETLOSSUTILS_EXCEPTIONS_ILLEGALARGUMENTEXCEPTION_H_
#define PACKETLOSSUTILS_EXCEPTIONS_ILLEGALARGUMENTEXCEPTION_H_

#include "src/exceptions/BaseException.h"
#include "src/exceptions/ExceptionMacros.h"

PACKETLOSSUTILS_NEW_EXCEPTION(IllegalArgumentExceptionImpl)
#define IllegalArgumentException() IllegalArgumentExceptionImpl(__FILE__, __LINE__)

#endif /* PACKETLOSSUTILS_EXCEPTIONS_ILLEGALARGUMENTEXCEPTION_H_ */
