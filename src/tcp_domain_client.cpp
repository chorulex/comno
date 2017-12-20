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

namespace comno
{
tcp_domain_client::tcp_domain_client() noexcept
{
    _sock_fd = create_fd();
}
tcp_domain_client::tcp_domain_client(const socket_t sock_fd, const std::string& domain_file) noexcept
{
    _sock_fd = sock_fd;
    _domain_file = domain_file;
}

tcp_domain_client::~tcp_domain_client() noexcept
{
}

int tcp_domain_client::create_fd()
{
    return ::socket(AF_UNIX, SOCK_STREAM, 0);
}

bool tcp_domain_client::connect(const std::string& domain_file)
{
    if ( _sock_fd == -1 )
        return false;

    struct sockaddr_un serverAddr;
    if(domain_file.size() >= sizeof(serverAddr.sun_path))
        throw socket_exception(error_code(ENAVAIL));

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sun_family = AF_UNIX;
    memcpy(serverAddr.sun_path, domain_file.c_str(), domain_file.size());

    std::size_t len = offsetof(struct sockaddr_un, sun_path) + strlen(serverAddr.sun_path);
    int res = ::connect(_sock_fd, (struct sockaddr *)&serverAddr, len);
    if( res == -1 )
        throw socket_exception(error_code(errno));

    _domain_file = domain_file;
    return true;
}
}
