#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#include "socket_global.h"
#include "socket_exception.h"

namespace comno
{

std::string host_name()
{
    char name[1024];
    if( gethostname(name, sizeof(name)-1) != 0 ){
        throw socket_exception(error_code(errno));
        return "";
    }

    return std::string(name);
}

void close(const socket_t sock_fd)
{
    ::close(sock_fd);
}

void set_block(const socket_t sock_fd, bool blocked)
{
    ::ioctl(sock_fd, FIONBIO, blocked ? 1 : 0);
}

int send(const socket_t sock_fd, const char* buffer, int size)
{
    int res = ::send(sock_fd, buffer, size, MSG_NOSIGNAL);
    if (res == -1 )
        throw socket_exception(error_code(errno));

    //LOG_DEBUG(main_logger, "size:%d.", res);
    return res;
}

int send(const socket_t sock_fd, const std::string& buffer)
{
    return send(sock_fd, buffer.c_str(), buffer.size());
}
}
