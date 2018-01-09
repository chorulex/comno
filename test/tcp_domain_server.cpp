#include <cstddef>
#include "tcp_domain_server.h"

tcp_domain_server::tcp_domain_server()
{
}
tcp_domain_server::~tcp_domain_server()
{
    if( _acceptor != nullptr )
        _acceptor->close();
}

bool tcp_domain_server::listen(const std::string& path)
{
    ::unlink(path.c_str());

    comno::domain::stream_protocol::endpoint ep(path);
    _acceptor = new comno::domain::stream_protocol::acceptor(ep);
    return true;
}

tcp_domain_client tcp_domain_server::accept() 
{ 
    return tcp_domain_client(_acceptor->accept());
}