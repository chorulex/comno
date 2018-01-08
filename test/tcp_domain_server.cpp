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
    _acceptor = new comno::domain::stream_protocol::acceptor(comno::domain::stream_protocol::endpoint(path));
    return true;
}

tcp_domain_client tcp_domain_server::accept() 
{ 
    comno::domain::stream_protocol::socket client = _acceptor->accept();
    return tcp_domain_client(client);
}