#ifndef _COMNO_TCP_CLIENT_SOCKET_H_
#define _COMNO_TCP_CLIENT_SOCKET_H_

#include "comno/socket.h"

class tcp_client
{
    tcp_client(const tcp_client&) = default;
    tcp_client& operator= (const tcp_client&) = default;

public:
    tcp_client() noexcept;
    explicit tcp_client(comno::tcp::socket& sock_fd) noexcept;
    tcp_client(tcp_client&&) noexcept;
    tcp_client& operator= (tcp_client&&) noexcept;

    /**
     * not close socket fd on this calling.
     * close fd manually.
     */
    ~tcp_client() noexcept;

public:
    bool connect(const comno::tcp::endpoint& host, time_t timeout_sec = 0);

    void   set_recv_timeout(time_t timeout); //seconds
    time_t get_recv_timeout(); //seconds

    void   set_send_timeout(time_t timeout); //seconds
    time_t get_send_timeout(); //seconds

    comno::tcp::endpoint local_endpoint() const
    {
        return _socket.local_endpoint();
    }
    comno::tcp::endpoint remote_endpoint() const
    {
        return _socket.remote_endpoint();
    }

private:
    bool connect_timeout(time_t timeout_sec);

private:
    comno::tcp::socket _socket;
};

#endif
