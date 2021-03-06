#ifndef _COMNO_TCP_DOMAIN_SERVER_SOCKET_H_
#define _COMNO_TCP_DOMAIN_SERVER_SOCKET_H_

#include <string>

#include "end_point.h"
#include "tcp_socket.h"
#include "tcp_domain_client.h"
#include "socket_exception.h"

namespace comno
{
/**
 * speciafilly for UNIX/Linux Domain sockets
 *
 * AF_UNIX | SOCK_STREAM | IPPROTO_TCP
 */
class tcp_domain_server : public tcp_socket
{
public:
    tcp_domain_server();
    tcp_domain_server(const socket_t sock_fd, const std::string& file);
    ~tcp_domain_server();

public:
    bool listen(const std::string& file);
    tcp_domain_client accept();
    void close() override;

    const std::string& domain_file() const { return _domain_file;}

private:
    int create_fd() override;
    void bind(const std::string& file);

private:
    std::string _domain_file;
};

};
#endif


