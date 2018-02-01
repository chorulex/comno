#ifndef _COMNO_HOST_NAME_H_
#define _COMNO_HOST_NAME_H_

#include "comno/impl/detail/socket_ops.h"
#include "comno/impl/exception/throw_exception.hpp"

namespace comno
{

std::string host_name()
{
    comno::system::error_code ec;
    const std::string& rs = comno::detail::socket_ops::gethostname(ec);
    comno::throw_exception(ec);

    return rs;
}

}
#endif