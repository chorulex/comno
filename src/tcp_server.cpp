#include <unistd.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "tcp_server.h"

namespace QtSocket
{
TCPServer::TCPServer()
{
    _sock_fd = TCPSocket::CreateFD();
}
TCPServer::~TCPServer()
{
    if( _sock_fd != -1 )
        ::close(_sock_fd);
}

bool TCPServer::Listen(const unsigned int port)
{
    if (_sock_fd < 0) 
        return false;

    Bind(port);

    int res = listen(_sock_fd, 10);
    if (res < 0) {
        throw SocketException(errno);
    }

    _listen_port = port;
    return true;
}

void TCPServer::Bind(const unsigned int port)
{
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(port);

    int res = ::bind(_sock_fd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    if( res == -1 )
        throw SocketException(errno);
}

void TCPServer::SetReuseAddr(bool use)
{
    SetSockOpt(SOL_SOCKET, SO_REUSEADDR, use ? 1 : 0);
}

TCPClient TCPServer::Accept()
{
    struct sockaddr_in clientAddr;
    socklen_t cliLen = sizeof(clientAddr);

    int clientSocket = accept(_sock_fd, (struct sockaddr *)&clientAddr, &cliLen);
    if (clientSocket == -1 ) {
        throw SocketException(errno);
    }

    EndPoint dest, src;
    dest.port = ListenPort();
    src.ip = ::inet_ntoa(clientAddr.sin_addr);

    return TCPClient(clientSocket, src, dest);
}

}
