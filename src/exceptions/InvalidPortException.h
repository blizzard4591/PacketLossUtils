#ifndef PACKETLOSSUTILS_EXCEPTIONS_INVALIDPORTEXCEPTION_H_
#define PACKETLOSSUTILS_EXCEPTIONS_INVALIDPORTEXCEPTION_H_

#include "src/exceptions/BaseException.h"
#include "src/exceptions/ExceptionMacros.h"

PACKETLOSSUTILS_NEW_EXCEPTION(InvalidPortExceptionImpl)
#define InvalidPortException() InvalidPortExceptionImpl(__FILE__, __LINE__)
 
#endif /* PACKETLOSSUTILS_EXCEPTIONS_INVALIDPORTEXCEPTION_H_ */
