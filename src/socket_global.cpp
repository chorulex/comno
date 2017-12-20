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

namespace QtSocket
{

void Close(const SocketFd sock_fd)
{
    ::close(sock_fd);
}

void SetBlock(const SocketFd sock_fd, bool blocked)
{
    ::ioctl(sock_fd, FIONBIO, blocked ? 1 : 0);
}

int Send(const SocketFd sock_fd, const char* buffer, int size)
{
    int res = ::send(sock_fd, buffer, size, MSG_NOSIGNAL);
    if (res == -1 )
        throw SocketException(ErrorCode(errno));

    //LOG_DEBUG(main_logger, "size:%d.", res);
    return res;
}

int Send(const SocketFd sock_fd, const std::string& buffer)
{
    return Send(sock_fd, buffer.c_str(), buffer.size());
}

void SetSockOpt(const SocketFd sock_fd, int level, int opt_name, const void* opt_val, socklen_t opt_len)
{
    if( -1 == ::setsockopt(sock_fd, level, opt_name, opt_val, opt_len) )
        throw SocketException(ErrorCode(errno));
}

void GetSockOpt(const SocketFd sock_fd, int level, int opt_name, void* opt_val, socklen_t* opt_len)
{
    if( -1 == ::getsockopt(sock_fd, level, opt_name, opt_val, opt_len) )
        throw SocketException(ErrorCode(errno));
}
}