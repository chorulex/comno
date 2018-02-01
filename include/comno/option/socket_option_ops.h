#ifndef _COMNO_SOCK_OPTION_OPS_H_
#define _COMNO_SOCK_OPTION_OPS_H_

#include "comno/system/error_code.h"
#include "comno/impl/detail/socket_ops.h"
#include "socket_option.h"

namespace comno
{
namespace option
{

template<typename OptionObj>
inline void getsockopt(const socket_t& sock_fd, OptionObj& opt, system::error_code& ec)
{
    socklen_t len = opt.size();
    comno::detail::socket_ops::getsockopt(sock_fd, opt.level(), opt.name(), static_cast<void*>(opt.data()), &len, ec);
    opt.resize(len);
}

template<typename OptionProxy>
inline void setsockopt(const socket_t& sock_fd, const OptionProxy& opt, system::error_code& ec)
{
    comno::detail::socket_ops::setsockopt(sock_fd, opt.level(), opt.name(), static_cast<const void*>(opt.data()), opt.size(),ec);
}
}
}
#endif