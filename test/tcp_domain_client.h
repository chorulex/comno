#ifndef _COMNO_TCP_DOMAIN_CLIENT_SOCKET_H_
#define _COMNO_TCP_DOMAIN_CLIENT_SOCKET_H_

#include <cstddef>
#include <string>

#include "comno/socket.h"

class tcp_domain_client
{
public:
    tcp_domain_client(const tcp_domain_client&) = delete;
    tcp_domain_client& operator= (const tcp_domain_client&) = delete;

    tcp_domain_client() noexcept;
    explicit tcp_domain_client(comno::domain::stream_protocol::socket&&) noexcept;
    tcp_domain_client(tcp_domain_client&&) noexcept;

    ~tcp_domain_client() noexcept;

public:
    bool connect(const std::string& domain_file);
    std::string domain_file() const
    {
        return std::string(_socket.local_endpoint().path());
    }

private:
    comno::domain::stream_protocol::socket _socket;
};

#endif
