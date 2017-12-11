#ifndef _QT_TCP_SERVER_SOCKET_H_
#define _QT_TCP_SERVER_SOCKET_H_

#include <string>

#include "end_point.h"
#include "socket.h"
#include "tcp_client.h"
#include "socket_exception.h"

namespace QtSocket
{
/**
 * AF_INET | SOCK_STREAM | IPPROTO_TCP
 */
class TCPServer : public TCPSocket
{
public:
    TCPServer();
    TCPServer(const SocketFd sock_fd);
    ~TCPServer();

public:
    void SetReuseAddr(bool use);
    bool Listen(const unsigned int port);
    TCPClient Accept();

    unsigned int ListenPort() const { return _listen_port;}

private:
    void Bind(const unsigned int port);

private:
    unsigned int _listen_port{0};
};

};
#endif

