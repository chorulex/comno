#ifndef _COMNO_BASIC_STREAM_SOCKET_H_
#define _COMNO_BASIC_STREAM_SOCKET_H_

#include "comno/utility/noncopyable.h"
#include "comno/impl/ip/basic_endpoint.h"
#include "comno/impl/basic_socket.h"

namespace comno
{

template <typename protocol_t>
class basic_stream_socket
    : public comno::utility::noncopyable
    , public basic_socket<protocol_t>
{
    using endpoint_type = typename protocol_t::endpoint;

public:
    basic_stream_socket()
    {
    }
    basic_stream_socket(const socket_t& sock, const endpoint_type& local, const endpoint_type& remote)
        :basic_socket<protocol_t>(sock, local, remote)
    {
    }

    basic_stream_socket(basic_stream_socket&& other)
    {
        this->_sock_fd = other._sock_fd;
        this->_local_ep = other._local_ep;
        this->_remote_ep = other._remote_ep;

       other._sock_fd.reset(); 
    }

    basic_stream_socket& operator= (basic_stream_socket&& other)
    {
        this->_sock_fd = other._sock_fd;
        this->_local_ep = other._local_ep;
        this->_remote_ep = other._remote_ep;

       other._sock_fd.reset(); 
       return *this;
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
        comno::system::error_code ec;
        std::size_t ret = comno::detail::socket_ops::send(this->_sock_fd, buffer, size, MSG_NOSIGNAL, ec);
        throw_exception(ec);

        return ret;
    }

    std::size_t receive(char* buffer, std::size_t max_size)
    {
        comno::system::error_code ec;
        std::size_t ret = comno::detail::socket_ops::recv(this->_sock_fd, buffer, max_size, MSG_NOSIGNAL, ec);
        throw_exception(ec);

        return ret;
    }

};

}
#endif
