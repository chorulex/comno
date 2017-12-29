#ifndef _COMNO_SOCKET_BASE_H_
#define _COMNO_SOCKET_BASE_H_

#include <string>

#include "comno/option/socket_option_ops.h"
#include "socket_exception.h"
#include "socket_t.h"
#include "socket_global.h"

namespace comno
{
class socket_base
{
protected:
    socket_base() = default;

public:
    using reuse_address = option::boolean_t<SOL_SOCKET, SO_REUSEADDR>;

    virtual ~socket_base() = default;

public:
    virtual void close();
    socket_t fd() const { return _sock_fd;}

    void block(bool val);

    template<typename option_t>
    void set_option(const option_t& opt)
    {
        system::error_code ec;
        option::setsockopt(_sock_fd, opt, ec);
        if( ec != 0 )
            throw socket_exception(ec);
    }
    template<typename option_t>
    void get_option(option_t& opt)
    {
        system::error_code ec;
        option::getsockopt(_sock_fd, opt, ec);
        if( ec != 0 )
            throw socket_exception(ec);
    }

protected:
    virtual int create_fd() = 0;

protected:
    socket_t _sock_fd;
};

}
#endif
