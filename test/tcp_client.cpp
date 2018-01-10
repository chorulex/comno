#include <cstring>
#include "tcp_client.h"

tcp_client::tcp_client()noexcept
{
    _socket.open();
}

tcp_client::tcp_client(tcp_client&& src) noexcept
{
    this->_socket = std::move(src._socket);
}

tcp_client& tcp_client::operator= (tcp_client&& src) noexcept
{
    this->_socket = std::move(src._socket);
    return *this;
}
tcp_client::tcp_client(comno::tcp::socket& sock_fd)noexcept
{
    _socket = std::move(sock_fd);
}

tcp_client::~tcp_client()noexcept
{
}

/**
 * Call Close() to close and reset fd before recall this to reconnect to other host.
 */
bool tcp_client::connect(const comno::tcp::endpoint& host, time_t timeout_sec)
{
    if ( !_socket.is_open() )
        _socket.open();

    /*
    // until to success or fail
    if( timeout_sec == 0 ){
        return _socket.connect(host);
    }else{
        _socket.block(false);
        try{
            _socket.connect(host);
        }catch(comno::socket_exception& ex){
            try{
                connect_timeout(timeout_sec);
            }catch(...){
                _socket.close();
                throw;
            }
        }

        _socket.block(true);
    }
    */

    _socket.connect(host);
    return true;
}

/**
 * return value:
 * false: connect successfully.
 * throw Etsocket_exception obj.
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
        _socket.close();
        throw socket_exception(system::error_code(errno));
    }

    system::error_code ec;
    option::integer_t<SOL_SOCKET, SO_ERROR> opt;
    option::getsockopt(_sock_fd, opt, ec);
    if( opt != 0 ){
        throw socket_exception(system::error_code(opt));
    }else {
        if( ec == EINPROGRESS )
            throw socket_exception(system::error_code(ETIMEDOUT));
        else
            throw socket_exception(ec);
    }

    return false;
}
*/

void tcp_client::set_recv_timeout(time_t timeout)//seconds
{
    _socket.set_option(comno::tcp::socket::receive_timeout(timeout));
}

time_t tcp_client::get_recv_timeout() //seconds
{
    comno::tcp::socket::receive_timeout opt;
    _socket.get_option(opt);
    return opt.value().tv_sec;
}

void tcp_client::set_send_timeout(time_t timeout)//seconds
{
    _socket.set_option(comno::tcp::socket::send_timeout(timeout));
}

time_t tcp_client::get_send_timeout() //seconds
{
    comno::tcp::socket::send_timeout opt;
    _socket.get_option(opt);
    return opt.value().tv_sec;
}