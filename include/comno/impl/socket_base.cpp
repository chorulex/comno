#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <memory>

#include "socket_base.h"
#include "socket_exception.h"

namespace comno
{
void socket_base::set_no_block()
{
    comno::set_block(_sock_fd, true);
}

void socket_base::set_block()
{
    comno::set_block(_sock_fd, false);
}

void socket_base::close()
{
    if( _sock_fd != -1 ){
        comno::close(_sock_fd);
        _sock_fd = -1;
    }
}

int tcp_socket::create_fd()
{
    return ::socket(AF_INET, SOCK_STREAM, 0);
}

void tcp_socket::set_recv_timeout(time_t timeout)//seconds
{
    set_option(option::timeval_t<SOL_SOCKET, SO_RCVTIMEO>(timeout));
}

time_t tcp_socket::get_recv_timeout() //seconds
{
    option::timeval_t<SOL_SOCKET, SO_RCVTIMEO> opt;
    get_option(opt);
    return opt.value().tv_sec;
}

void tcp_socket::set_send_timeout(time_t timeout)//seconds
{
    set_option(option::timeval_t<SOL_SOCKET, SO_SNDTIMEO>(timeout));
}

time_t tcp_socket::get_send_timeout() //seconds
{
    option::timeval_t<SOL_SOCKET, SO_SNDTIMEO> opt;
    get_option(opt);
    return opt.value().tv_sec;
}

int tcp_socket::recv(char* buffer, int max_len)
{
    int res = ::recv(_sock_fd, buffer, max_len, 0);
    if (res == -1 ) 
        throw socket_exception(error_code(errno));

    //LOG_DEBUG(main_logger, "size:%d.", res);
    return res;
}

int tcp_socket::send(const std::string& buffer)
{
    return comno::send(_sock_fd, buffer);
}

int tcp_socket::send(const char* buffer, int size)
{
    int res = comno::send(_sock_fd, buffer, size);
    if (res == -1 )
        throw socket_exception(error_code(errno));

    //LOG_DEBUG(main_logger, "size:%d.", res);
    return res;
}

}
