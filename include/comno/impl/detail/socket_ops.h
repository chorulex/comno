#ifndef _COMNO_SOCKET_OPS_H_
#define _COMNO_SOCKET_OPS_H_

#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#include <string>

#include "comno/impl/exception/throw_exception.hpp"
#include "comno/impl/detail/socket_type.h"

namespace comno
{

namespace detail
{

namespace socket_ops
{

#define CALL_WRAPPER(call_expr) \
    ec = comno::system::error_code(); \
    int ret = call_expr; \
    if( ret == -1 ) \
        ec = errno; 

std::string gethostname(comno::system::error_code& ec)
{
    char name[1024];
    CALL_WRAPPER(::gethostname(name, sizeof(name)-1))

    return std::string(name);
}

comno::type::socket_type socket(int family, int type, int protocol,
    comno::system::error_code& ec)
{
    ec = comno::system::error_code();

    comno::type::socket_type fd = ::socket(family, type, protocol);
    if( fd == comno::type::invalid_socket_value )
        ec = errno;

    return fd;
}

void close(comno::type::socket_type fd,
    comno::system::error_code& ec)
{
    CALL_WRAPPER(::close(fd))
}

void connect(comno::type::socket_type fd, 
    const comno::type::sockaddr_base* addr_ptr,
    comno::type::socket_len_t addr_len,
    comno::system::error_code& ec)
{
    CALL_WRAPPER(::connect(fd, addr_ptr, addr_len))
}

void bind(comno::type::socket_type fd, 
    const comno::type::sockaddr_base* addr_ptr,
    comno::type::socket_len_t addr_len,
    comno::system::error_code& ec)
{
    CALL_WRAPPER(::bind(fd, addr_ptr, addr_len))
}

comno::type::socket_type accept(comno::type::socket_type fd, 
    comno::type::sockaddr_base* addr_ptr,
    comno::type::socket_len_t* addr_len,
    comno::system::error_code& ec)
{
    ec = comno::system::error_code();

    comno::type::socket_type accept_fd = ::accept(fd, addr_ptr, addr_len);
    if (accept_fd == comno::type::invalid_socket_value )
        ec = errno;

    return accept_fd;
}

void listen(comno::type::socket_type fd, int backlog, comno::system::error_code& ec)
{
    CALL_WRAPPER(::listen(fd, backlog))
}

void ioctl(comno::type::socket_type fd,
    int request_flag,
    int request_val,
    comno::system::error_code& ec)
{
    CALL_WRAPPER(::ioctl(fd, request_flag, request_val))
}

void getsockname(comno::type::socket_type fd, 
    comno::type::sockaddr_base* addr_ptr,
    comno::type::socket_len_t* addr_len,
    comno::system::error_code& ec)
{
    CALL_WRAPPER(::getsockname(fd, addr_ptr, addr_len))
}

void getsockopt(comno::type::socket_type fd, 
    int level,
    int opt_name,
    void* opt_val,
    comno::type::socket_len_t* opt_len,
    comno::system::error_code& ec)
{
    CALL_WRAPPER(::getsockopt(fd, level, opt_name, opt_val, opt_len))
}

void setsockopt(comno::type::socket_type fd, 
    int level,
    int opt_name,
    const void* opt_val,
    comno::type::socket_len_t opt_len,
    comno::system::error_code& ec)
{
    CALL_WRAPPER(::setsockopt(fd, level, opt_name, opt_val, opt_len))
}

std::size_t send(comno::type::socket_type fd, 
    const void* buffer,
    std::size_t len,
    int flags,
    comno::system::error_code& ec)
{
    CALL_WRAPPER(::send(fd, buffer, len, flags))
    return ret == -1 ? 0 : ret;
}

std::size_t sendto(comno::type::socket_type fd, 
    const void* buffer,
    std::size_t len,
    int flags,
    const comno::type::sockaddr_base* addr_ptr,
    comno::type::socket_len_t addr_len,
    comno::system::error_code& ec)
{
    CALL_WRAPPER(::sendto(fd, buffer, len, flags, addr_ptr, addr_len))
    return ret == -1 ? 0 : ret;
}

std::size_t recv(comno::type::socket_type fd, 
    void* buffer,
    std::size_t max_len,
    int flags,
    comno::system::error_code& ec)
{
    CALL_WRAPPER(::recv(fd, buffer, max_len, flags))
    return ret == -1 ? 0 : ret;
}

std::size_t recvfrom(comno::type::socket_type fd, 
    void* buffer,
    std::size_t len,
    int flags,
    comno::type::sockaddr_base* addr_ptr,
    comno::type::socket_len_t* addr_len,
    comno::system::error_code& ec)
{
    CALL_WRAPPER(::recvfrom(fd, buffer, len, flags, addr_ptr, addr_len))
    return ret == -1 ? 0 : ret;
}

uint16_t host_to_network_short(uint16_t val)
{
    return ::htons(val);
}

uint32_t host_to_network_long(uint32_t val)
{
    return ::htonl(val);
}

uint16_t network_to_host_short(uint16_t val)
{
    return ::ntohs(val);
}

uint32_t network_to_host_long(uint32_t val)
{
    return ::ntohl(val);
}
} // end namespace socket_ops

} // end namespace detail

} // end namespace comno

#endif
