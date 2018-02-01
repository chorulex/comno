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
    using socket_type = typename protocol::socket;

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
    socket_type accept()
    {
        endpoint ep;
        socklen_t addr_len = ep.size();

        comno::system::error_code ec;
        comno::type::socket_type fd =
                    comno::detail::socket_ops::accept(this->native_handle(), ep.data(), &addr_len, ec);
        comno::throw_exception(ec);

        return socket_type(comno::socket_t(fd), this->local_endpoint(), ep);
    }

private:
    void listen(int backlog = socket_base::max_listen_connections)
    {
        comno::system::error_code ec;
        comno::detail::socket_ops::listen(this->native_handle(), backlog, ec);
        comno::throw_exception(ec);
    }
};

}
#endif