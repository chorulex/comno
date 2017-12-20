#ifndef _QT_SOCKET_EXCEPTION_H_
#define _QT_SOCKET_EXCEPTION_H_

#include <exception>
#include "error_code.h"

namespace comno
{

class socket_exception : public std::exception
{
public:
    socket_exception(const comno::error_code& err);
    int error_code() const { return _errno.value();}

public:
    virtual const char* what() const noexcept;

private:
    comno::error_code _errno;
};

}
#endif
