#ifndef PACKETLOSSUTILS_EXCEPTIONS_INVALIDINPUTEXCEPTION_H_
#define PACKETLOSSUTILS_EXCEPTIONS_INVALIDINPUTEXCEPTION_H_

#include "src/exceptions/BaseException.h"
#include "src/exceptions/ExceptionMacros.h"

PACKETLOSSUTILS_NEW_EXCEPTION(InvalidInputExceptionImpl)
#define InvalidInputException() InvalidInputExceptionImpl(__FILE__, __LINE__)
 
#endif /* PACKETLOSSUTILS_EXCEPTIONS_INVALIDINPUTEXCEPTION_H_ */
