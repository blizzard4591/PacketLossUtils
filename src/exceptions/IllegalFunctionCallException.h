#ifndef OPENMITTSU_EXCEPTIONS_ILLEGALFUNCTIONCALLEXCEPTION_H_
#define OPENMITTSU_EXCEPTIONS_ILLEGALFUNCTIONCALLEXCEPTION_H_

#include "src/exceptions/BaseException.h"
#include "src/exceptions/ExceptionMacros.h"

PACKAGELOSSUTILS_NEW_EXCEPTION(IllegalFunctionCallExceptionImpl)
#define IllegalFunctionCallException() IllegalFunctionCallExceptionImpl(__FILE__, __LINE__)

#endif /* OPENMITTSU_EXCEPTIONS_ILLEGALFUNCTIONCALLEXCEPTION_H_ */
