#include <cstdio>
#include <cstring>
#include <cassert>
#include <errno.h>
#include <sys/signal.h>

#include <string>
#include <vector>
#include <iostream>

#include "tcp_server.h"
#include "tcp_client.h"
#include "socket_exception.h"

using namespace QtSocket;

const char* PROMPT_STR = ">> ";
#define TEST_PROMPT(func) printf("[%s] --- RUNNING\n", func);
#define ERR_PROMPT() printf("line:%d, error:%s.\n", __LINE__, ex.what());

void TestConnect()
{
    TEST_PROMPT(__FUNCTION__);

    QtSocket::TCPClient socket;

    try{
        QtSocket::EndPoint host("192.168.2.186", 20000);
        socket.Connect(host);
    }catch(QtSocket::SocketException& ex){
        assert(ex.GetErrCode() == ECONNREFUSED );
    }

    try{
        QtSocket::EndPoint host("192.168.2.600", 20000); //no this host
        socket.Connect(host);
    }catch(QtSocket::SocketException& ex){
        assert(ex.GetErrCode() == EINVAL );
    }

    try{
        QtSocket::EndPoint host("192.168.2.0", 20000); //no this host
        socket.Connect(host);
    }catch(QtSocket::SocketException& ex){
        assert(ex.GetErrCode() == ENETUNREACH );
    }

    try{
        QtSocket::EndPoint host("192.168.2.16", 20000); //no this host
        socket.Connect(host);
    }catch(QtSocket::SocketException& ex){
        assert(ex.GetErrCode() == EHOSTUNREACH );
    }

    try{
        QtSocket::EndPoint host("192.168.2", 20000); //no this host
        socket.Connect(host);
    }catch(QtSocket::SocketException& ex){
        assert(ex.GetErrCode() == ETIMEDOUT );
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
        assert(ex.GetErrCode() == ECONNREFUSED );
    }

    try{
        QtSocket::EndPoint host("192.168.2.186", 20000);
        socket.Connect(host, 3);
    }catch(QtSocket::SocketException& ex){
        assert(ex.GetErrCode() == ECONNABORTED );
    }

    try{
        QtSocket::EndPoint host("192.168.2.600", 20000); //no this host
        socket.Connect(host, 3);
    }catch(QtSocket::SocketException& ex){
        assert(ex.GetErrCode() == EINVAL );
    }

    try{
        QtSocket::EndPoint host("192.168.2", 20000); //no this host
        socket.Connect(host, 3);
    }catch(QtSocket::SocketException& ex){
        assert(ex.GetErrCode() == EINPROGRESS );
    }

    try{
        QtSocket::EndPoint host("192.168.2.0", 20000); //no this host
        socket.Connect(host, 3);
    }catch(QtSocket::SocketException& ex){
        assert(ex.GetErrCode() == EALREADY );
    }

    try{
        QtSocket::EndPoint host("192.168.2.16", 20000); //no this host
        socket.Connect(host, 3);
    }catch(QtSocket::SocketException& ex){
        assert(ex.GetErrCode() == EALREADY );
    }
}

void TestListen()
{
    TEST_PROMPT(__FUNCTION__);
    QtSocket::TCPServer srv;

    try{
        srv.Listen(22);
    }catch(QtSocket::SocketException& ex){
        assert(ex.GetErrCode() == EACCES );
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
        assert(ex.GetErrCode() == EINVAL );
    }

    try{
        QtSocket::TCPServer srv3;
        bool res = srv3.Listen(20000);
    }catch(QtSocket::SocketException& ex){
        assert(ex.GetErrCode() == EADDRINUSE );
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
        srv.Listen(20000);

        QtSocket::TCPClient client = srv.Accept();
        assert(client.DestPort() == 20000);
        assert(client.DestIP().empty());

        assert(client.SrcIP() == "192.168.2.101");
        assert(client.SrcPort() == 0);
    });

    std::thread client_thread([]{
        std::this_thread::sleep_for(std::chrono::seconds(5));

        EndPoint host("192.168.2.101", 20000);
        QtSocket::TCPClient client;
        client.Connect(host);
    });

    if( listen_thread.joinable() )
        listen_thread.join();
    if( client_thread.joinable() )
        client_thread.join();
}

int main(int argc, char* argv[])
{
    TestAccept();

    TestConnect();
    TestConnectTimeout();
    TestListen();

    return 0;
}

