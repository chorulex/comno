#ifndef _COMNO_SOCKET_T_H_
#define _COMNO_SOCKET_T_H_

#include "comno/impl/detail/socket_type.h"

namespace comno
{
class socket_t
{
public:
    socket_t() : _fd(comno::type::invalid_socket_value)
    {
    }
    socket_t(const socket_t& src) : _fd(src._fd)
    {
    }
    explicit socket_t(comno::type::socket_type fd) : _fd(fd)
    {
    }
    socket_t& operator= (const socket_t& src)
    {
        _fd = src._fd;
        return *this;
    }
    socket_t& operator= (const comno::type::socket_type& src)
    {
        _fd = src;
        return *this;
    }

public:
    operator comno::type::socket_type() const
    {
        return _fd;
    }

    void reset()
    {
        _fd = comno::type::invalid_socket_value;
    }

    bool illegal() const
    {
        return _fd == comno::type::invalid_socket_value;
    }

private:
    comno::type::socket_type _fd;
};

}

#endif
