#include <string>

#include "socket_exception.h"

namespace comno
{

socket_exception::socket_exception(const comno::error_code& err) : _errno(err.value())
{
}

const char* socket_exception::what() const noexcept
{
    std::string err("socket error: ");
    err.append(_errno.to_string());
    err.append(" (");
    err.append(_errno.details());
    err.append(")");

    return err.c_str();
}

}
