#ifndef _QT_SOCKET_EXCEPTION_H_
#define _QT_SOCKET_EXCEPTION_H_

#include <exception>
#include <errno.h>

namespace QtSocket
{
class SocketException : public std::exception
{
public:
    SocketException(int err);
    int ErrorCode() const { return _errno;}

public:
    virtual const char* what() const noexcept;

private:
    int _errno;
};

}
#endif
