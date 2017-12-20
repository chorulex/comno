#ifndef _COMNO_TCP_CLIENT_SOCKET_H_
#define _COMNO_TCP_CLIENT_SOCKET_H_

#include "end_point.h"
#include "socket.h"

namespace comno
{
class tcp_client : public tcp_socket
{
public:
    tcp_client() noexcept;
    tcp_client(const socket_t sock_fd)noexcept;
    tcp_client(const socket_t sock_fd, const end_point& src, const end_point& dest) noexcept;

    /**
     * not close socket fd on this calling.
     * close fd manually.
     */
    ~tcp_client() noexcept;

public:
    bool connect(const end_point& host, time_t timeout_sec = 0);

    end_point dest_end_point() const { return _dest_end_point;}
    end_point src_end_point() const { return _src_end_point;}

private:
    bool connect_timeout(time_t timeout_sec);

private:
    end_point _dest_end_point;
    end_point _src_end_point;
};

}

#endif
