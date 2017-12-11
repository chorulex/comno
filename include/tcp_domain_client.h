#ifndef _QT_TCP_DOMAIN_CLIENT_SOCKET_H_
#define _QT_TCP_DOMAIN_CLIENT_SOCKET_H_

#include "socket.h"

namespace QtSocket
{
class TCPDomainClient : public TCPSocket
{
public:
    TCPDomainClient() noexcept;
    TCPDomainClient(const SocketFd sock_fd, const std::string& domain_file) noexcept;

    /**
     * not close socket fd on this calling.
     * close fd manually.
     */
    ~TCPDomainClient() noexcept;

public:
    bool Connect(const std::string& domain_file);
    const std::string& DomainFile() const { return _domain_file;}

private:
    int CreateFD();

private:
    std::string _domain_file;
};

}

#endif
