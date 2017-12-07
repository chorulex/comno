#include <cstring>

#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "tcp_client.h"
#include "socket_exception.h"

namespace QtSocket
{

TCPClient::TCPClient()noexcept
{
    _sock_fd = TCPSocket::CreateFD();
}
TCPClient::TCPClient(const SocketFd sock_fd)noexcept
{
    _sock_fd = sock_fd;
}

TCPClient::TCPClient(const SocketFd sock_fd, const EndPoint& src, const EndPoint& dest)noexcept
{
    _sock_fd = sock_fd;
    _src_end_point = src;
    _dest_end_point = dest;
}

TCPClient::~TCPClient()noexcept
{
    try{
        if( _sock_fd != -1 )
            close(_sock_fd);
    }catch(...) {
        exit(-1);
    }
}

bool TCPClient::Connect(const EndPoint& host, time_t timeout_sec)
{
    if ( _sock_fd == -1 )
        return false;

    sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port    = htons(host.port);
    if(inet_aton(host.ip.c_str(), &server_address.sin_addr) == 0)
        throw SocketException(EINVAL);

    // until to success or fail
    if( timeout_sec == 0 ){
        if (connect(_sock_fd, (sockaddr*)&server_address, sizeof(server_address)) == -1) {
            throw SocketException(errno);
            return false;
        }
    }else{
        SetNoBlock();
        int ret = connect(_sock_fd, (sockaddr*)&server_address, sizeof(server_address));
        if( ret == -1 ){
            if( ConnectTimeout(timeout_sec) )
                return false;
        }

        SetBlock();
    }

    _dest_end_point = host;
    return true;
}

/**
 * return value:
 * false: connect successfully.
 * throw EtSocketException obj.
 */
bool TCPClient::ConnectTimeout(time_t timeout_sec)
{
    struct timeval time_val;
    time_val.tv_sec = timeout_sec;
    time_val.tv_usec = 0;

    fd_set fset;
    FD_ZERO(&fset);
    FD_SET(_sock_fd, &fset);

    int res = select(_sock_fd+1, NULL, &fset, NULL, &time_val);
    if( res <= 0 )
        throw SocketException(errno);

    int error = -1;
    size_t err_len = sizeof(error);
    getsockopt( _sock_fd, SOL_SOCKET, SO_ERROR, &error, (socklen_t*)&err_len);
    if( error != 0 ){
        throw SocketException(error);
    }else {
        if( errno == EINPROGRESS )
            throw SocketException(ETIMEDOUT);
        else
            throw SocketException(errno);
    }

    return false;
}
}
