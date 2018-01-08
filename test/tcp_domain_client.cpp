#include <cstring>

#include "tcp_domain_client.h"

tcp_domain_client::tcp_domain_client() noexcept
{
}

tcp_domain_client::tcp_domain_client(tcp_domain_client&& src) noexcept
{
    this->_socket = src._socket;
    this->_endpoint = src._endpoint;
}

tcp_domain_client::tcp_domain_client(const comno::domain::stream_protocol::socket& sock_fd) noexcept
{
    _socket = sock_fd;
}

tcp_domain_client::~tcp_domain_client() noexcept
{
}

bool tcp_domain_client::connect(const std::string& domain_file)
{
    return _socket.connect(comno::domain::stream_protocol::endpoint(domain_file));
}