#ifndef _COMNO_ACCEPTOR_
#define _COMNO_ACCEPTOR_

#include <cstring>

#include "comno/utility/noncopyable.h"
#include "comno/impl/ip/endpoint.h"
#include "comno/impl/domain/endpoint.h"
#include "basic_socket.h"

namespace comno
{

template <typename protocol>
class basic_acceptor:
    public utility::noncopyable,
    public comno::basic_socket<protocol>
{
    using endpoint = typename protocol::endpoint;

public:
    using socket = typename protocol::socket;

    explicit basic_acceptor(const endpoint& ep, int backlog = socket_base::max_listen_connections)
    {
        bind(ep);
        listen(backlog);
    }
    ~basic_acceptor()
    {
        this->close();
    }

public:
    socket accept()
    {
        endpoint ep;

        socklen_t addr_len = ep.size();

        int clientSocket = ::accept(this->fd(), (struct sockaddr*)ep.data(), &addr_len);
        if (clientSocket == -1 )
            throw comno::socket_exception(comno::system::error_code(errno));

        return socket(comno::socket_t(clientSocket), this->local_endpoint(), ep);
    }

private:
    void bind(const endpoint& ep)
    {
        this->local_endpoint(ep);
        int res = ::bind(this->fd(), ep.data(), ep.size());
        if( res == -1 )
            throw comno::socket_exception(comno::system::error_code(errno));
    }

    void listen(int backlog = socket_base::max_listen_connections)
    {
        int res = ::listen(this->fd(), backlog);
        if( res == -1 ) 
            throw comno::socket_exception(comno::system::error_code(errno));
    }

};

}
#endif