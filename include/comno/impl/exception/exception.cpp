#include <string>

#include "exception.h"

namespace comno
{

exception::exception(const system::error_code& err) : _errno(err.value())
{
}

const char* exception::what() const noexcept
{
    std::string err("socket error: ");
    err.append(_errno.to_string());
    err.append(" (");
    err.append(_errno.details());
    err.append(")");

    return err.c_str();
}

}