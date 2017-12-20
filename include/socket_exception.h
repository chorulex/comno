#ifndef _QT_SOCKET_EXCEPTION_H_
#define _QT_SOCKET_EXCEPTION_H_

#include <exception>
#include "error_code.h"

namespace QtSocket
{

class SocketException : public std::exception
{
public:
    SocketException(const ErrorCode& err);
    int error_code() const { return _errno.value();}

public:
    virtual const char* what() const noexcept;

private:
    ErrorCode _errno;
};

}
#endif
