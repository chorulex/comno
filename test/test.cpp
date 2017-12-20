#include <cstdio>
#include <cstring>
#include <cassert>
#include <errno.h>
#include <sys/signal.h>

#include <string>
#include <vector>
#include <iostream>

#include "socket_exception.h"
#include "tcp_client.h"
#include "tcp_server.h"
#include "tcp_domain_client.h"
#include "tcp_domain_server.h"

using namespace QtSocket;

const char* PROMPT_STR = ">> ";
#define TEST_PROMPT(func) printf("[%s] --- RUNNING\n", func);
#define ERR_PROMPT() printf("line:%d, error:%s.\n", __LINE__, ex.what());

void TestSockOpt()
{
    TEST_PROMPT(__FUNCTION__);

    QtSocket::TCPClient client;

    int rec_buff_size = 2048;
    client.SetSockOpt(SOL_SOCKET, SO_RCVBUF, rec_buff_size);
    client.GetSockOpt(SOL_SOCKET, SO_RCVBUF, rec_buff_size);
    assert( rec_buff_size > 0 );

    time_t timeout = 3;
    client.SetRecvTimeOut(timeout);
    assert( client.GetRecvTimeOut() == timeout );

    client.SetSendTimeOut(timeout);
    assert( client.GetSendTimeOut() == timeout );
}

void TestConnect()
{
    TEST_PROMPT(__FUNCTION__);

    QtSocket::TCPClient socket;

    try{
        QtSocket::EndPoint host("192.168.2.186", 30000);
        socket.Connect(host);
    }catch(QtSocket::SocketException& ex){
        assert(ex.error_code() == ECONNREFUSED );
    }

    try{
        QtSocket::EndPoint host("192.168.2.600", 20000); //no this host
        socket.Connect(host);
    }catch(QtSocket::SocketException& ex){
        assert(ex.error_code() == EINVAL );
    }

    try{
        QtSocket::EndPoint host("192.168.2.0", 20000); //no this host
        socket.Connect(host);
    }catch(QtSocket::SocketException& ex){
        assert(ex.error_code() == ENETUNREACH );
    }

    try{
        QtSocket::EndPoint host("192.168.2.16", 20000); //no this host
        socket.Connect(host);
    }catch(QtSocket::SocketException& ex){
        assert(ex.error_code() == EHOSTUNREACH );
    }

    try{
        QtSocket::EndPoint host("192.168.2", 20000); //no this host
        socket.Connect(host);
    }catch(QtSocket::SocketException& ex){
        assert(ex.error_code() == ETIMEDOUT );
    }
}

void TestConnectTimeout()
{
    TEST_PROMPT(__FUNCTION__);

    QtSocket::TCPClient socket;

    try{
        QtSocket::EndPoint host("192.168.2.186", 20000);
        socket.Connect(host, 3);
    }catch(QtSocket::SocketException& ex){
        assert(ex.error_code() == ECONNREFUSED );
    }

    try{
        QtSocket::EndPoint host("192.168.2.186", 20000);
        socket.Connect(host, 3);
    }catch(QtSocket::SocketException& ex){
        assert(ex.error_code() == ECONNABORTED );
    }

    try{
        QtSocket::EndPoint host("192.168.2.600", 20000); //no this host
        socket.Connect(host, 3);
    }catch(QtSocket::SocketException& ex){
        assert(ex.error_code() == EINVAL );
    }

    try{
        QtSocket::EndPoint host("192.168.2", 20000); //no this host
        socket.Connect(host, 3);
    }catch(QtSocket::SocketException& ex){
        assert(ex.error_code() == EINPROGRESS );
    }

    try{
        QtSocket::EndPoint host("192.168.2.0", 20000); //no this host
        socket.Connect(host, 3);
    }catch(QtSocket::SocketException& ex){
        assert(ex.error_code() == EALREADY );
    }

    try{
        QtSocket::EndPoint host("192.168.2.16", 20000); //no this host
        socket.Connect(host, 3);
    }catch(QtSocket::SocketException& ex){
        assert(ex.error_code() == EALREADY );
    }
}

void TestListen()
{
    TEST_PROMPT(__FUNCTION__);
    QtSocket::TCPServer srv;

    try{
        srv.Listen(22);
    }catch(QtSocket::SocketException& ex){
        assert(ex.error_code() == EACCES );
    }
    unsigned int port = srv.ListenPort();
    assert(port == 0);
    srv.Close();

    QtSocket::TCPServer srv2;
    srv2.Listen(20000);
    port = srv2.ListenPort();
    assert(port == 20000);

    try{
        bool res = srv2.Listen(20000);
        assert(res == true);
    }catch(QtSocket::SocketException& ex){
        assert(ex.error_code() == EINVAL );
    }

    try{
        QtSocket::TCPServer srv3;
        bool res = srv3.Listen(20000);
    }catch(QtSocket::SocketException& ex){
        assert(ex.error_code() == EADDRINUSE );
    }
    srv2.Close();
}

#include <thread>
#include <chrono>
void TestAccept()
{
    TEST_PROMPT(__FUNCTION__);

    std::thread listen_thread([]{
        QtSocket::TCPServer srv;
        srv.SetReuseAddr(true);
        srv.Listen(20000);

        QtSocket::TCPClient client = srv.Accept();
        assert(client.DestPort() == 20000);
        assert(client.DestIP().empty());

        assert(client.SrcIP() == "127.0.0.1");
        assert(client.SrcPort() == 0);
    });

    std::thread client_thread([]{
        std::this_thread::sleep_for(std::chrono::seconds(2));

        EndPoint host("127.0.0.1", 20000);
        QtSocket::TCPClient client;
        client.Connect(host);
    });

    if( listen_thread.joinable() )
        listen_thread.join();
    if( client_thread.joinable() )
        client_thread.join();
}
void TestDomainListen()
{
    TEST_PROMPT(__FUNCTION__);
    const char* domain_path = "test_domain_file";

    QtSocket::TCPDomainServer srv;
    srv.Listen(domain_path);
}
void TestDomainConnect()
{
    TEST_PROMPT(__FUNCTION__);
    const char* domain_path = "test_domain_file";

    QtSocket::TCPDomainClient client;
    try{
        client.Connect(domain_path);
    }catch(QtSocket::SocketException& ex){
        assert(ex.error_code() == ENOENT );
    }
}
void TestDomainAccept()
{
    TEST_PROMPT(__FUNCTION__);
    const char* domain_path = "test_domain_file";

    std::thread listen_thread([&domain_path]{
        QtSocket::TCPDomainServer srv;
        srv.Listen(domain_path);

        QtSocket::TCPDomainClient client = srv.Accept();
        assert(client.FD() != 0);
        assert(client.DomainFile() == domain_path);
    });

    std::thread client_thread([&domain_path]{
        std::this_thread::sleep_for(std::chrono::seconds(2));

        QtSocket::TCPDomainClient client;
        client.Connect(domain_path);
        assert(client.DomainFile() == domain_path);
    });

    if( listen_thread.joinable() )
        listen_thread.join();
    if( client_thread.joinable() )
        client_thread.join();
}

int main(int argc, char* argv[])
{
    TestSockOpt();
    TestAccept();

    TestDomainListen();
    TestDomainConnect();
    TestDomainAccept();

    TestConnect();
    TestConnectTimeout();
    TestListen();

    return 0;
}

