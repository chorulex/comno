#include <cstddef>
#include <unistd.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "tcp_domain_server.h"

namespace QtSocket
{
TCPDomainServer::TCPDomainServer()
{
    _sock_fd = CreateFD();
}
TCPDomainServer::TCPDomainServer(const SocketFd sock_fd, const std::string& path)
{
    _sock_fd = sock_fd;
    _domain_file = path;
}
TCPDomainServer::~TCPDomainServer()
{
    Close();
}

int TCPDomainServer::CreateFD()
{
    return ::socket(AF_UNIX, SOCK_STREAM, 0);
}

void TCPDomainServer::Close()
{
    if( _sock_fd != -1 ){
        ::close(_sock_fd);

        if( !_domain_file.empty() )
            unlink(_domain_file.c_str());
    }
}

bool TCPDomainServer::Listen(const std::string& path)
{
    if (_sock_fd < 0) 
        return false;

    Bind(path);

    int res = ::listen(_sock_fd, 10);
    if (res < 0) 
        throw SocketException(errno);

    _domain_file = path;
    return true;
}

void TCPDomainServer::Bind(const std::string& path)
{
    struct sockaddr_un serverAddr;
    if(path.size() >= sizeof(serverAddr.sun_path))
        throw SocketException(ENAVAIL);

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sun_family = AF_UNIX;
    memcpy(serverAddr.sun_path, path.c_str(), path.size());
    unlink(path.c_str());

    std::size_t len = offsetof(struct sockaddr_un, sun_path) + strlen(serverAddr.sun_path);
    int res = ::bind(_sock_fd, (struct sockaddr *)&serverAddr, len);
    if( res == -1 )
        throw SocketException(errno);
}

TCPDomainClient TCPDomainServer::Accept() 
{ 
    sockaddr_un clientAddr; 
    unsigned int cliLen = sizeof(clientAddr); 
 
    int clientSocket = ::accept(_sock_fd, (sockaddr*)&clientAddr, &cliLen); 
    if (clientSocket == -1 )
        throw SocketException(errno);
    
    return TCPDomainClient(clientSocket, _domain_file);
}
}