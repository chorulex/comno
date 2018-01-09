#ifndef _COMNO_BASIC_SOCKET_H_
#define _COMNO_BASIC_SOCKET_H_

#include <string>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "comno/utility/noncopyable.h"
#include "comno/option/socket_option_ops.h"
#include "comno/impl/ip/basic_endpoint.h"
#include "comno/impl/domain/basic_endpoint.h"

#include "comno/impl/exception/throw_exception.hpp"
#include "socket_base.h"

namespace comno
{

template <typename protocol_t>
class basic_socket :
        public comno::utility::noncopyable,
        public socket_base
{
    using endpoint_type = typename protocol_t::endpoint;

public:
    using reuse_address = option::boolean_t<SOL_SOCKET, SO_REUSEADDR>;

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

        _sock_fd = ::socket(protocol.family(), protocol.type(), protocol.protocol());
        if( _sock_fd.illegal() ){
            throw_exception(errno);
        }
    }

    /**
     * close socket file discriptor, and release fd resource to system.
     */
    void close()
    {
        if( is_open() ){
            ::close(_sock_fd);
            _sock_fd.reset();
        }
    }

    void connect(const endpoint_type& ep)
    {
        if( !is_open() ){
            open();
        }

        if (::connect(_sock_fd, ep.data(), ep.size()) == -1 ) {
            close();
            throw_exception(errno);
        }

        _remote_ep = ep;
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
        system::error_code ec;
        option::setsockopt(_sock_fd, opt, ec);
        if( ec != 0 )
            throw_exception(ec);
    }
    template<typename option_t>
    void get_option(option_t& opt)
    {
        system::error_code ec;
        option::getsockopt(_sock_fd, opt, ec);
        if( ec != 0 )
            throw_exception(ec);
    }

    std::size_t send(const std::string& buffer)
    {
        return this->send(buffer.c_str(), buffer.size());
    }
    std::size_t send(const char* buffer)
    {
        return this->send(buffer, strlen(buffer));
    }

    std::size_t send(const char* buffer, int size)
    {
        int ret = ::send(_sock_fd, buffer, size, MSG_NOSIGNAL);
        if( ret <= 0 )
            throw_exception(errno);

        return ret;
    }

    std::size_t receive(char* buffer, std::size_t max_size)
    {
        int ret = ::recv(_sock_fd, buffer, max_size, 0);
        if( ret <= 0 )
            throw_exception(errno);

        return ret;
    }

    int native_handle() const 
    {
        return _sock_fd;
    }

protected:
    void set_block(bool blocked)
    {
        ::ioctl(_sock_fd, FIONBIO, blocked ? 0 : 1);
    }

protected:
    socket_t _sock_fd;

    endpoint_type _local_ep;
    endpoint_type _remote_ep;
};

}
#endif
