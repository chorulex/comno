#ifndef _COMNO_BASIC_SOCKET_H_
#define _COMNO_BASIC_SOCKET_H_

#include <string>

#include "comno/option/socket_option_ops.h"
#include "comno/impl/detail/socket_ops.h"
#include "comno/impl/ip/basic_endpoint.h"
#include "comno/impl/domain/basic_endpoint.h"

#include "socket_base.h"

namespace comno
{

template <typename protocol_t>
class basic_socket : public socket_base
{
    using endpoint_type = typename protocol_t::endpoint;

public:
    using reuse_address = comno::option::boolean_t<SOL_SOCKET, SO_REUSEADDR>;

    using receive_timeout = comno::option::timeval_t<SOL_SOCKET, SO_RCVTIMEO>;
    using send_timeout = comno::option::timeval_t<SOL_SOCKET, SO_SNDTIMEO>;

    using receive_buf_size = comno::option::integer_t<SOL_SOCKET, SO_RCVBUF>;
    using send_buf_size = comno::option::integer_t<SOL_SOCKET, SO_SNDBUF>;

    using linger = comno::option::linger_t<SOL_SOCKET, SO_LINGER>;

    /**
     * construct socket.
     * and create socket fd.
     */
    basic_socket()
    {
        open();
    }

    basic_socket(basic_socket&& other)
        :_sock_fd(other._sock_fd)
        ,_local_ep(other._local_ep)
        ,_remote_ep(other._remote_ep)
    {
       other._sock_fd.reset(); 
    }

    basic_socket& operator= (basic_socket&& other)
    {
        _sock_fd = other._sock_fd;
        _local_ep = other._local_ep;
        _remote_ep = other._remote_ep;

       other._sock_fd.reset(); 
       return *this;
    }

    basic_socket(const socket_t& sock, const endpoint_type& local, const endpoint_type& remote)
        :_sock_fd(sock)
        ,_local_ep(local)
        ,_remote_ep(remote)
    {
    }

protected:
    /**
     * destruct socket.
     * and close socket fd.
     */
    ~basic_socket()
    {
        //shutdown();
        close();
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

    bool is_open() const
    {
        return !_sock_fd.illegal();
    }

    /**
     * create socket file discriptor.
     * throw comno::exception, if fail.
     */
    void open(const protocol_t protocol = protocol_t())
    {
        if( !_sock_fd.illegal() )
            return;

        comno::system::error_code ec;
        _sock_fd = comno::detail::socket_ops::socket(protocol.family(), protocol.type(), protocol.protocol(), ec);
        throw_exception(ec);
    }

    /**
     * close socket file discriptor, and release fd resource to system.
     */
    void close()
    {
        if( is_open() ){
            comno::system::error_code ec;
            comno::detail::socket_ops::close(_sock_fd, ec);
            throw_exception(ec);

            _sock_fd.reset();
        }
    }

    void connect(const endpoint_type& ep)
    {
        if( !is_open() ){
            open();
        }

        comno::system::error_code ec;
        comno::detail::socket_ops::connect(_sock_fd, ep.data(), ep.size(), ec);
        if ( ec != 0 ) {
            close();
            throw_exception(ec);
        }else{
            _remote_ep = ep;
            resolver_local_endpoint();
        }
    }

    void bind(const endpoint_type& ep)
    {
        comno::system::error_code ec;
        comno::detail::socket_ops::bind(_sock_fd, ep.data(), ep.size(), ec);
        comno::throw_exception(ec);

        local_endpoint(ep);
    }

    // shutdown socket.
    void shutdown();

    // set socket block or not.
    void block(bool val)
    {
        this->set_block(_sock_fd, val);
    }

    template<typename option_t>
    void set_option(const option_t& opt)
    {
        comno::system::error_code ec;
        comno::option::setsockopt(_sock_fd, opt, ec);
        if( ec != 0 )
            throw_exception(ec);
    }
    template<typename option_t>
    void get_option(option_t& opt)
    {
        comno::system::error_code ec;
        comno::option::getsockopt(_sock_fd, opt, ec);
        if( ec != 0 )
            throw_exception(ec);
    }

    int native_handle() const 
    {
        return _sock_fd;
    }

protected:
    void set_block(bool blocked)
    {
        comno::system::error_code ec;
        comno::detail::socket_ops::ioctl(_sock_fd, FIONBIO, blocked ? 0 : 1, ec);
        comno::throw_exception(ec);
    }

private:
    void resolver_local_endpoint()
    {
        socklen_t len = _local_ep.size();
        comno::system::error_code ec;

        comno::detail::socket_ops::getsockname(_sock_fd,
            const_cast<comno::type::sockaddr_base*>(this->_local_ep.data()),
            &len,
            ec);
        comno::throw_exception(ec);
    }


protected:
    socket_t _sock_fd;

    endpoint_type _local_ep;
    endpoint_type _remote_ep;
};

}
#endif
