#ifndef _COMNO_TCP_DOMAIN_SERVER_SOCKET_H_
#define _COMNO_TCP_DOMAIN_SERVER_SOCKET_H_

#include <string>

#include "comno/socket.h"
#include "tcp_domain_client.h"

class tcp_domain_server
{
public:
    tcp_domain_server();
    ~tcp_domain_server();

public:
    bool listen(const std::string& file);
    tcp_domain_client accept();

    std::string domain_file() const
    {
        //return _socket.endpoint().path();
        return "";
    }

private:
    comno::domain::stream_protocol::acceptor* _acceptor = nullptr;
};

#endif