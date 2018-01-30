#ifndef _COMNO_BASIC_DGRAM_SOCKET_H_
#define _COMNO_BASIC_DGRAM_SOCKET_H_

#include "comno/utility/noncopyable.h"
#include "comno/impl/ip/basic_endpoint.h"
#include "comno/impl/basic_socket.h"

namespace comno
{

template <typename protocol_t>
class basic_datagram_socket
    : public comno::utility::noncopyable
    , public basic_socket<protocol_t>
{
    using endpoint_type = typename protocol_t::endpoint;

public:
    basic_datagram_socket()
    {
    }
    basic_datagram_socket(const socket_t& sock, const endpoint_type& local, const endpoint_type& remote)
        :basic_socket<protocol_t>(sock, local, remote)
    {
    }

    basic_datagram_socket(basic_datagram_socket&& other)
    {
        this->_sock_fd = other._sock_fd;
        this->_local_ep = other._local_ep;
        this->_remote_ep = other._remote_ep;

       other._sock_fd.reset(); 
    }

    basic_datagram_socket& operator= (basic_datagram_socket&& other)
    {
        this->_sock_fd = other._sock_fd;
        this->_local_ep = other._local_ep;
        this->_remote_ep = other._remote_ep;

       other._sock_fd.reset(); 
       return *this;
    }

    std::size_t send_to(const endpoint_type& ep, const std::string& buffer)
    {
        int ret = ::sendto(this->_sock_fd,
                            buffer.c_str(),
                            buffer.size(),
                            MSG_NOSIGNAL,
                            ep.data(),
                            ep.size());
        if( ret <= 0 )
            throw_exception(errno);

        return ret;
    }

    std::size_t receive_from(endpoint_type& ep, char* buffer, std::size_t max_size)
    {
        socklen_t len = ep.size();
        int ret = ::recvfrom(this->_sock_fd,
                            buffer,
                            max_size,
                            MSG_NOSIGNAL,
                            ep.data(),
                            &len);
        if( ret <= 0 )
            throw_exception(errno);

        return ret;
    }
};

}
#endif
