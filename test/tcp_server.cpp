#include "tcp_server.h"

tcp_server::tcp_server()
{
}

tcp_server::~tcp_server()
{
    if( _acceptor == nullptr )
        return;

    if( _acceptor->is_open() )
        _acceptor->close();

    delete _acceptor;
}

bool tcp_server::listen(const unsigned int port)
{
    _acceptor = new comno::tcp::acceptor(comno::tcp::endpoint(comno::address_v4::any(), port));
    return true;
}

tcp_client tcp_server::accept()
{
    comno::tcp::socket sock = _acceptor->accept();
    return tcp_client(sock);
}
