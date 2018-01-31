#ifndef _COMNO_ACCEPTOR_
#define _COMNO_ACCEPTOR_

#include <cstring>

#include "comno/utility/noncopyable.h"
#include "comno/impl/ip/basic_endpoint.h"
#include "comno/impl/domain/basic_endpoint.h"
#include "comno/impl/basic_socket.h"
#include "comno/impl/basic_stream_socket.h"

namespace comno
{

template <typename protocol>
class basic_acceptor
    : public comno::utility::noncopyable
    , public comno::basic_socket<protocol>
{
    using endpoint = typename protocol::endpoint;

public:
    using socket = typename protocol::socket;

    /**
     *  Bind to endpoint, and listen on this endpoint.
     * 
     * @NOTE: 
     * If protocol is domain, endpoint include domain file path,
     * if this file exist, fail to bind! So shuld to remove this file with unlink().
     * 
     */
    explicit basic_acceptor(const endpoint& ep, int backlog = socket_base::max_listen_connections)
    {
        this->bind(ep);
        listen(backlog);
    }
    ~basic_acceptor()
    {
    }

public:
    socket accept()
    {
        endpoint ep;

        socklen_t addr_len = ep.size();

        int clientSocket = ::accept(this->native_handle(), (struct sockaddr*)ep.data(), &addr_len);
        if (clientSocket == -1 )
            comno::throw_exception(errno);

        return socket(comno::socket_t(clientSocket), this->local_endpoint(), ep);
    }

private:
    void listen(int backlog = socket_base::max_listen_connections)
    {
        int res = ::listen(this->native_handle(), backlog);
        if( res == -1 ) 
            comno::throw_exception(errno);
    }
};

}
#endif