#ifndef _COMNO_SOCKET_EXCEPTION_H_
#define _COMNO_SOCKET_EXCEPTION_H_

#include <exception>
#include "comno/system/error_code.h"

namespace comno
{

class socket_exception : public std::exception
{
public:
    socket_exception(const comno::system::error_code& err);
    int error_code() const { return _errno.value();}

public:
    virtual const char* what() const noexcept;

private:
    system::error_code _errno;
};

}
#endif
