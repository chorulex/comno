#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <memory>

#include "socket.h"
#include "socket_exception.h"

namespace comno
{
void socket::set_sock_opt(int level, int opt_name, const unsigned char opt_val)
{
    comno::set_sock_opt(_sock_fd, level, opt_name, (const void*)(&opt_val), sizeof(unsigned char));
}

void socket::set_sock_opt(int level, int opt_name, const int opt_val)
{
    comno::set_sock_opt(_sock_fd, level, opt_name, (const void*)(&opt_val), sizeof(int));
}

void socket::set_sock_opt(int level, int opt_name, const unsigned int opt_val)
{
    comno::set_sock_opt(_sock_fd, level, opt_name, (const void*)(&opt_val), sizeof(unsigned int));
}

void socket::set_sock_opt(int level, int opt_name, const struct timeval &opt_val)
{
    comno::set_sock_opt(_sock_fd, level, opt_name, (const void*)(&opt_val), sizeof(struct timeval));
}

void socket::get_sock_opt(int level, int opt_name, unsigned char& opt_val)
{
    socklen_t len = sizeof(unsigned char);
    comno::get_sock_opt(_sock_fd, level, opt_name, (void*)(&opt_val), &len);
}

void socket::get_sock_opt(int level, int opt_name, int& opt_val)
{
    opt_val = 0;
    socklen_t len = sizeof(opt_val);
    comno::get_sock_opt(_sock_fd, level, opt_name, (void*)(&opt_val), &len);
}

void socket::get_sock_opt(int level, int opt_name, unsigned int &opt_val)
{
    socklen_t len = sizeof(unsigned int);
    comno::get_sock_opt(_sock_fd, level, opt_name, (void*)(&opt_val), &len);
}

void socket::get_sock_opt(int level, int opt_name, struct timeval &opt_val)
{
    socklen_t len = sizeof(struct timeval);
    comno::get_sock_opt(_sock_fd, level, opt_name, (void*)(&opt_val), &len);
}

void socket::set_no_block()
{
    comno::set_block(_sock_fd, true);
}

void socket::set_block()
{
    comno::set_block(_sock_fd, false);
}

void socket::close()
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
    struct timeval time_out = {timeout,0};
    set_sock_opt(SOL_SOCKET, SO_RCVTIMEO, time_out);
}

time_t tcp_socket::get_recv_timeout() //seconds
{
    struct timeval time_out = {0,0};
    get_sock_opt(SOL_SOCKET, SO_RCVTIMEO, time_out);
    return time_out.tv_sec;
}

void tcp_socket::set_send_timeout(time_t timeout)//seconds
{
    struct timeval time_out = {timeout,0};
    set_sock_opt(SOL_SOCKET, SO_SNDTIMEO, time_out);
}

time_t tcp_socket::get_send_timeout() //seconds
{
    struct timeval time_out = {0,0};
    get_sock_opt(SOL_SOCKET, SO_SNDTIMEO, time_out);
    return time_out.tv_sec;
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
