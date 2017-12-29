#include <unistd.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "tcp_server.h"

namespace comno
{
tcp_server::tcp_server()
{
    _sock_fd = tcp_socket::create_fd();
}
tcp_server::~tcp_server()
{
    if( _sock_fd != -1 )
        ::close(_sock_fd);
}

bool tcp_server::listen(const unsigned int port)
{
    if (_sock_fd < 0) 
        return false;

    bind(port);

    int res = ::listen(_sock_fd, 10);
    if (res < 0) {
        throw socket_exception(system::error_code(errno));
    }

    _listen_port = port;
    return true;
}

void tcp_server::bind(const unsigned int port)
{
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(port);

    int res = ::bind(_sock_fd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    if( res == -1 )
        throw socket_exception(system::error_code(errno));
}

tcp_client tcp_server::accept()
{
    struct sockaddr_in clientAddr;
    socklen_t cliLen = sizeof(clientAddr);

    int clientSocket = ::accept(_sock_fd, (struct sockaddr *)&clientAddr, &cliLen);
    if (clientSocket == -1 ) {
        throw socket_exception(system::error_code(errno));
    }

    end_point dest, src;
    dest.port = listen_port();
    src.ip = ::inet_ntoa(clientAddr.sin_addr);

    return tcp_client(clientSocket, src, dest);
}

}
