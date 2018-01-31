#ifndef _COMNO_SOCKET_T_H_
#define _COMNO_SOCKET_T_H_

#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

namespace comno
{
class socket_t
{
    using socket_fd_type = int;

public:
    socket_t() : _fd(0)
    {
    }
    socket_t(const socket_t& src) : _fd(src._fd)
    {
    }
    explicit socket_t(socket_fd_type fd) : _fd(fd)
    {
    }
    socket_t& operator= (const socket_t& src)
    {
        _fd = src._fd;
        return *this;
    }
    socket_t& operator= (const socket_fd_type& src)
    {
        _fd = src;
        return *this;
    }

public:
    operator int() const
    {
        return _fd;
    }

    void reset()
    {
        _fd = 0;
    }

    bool illegal() const
    {
        return _fd <= 0;
    }

private:
    socket_fd_type _fd;
};

}

#endif
