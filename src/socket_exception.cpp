#include <string>

#include "socket_exception.h"

namespace QtSocket
{

SocketException::SocketException(const ErrorCode& err) : _errno(err.value())
{
}

const char* SocketException::what() const noexcept
{
    std::string err("socket error: ");
    err.append(_errno.to_string());
    err.append(" (");
    err.append(_errno.details());
    err.append(")");

    return err.c_str();
}

}
