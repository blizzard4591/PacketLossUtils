#ifndef OPENMITTSU_EXCEPTIONS_INVALIDPORTEXCEPTION_H_
#define OPENMITTSU_EXCEPTIONS_INVALIDPORTEXCEPTION_H_

#include "src/exceptions/BaseException.h"
#include "src/exceptions/ExceptionMacros.h"

PACKAGELOSSUTILS_NEW_EXCEPTION(InvalidPortExceptionImpl)
#define InvalidPortException() InvalidPortExceptionImpl(__FILE__, __LINE__)
 
#endif /* OPENMITTSU_EXCEPTIONS_INVALIDPORTEXCEPTION_H_ */
