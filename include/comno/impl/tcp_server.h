#ifndef _COMNO_TCP_SERVER_SOCKET_H_
#define _COMNO_TCP_SERVER_SOCKET_H_

#include <string>

#include "end_point.h"
#include "tcp_socket.h"
#include "tcp_client.h"
#include "socket_exception.h"

namespace comno
{
/**
 * AF_INET | SOCK_STREAM | IPPROTO_TCP
 */
class tcp_server : public tcp_socket
{
public:
    tcp_server();
    tcp_server(const socket_t  sock_fd);
    ~tcp_server();

public:
    bool listen(const unsigned int port);
    tcp_client accept();

    unsigned int listen_port() const { return _listen_port;}

private:
    void bind(const unsigned int port);

private:
    unsigned int _listen_port{0};
};

};
#endif

