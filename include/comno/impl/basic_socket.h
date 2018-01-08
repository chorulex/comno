#ifndef _COMNO_BASIC_SOCKET_H_
#define _COMNO_BASIC_SOCKET_H_

#include <string>

#include "comno/option/socket_option_ops.h"
#include "socket_exception.h"
#include "socket_global.h"
#include "ip/endpoint.h"
#include "socket_base.h"
#include "socket_t.h"

namespace comno
{

template <typename protocol_t>
class basic_socket : public socket_base
{
    using endpoint_type = typename protocol_t::endpoint;

public:
    using reuse_address = option::boolean_t<SOL_SOCKET, SO_REUSEADDR>;

    basic_socket()
    {
        open();
    }

    basic_socket(const socket_t& sock, const endpoint_type& local, const endpoint_type& remote)
        :_sock_fd(sock)
        ,_local_ep(local)
        ,_remote_ep(remote)
    {
        //shutdown();
        close();
    }

    ~basic_socket()
    {

    }

public:
    endpoint_type local_endpoint() const
    {
        return _local_ep;
    }
    void local_endpoint(const endpoint_type& ep)
    {
        _local_ep = ep;
    }
    endpoint_type remote_endpoint() const
    {
        return _remote_ep;
    }
    void remote_endpoint(const endpoint_type& ep)
    {
        _remote_ep = ep;
    }

    /**
     * create socket file discriptor.
     * throw comno::socket_exception, if fail.
     */
    void open(const protocol_t protocol = protocol_t())
    {
        if( !_sock_fd.illegal() )
            return;

        _sock_fd = ::socket(protocol.family(), protocol.type(), protocol.protocol());
        if( _sock_fd.illegal() ){
            throw socket_exception(system::error_code(errno));
        }
    }

    bool is_open() const
    {
        return !_sock_fd.illegal();
    }
    bool connect(const endpoint_type& ep)
    {
        if (::connect(_sock_fd, ep.data(), ep.size()) == -1 ) {
            close();
            throw socket_exception(system::error_code(errno));
        }

        _remote_ep = ep;
        return true;
    }

    // shutdown socket.
    void shutdown();

    /**
     * close socket file discriptor, and release fd resource to system.
     */
    void close()
    {
        if( is_open() )
            _sock_fd.reset();

        /*
        if( protocol_t().family() == AF_UNIX ){
            unlink(this->_local_ep.path());
        }
        */
    }

    // set socket block or not.
    void block(bool val)
    {
        comno::set_block(_sock_fd, val);
    }

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

    int fd() const 
    {
        return _sock_fd;
    }
protected:

protected:
    socket_t _sock_fd;

    endpoint_type _local_ep;
    endpoint_type _remote_ep;
};
}
#endif
