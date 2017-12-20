#include <cstddef>
#include <unistd.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "tcp_domain_server.h"

namespace comno
{
tcp_domain_server::tcp_domain_server()
{
    _sock_fd = create_fd();
}
tcp_domain_server::tcp_domain_server(const SocketFd sock_fd, const std::string& path)
{
    _sock_fd = sock_fd;
    _domain_file = path;
}
tcp_domain_server::~tcp_domain_server()
{
    close();
}

int tcp_domain_server::create_fd()
{
    return ::socket(AF_UNIX, SOCK_STREAM, 0);
}

void tcp_domain_server::close()
{
    if( _sock_fd != -1 ){
        ::close(_sock_fd);

        if( !_domain_file.empty() )
            unlink(_domain_file.c_str());
    }
}

bool tcp_domain_server::listen(const std::string& path)
{
    if (_sock_fd < 0) 
        return false;

    this->bind(path);

    int res = ::listen(_sock_fd, 10);
    if (res < 0) 
        throw socket_exception(error_code(errno));

    _domain_file = path;
    return true;
}

void tcp_domain_server::bind(const std::string& path)
{
    struct sockaddr_un serverAddr;
    if(path.size() >= sizeof(serverAddr.sun_path))
        throw socket_exception(error_code(ENAVAIL));

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sun_family = AF_UNIX;
    memcpy(serverAddr.sun_path, path.c_str(), path.size());
    unlink(path.c_str());

    std::size_t len = offsetof(struct sockaddr_un, sun_path) + strlen(serverAddr.sun_path);
    int res = ::bind(_sock_fd, (struct sockaddr *)&serverAddr, len);
    if( res == -1 )
        throw socket_exception(error_code(errno));
}

tcp_domain_client tcp_domain_server::accept() 
{ 
    sockaddr_un clientAddr; 
    unsigned int cliLen = sizeof(clientAddr); 
 
    int clientSocket = ::accept(_sock_fd, (sockaddr*)&clientAddr, &cliLen); 
    if (clientSocket == -1 )
        throw socket_exception(error_code(errno));
    
    return tcp_domain_client(clientSocket, _domain_file);
}
}
