#include <cstring>

#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "tcp_client.h"
#include "socket_exception.h"

namespace comno
{
tcp_client::tcp_client()noexcept
{
    _sock_fd = tcp_socket::create_fd();
}
tcp_client::tcp_client(const SocketFd sock_fd)noexcept
{
    _sock_fd = sock_fd;
}

tcp_client::tcp_client(const SocketFd sock_fd, const end_point& src, const end_point& dest)noexcept
{
    _sock_fd = sock_fd;
    _src_end_point = src;
    _dest_end_point = dest;
}

tcp_client::~tcp_client()noexcept
{
}

/**
 * Call Close() to close and reset fd before recall this to reconnect to other host.
 */
bool tcp_client::connect(const end_point& host, time_t timeout_sec)
{
    if ( _sock_fd == -1 ){
        _sock_fd = tcp_socket::create_fd();
        if ( _sock_fd == -1 )
            throw socket_exception(error_code(errno));
    }

    sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port    = htons(host.port);
    if(inet_aton(host.ip.c_str(), &server_address.sin_addr) == 0){
        close();
        throw socket_exception(error_code(EINVAL));
    }

    // until to success or fail
    if( timeout_sec == 0 ){
        if (::connect(_sock_fd, (sockaddr*)&server_address, sizeof(server_address)) == -1) {
            close();
            throw socket_exception(error_code(errno));
            return false;
        }
    }else{
        set_no_block();
        int ret = ::connect(_sock_fd, (sockaddr*)&server_address, sizeof(server_address));
        if( ret == -1 ){
            try{
                connect_timeout(timeout_sec);
            }catch(...){
                close();
                throw;
            }
        }

        set_block();
    }

    _dest_end_point = host;
    return true;
}

/**
 * return value:
 * false: connect successfully.
 * throw Etsocket_exception obj.
 */
bool tcp_client::connect_timeout(time_t timeout_sec)
{
    struct timeval time_val;
    time_val.tv_sec = timeout_sec;
    time_val.tv_usec = 0;

    fd_set fset;
    FD_ZERO(&fset);
    FD_SET(_sock_fd, &fset);

    int res = select(_sock_fd+1, NULL, &fset, NULL, &time_val);
    if( res <= 0 ){
        close();
        throw socket_exception(error_code(errno));
    }

    int error = -1;
    get_sock_opt(SOL_SOCKET, SO_ERROR, error);
    if( error != 0 ){
        throw socket_exception(error_code(error));
    }else {
        if( errno == EINPROGRESS )
            throw socket_exception(error_code(ETIMEDOUT));
        else
            throw socket_exception(error_code(errno));
    }

    return false;
}
}
