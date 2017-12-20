#ifndef _QT_TCP_DOMAIN_CLIENT_SOCKET_H_
#define _QT_TCP_DOMAIN_CLIENT_SOCKET_H_

#include "socket.h"

namespace comno
{
class tcp_domain_client : public tcp_socket
{
public:
    tcp_domain_client() noexcept;
    tcp_domain_client(const SocketFd sock_fd, const std::string& domain_file) noexcept;

    /**
     * not close socket fd on this calling.
     * close fd manually.
     */
    ~tcp_domain_client() noexcept;

public:
    bool connect(const std::string& domain_file);
    const std::string& domain_file() const { return _domain_file;}

private:
    int create_fd() override;

private:
    std::string _domain_file;
};

}

#endif
