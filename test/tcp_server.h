#ifndef _COMNO_TCP_SERVER_SOCKET_H_
#define _COMNO_TCP_SERVER_SOCKET_H_

#include <string>

#include "tcp_client.h"
#include "comno/socket.h"

class tcp_server
{
public:
    tcp_server();
    ~tcp_server();

public:
    bool listen(const unsigned int port);
    tcp_client accept();

    bool is_open() const
    {
        return _acceptor == nullptr ? false : _acceptor->is_open();
    }
    void close()
    {
        if( _acceptor != nullptr )
            _acceptor->close();
    }

    unsigned int listen_port() const { return _acceptor->local_endpoint().port();}
    
    void reuse_address(bool enable)
    {
        if( _acceptor != nullptr ){
            comno::tcp::socket::reuse_address opt(true);
            _acceptor->set_option(opt);
        }
    }

    comno::tcp::endpoint local_endpoint() const
    {
        return  _acceptor != nullptr ? _acceptor->local_endpoint() : comno::tcp::endpoint();
    }

private:
    comno::tcp::acceptor* _acceptor = nullptr;
};

#endif