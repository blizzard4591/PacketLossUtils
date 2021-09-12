#ifndef PACKETLOSSUTILS_EXCEPTIONS_CRYPTOEXCEPTION_H_
#define PACKETLOSSUTILS_EXCEPTIONS_CRYPTOEXCEPTION_H_

#include "src/exceptions/BaseException.h"
#include "src/exceptions/ExceptionMacros.h"

PACKETLOSSUTILS_NEW_EXCEPTION(CryptoExceptionImpl)
#define CryptoException() CryptoExceptionImpl(__FILE__, __LINE__)

#endif /* PACKETLOSSUTILS_EXCEPTIONS_CRYPTOEXCEPTION_H_ */
