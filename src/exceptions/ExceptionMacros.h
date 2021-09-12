#ifndef PACKETLOSSUTILS_EXCEPTIONS_EXCEPTIONMACROS_H_
#define PACKETLOSSUTILS_EXCEPTIONS_EXCEPTIONMACROS_H_

/*!
 * Macro to generate descendant exception classes. As all classes are nearly the same, this makes changing common
 * features much easier.
 */
#define PACKETLOSSUTILS_NEW_EXCEPTION(exception_name) namespace packetlossutils { \
namespace exceptions { \
class exception_name : public BaseException { \
public: \
exception_name(char const* file, int line) NOEXCEPT : BaseException(file, line) { \
} \
exception_name(char const* file, int line, char const* cstr) NOEXCEPT : BaseException(file, line, cstr) { \
} \
exception_name(exception_name const& cp) NOEXCEPT : BaseException(cp) { \
} \
virtual ~exception_name() NOEXCEPT { \
} \
template<typename T> \
exception_name& operator<<(T const& var) { \
	this->stream << var; \
	return *this; \
} \
virtual const char* name() const NOEXCEPT override { \
	return #exception_name; \
} \
}; \
} \
}

#endif /* PACKETLOSSUTILS_EXCEPTIONS_EXCEPTIONMACROS_H_ */
