#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#include "socket.h"
#include "socket_exception.h"

namespace QtSocket
{
void Socket::SetSockOpt(int level, int opt_name, const void* opt_val, socklen_t opt_len)
{
    if( -1 == ::setsockopt(_sock_fd, level, opt_name, opt_val, opt_len) )
        throw SocketException(errno);
}

void Socket::GetSockOpt(int level, int opt_name, void* opt_val, socklen_t* opt_len)
{
    if( -1 == ::getsockopt(_sock_fd, level, opt_name, opt_val, opt_len) )
        throw SocketException(errno);
}

bool Socket::SetNoBlock()
{
    int flags = ::fcntl(_sock_fd, F_GETFL, 0);
    if( flags < 0 )
        return false;
        
    return ::fcntl(_sock_fd, F_SETFL, O_NONBLOCK) != -1;
}

bool Socket::SetBlock()
{
    int flags = ::fcntl(_sock_fd, F_GETFL, 0);
    if( flags < 0 )
        return false;
        
    return ::fcntl(_sock_fd, F_SETFL, O_NONBLOCK) != -1;
}

void Socket::Close()
{
    if( _sock_fd != -1 ){
        ::close(_sock_fd);
        _sock_fd = -1;
    }
}

int TCPSocket::CreateFD()
{
    return ::socket(AF_INET, SOCK_STREAM, 0);
}

void TCPSocket::SetRecvTimeOut(time_t timeout)//seconds
{
    struct timeval time_out = {timeout,0};
    SetSockOpt(SOL_SOCKET, SO_RCVTIMEO, (char*)&time_out, sizeof(struct timeval));
}

void TCPSocket::SetSendTimeOut(time_t timeout)//seconds
{
    struct timeval time_out = {timeout,0};
    SetSockOpt(SOL_SOCKET, SO_SNDTIMEO, (char*)&time_out, sizeof(struct timeval));
}

int TCPSocket::Recv(char* buffer, int max_len)
{
    int res = ::recv(_sock_fd, buffer, max_len, 0);
    if (res == -1 ) 
        throw SocketException(errno);

    //LOG_DEBUG(main_logger, "size:%d.", res);
    return res;
}

int TCPSocket::Send(const std::string& buffer)
{
    return Send(buffer.c_str(), buffer.size());
}

int TCPSocket::Send(const char* buffer, int size)
{
    int res = send(_sock_fd, buffer, size, MSG_NOSIGNAL);
    if (res == -1 )
        throw SocketException(errno);

    //LOG_DEBUG(main_logger, "size:%d.", res);
    return res;
}


}
