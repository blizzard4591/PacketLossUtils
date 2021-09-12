#ifndef PACKETLOSSUTILS_EXCEPTIONS_INSUFFICIENTINPUTEXCEPTION_H_
#define PACKETLOSSUTILS_EXCEPTIONS_INSUFFICIENTINPUTEXCEPTION_H_

#include "src/exceptions/BaseException.h"
#include "src/exceptions/ExceptionMacros.h"

PACKETLOSSUTILS_NEW_EXCEPTION(InsufficientInputExceptionImpl)
#define InsufficientInputException() InsufficientInputExceptionImpl(__FILE__, __LINE__)
 
#endif /* PACKETLOSSUTILS_EXCEPTIONS_INSUFFICIENTINPUTEXCEPTION_H_ */
