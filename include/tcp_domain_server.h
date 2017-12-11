#ifndef _QT_TCP_DOMAIN_SERVER_SOCKET_H_
#define _QT_TCP_DOMAIN_SERVER_SOCKET_H_

#include <string>

#include "end_point.h"
#include "socket.h"
#include "tcp_domain_client.h"
#include "socket_exception.h"

namespace QtSocket
{
/**
 * AF_UNIX | SOCK_STREAM | IPPROTO_TCP
 */
class TCPDomainServer : public TCPSocket
{
public:
    TCPDomainServer();
    TCPDomainServer(const SocketFd sock_fd, const std::string& file);
    ~TCPDomainServer();

public:
    bool Listen(const std::string& file);
    TCPDomainClient Accept();
    void Close() override;

    const std::string& DomainFile() const { return _domain_file;}

private:
    int CreateFD() override;
    void Bind(const std::string& file);

private:
    std::string _domain_file;
};

};
#endif


