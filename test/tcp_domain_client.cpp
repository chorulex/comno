#include <cstring>

#include "tcp_domain_client.h"

tcp_domain_client::tcp_domain_client() noexcept
{
}

tcp_domain_client::tcp_domain_client(tcp_domain_client&& src) noexcept
{
    this->_socket = std::move(src._socket);
}

tcp_domain_client::tcp_domain_client(comno::domain::stream_protocol::socket&& sock_fd) noexcept
{
    this->_socket = std::move(sock_fd);
}

tcp_domain_client::~tcp_domain_client() noexcept
{
}

bool tcp_domain_client::connect(const std::string& domain_file)
{
    try {
        _socket.connect(comno::domain::stream_protocol::endpoint(domain_file));
        return true;
    }
    catch( comno::socket_exception& ex){
        return false;
    }
}