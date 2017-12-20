#include <cstring>
#include <cstddef>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <sys/socket.h>

#include "tcp_domain_client.h"
#include "socket_exception.h"

namespace QtSocket
{
TCPDomainClient::TCPDomainClient() noexcept
{
    _sock_fd = CreateFD();
}
TCPDomainClient::TCPDomainClient(const SocketFd sock_fd, const std::string& domain_file) noexcept
{
    _sock_fd = sock_fd;
    _domain_file = domain_file;
}

TCPDomainClient::~TCPDomainClient() noexcept
{
}

int TCPDomainClient::CreateFD()
{
    return ::socket(AF_UNIX, SOCK_STREAM, 0);
}

bool TCPDomainClient::Connect(const std::string& domain_file)
{
    if ( _sock_fd == -1 )
        return false;

    struct sockaddr_un serverAddr;
    if(domain_file.size() >= sizeof(serverAddr.sun_path))
        throw SocketException(ErrorCode(ENAVAIL));

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sun_family = AF_UNIX;
    memcpy(serverAddr.sun_path, domain_file.c_str(), domain_file.size());

    std::size_t len = offsetof(struct sockaddr_un, sun_path) + strlen(serverAddr.sun_path);
    int res = ::connect(_sock_fd, (struct sockaddr *)&serverAddr, len);
    if( res == -1 )
        throw SocketException(ErrorCode(errno));

    _domain_file = domain_file;
    return true;
}
}