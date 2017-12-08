#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <memory>

#include "socket.h"
#include "socket_exception.h"

namespace QtSocket
{
void Socket::SetSockOpt(int level, int opt_name, const unsigned char opt_val)
{
    QtSocket::SetSockOpt(_sock_fd, level, opt_name, (const void*)(&opt_val), sizeof(unsigned char));
}

void Socket::SetSockOpt(int level, int opt_name, const int opt_val)
{
    QtSocket::SetSockOpt(_sock_fd, level, opt_name, (const void*)(&opt_val), sizeof(int));
}

void Socket::SetSockOpt(int level, int opt_name, const unsigned int opt_val)
{
    QtSocket::SetSockOpt(_sock_fd, level, opt_name, (const void*)(&opt_val), sizeof(unsigned int));
}

void Socket::SetSockOpt(int level, int opt_name, const struct timeval &opt_val)
{
    QtSocket::SetSockOpt(_sock_fd, level, opt_name, (const void*)(&opt_val), sizeof(struct timeval));
}

void Socket::GetSockOpt(int level, int opt_name, unsigned char& opt_val)
{
    socklen_t len = sizeof(unsigned char);
    QtSocket::GetSockOpt(_sock_fd, level, opt_name, (void*)(&opt_val), &len);
}

void Socket::GetSockOpt(int level, int opt_name, int& opt_val)
{
    opt_val = 0;
    socklen_t len = sizeof(opt_val);
    QtSocket::GetSockOpt(_sock_fd, level, opt_name, (void*)(&opt_val), &len);
}

void Socket::GetSockOpt(int level, int opt_name, unsigned int &opt_val)
{
    socklen_t len = sizeof(unsigned int);
    QtSocket::GetSockOpt(_sock_fd, level, opt_name, (void*)(&opt_val), &len);
}

void Socket::GetSockOpt(int level, int opt_name, struct timeval &opt_val)
{
    socklen_t len = sizeof(struct timeval);
    QtSocket::GetSockOpt(_sock_fd, level, opt_name, (void*)(&opt_val), &len);
}

void Socket::SetNoBlock()
{
    QtSocket::SetBlock(_sock_fd, true);
}

void Socket::SetBlock()
{
    QtSocket::SetBlock(_sock_fd, false);
}

void Socket::Close()
{
    if( _sock_fd != -1 ){
        QtSocket::Close(_sock_fd);
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
    SetSockOpt(SOL_SOCKET, SO_RCVTIMEO, time_out);
}

time_t TCPSocket::GetRecvTimeOut() //seconds
{
    struct timeval time_out = {0,0};
    GetSockOpt(SOL_SOCKET, SO_RCVTIMEO, time_out);
    return time_out.tv_sec;
}

void TCPSocket::SetSendTimeOut(time_t timeout)//seconds
{
    struct timeval time_out = {timeout,0};
    SetSockOpt(SOL_SOCKET, SO_SNDTIMEO, time_out);
}

time_t TCPSocket::GetSendTimeOut() //seconds
{
    struct timeval time_out = {0,0};
    GetSockOpt(SOL_SOCKET, SO_SNDTIMEO, time_out);
    return time_out.tv_sec;
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
    return QtSocket::Send(_sock_fd, buffer);
}

int TCPSocket::Send(const char* buffer, int size)
{
    int res = QtSocket::Send(_sock_fd, buffer, size);
    if (res == -1 )
        throw SocketException(errno);

    //LOG_DEBUG(main_logger, "size:%d.", res);
    return res;
}

}
