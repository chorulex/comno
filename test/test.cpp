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

using namespace comno;

const char* PROMPT_STR = ">> ";
#define TEST_PROMPT(func) printf("[%s] --- RUNNING\n", func);
#define ERR_PROMPT() printf("line:%d, error:%s.\n", __LINE__, ex.what());

void TestSockOpt()
{
    TEST_PROMPT(__FUNCTION__);

    comno::tcp_client client;

    int rec_buff_size = 2048;
    client.set_sock_opt(SOL_SOCKET, SO_RCVBUF, rec_buff_size);
    client.get_sock_opt(SOL_SOCKET, SO_RCVBUF, rec_buff_size);
    assert( rec_buff_size > 0 );

    time_t timeout = 3;
    client.set_recv_timeout(timeout);
    assert( client.get_recv_timeout() == timeout );

    client.set_send_timeout(timeout);
    assert( client.get_send_timeout() == timeout );
}

void TestConnect()
{
    TEST_PROMPT(__FUNCTION__);

    comno::tcp_client socket;

    try{
        comno::end_point host("192.168.2.186", 30000);
        socket.connect(host);
    }catch(comno::socket_exception& ex){
        assert(ex.error_code() == ECONNREFUSED );
    }

    try{
        comno::end_point host("192.168.2.600", 20000); //no this host
        socket.connect(host);
    }catch(comno::socket_exception& ex){
        assert(ex.error_code() == EINVAL );
    }

    try{
        comno::end_point host("192.168.2.0", 20000); //no this host
        socket.connect(host);
    }catch(comno::socket_exception& ex){
        assert(ex.error_code() == ENETUNREACH );
    }

    try{
        comno::end_point host("192.168.2.16", 20000); //no this host
        socket.connect(host);
    }catch(comno::socket_exception& ex){
        assert(ex.error_code() == EHOSTUNREACH );
    }

    try{
        comno::end_point host("192.168.2", 20000); //no this host
        socket.connect(host);
    }catch(comno::socket_exception& ex){
        assert(ex.error_code() == ETIMEDOUT );
    }
}

void TestConnectTimeout()
{
    TEST_PROMPT(__FUNCTION__);

    comno::tcp_client socket;

    try{
        comno::end_point host("192.168.2.186", 20000);
        socket.connect(host, 3);
    }catch(comno::socket_exception& ex){
        assert(ex.error_code() == ECONNREFUSED );
    }

    try{
        comno::end_point host("192.168.2.186", 20000);
        socket.connect(host, 3);
    }catch(comno::socket_exception& ex){
        assert(ex.error_code() == ECONNABORTED );
    }

    try{
        comno::end_point host("192.168.2.600", 20000); //no this host
        socket.connect(host, 3);
    }catch(comno::socket_exception& ex){
        assert(ex.error_code() == EINVAL );
    }

    try{
        comno::end_point host("192.168.2", 20000); //no this host
        socket.connect(host, 3);
    }catch(comno::socket_exception& ex){
        assert(ex.error_code() == EINPROGRESS );
    }

    try{
        comno::end_point host("192.168.2.0", 20000); //no this host
        socket.connect(host, 3);
    }catch(comno::socket_exception& ex){
        assert(ex.error_code() == EALREADY );
    }

    try{
        comno::end_point host("192.168.2.16", 20000); //no this host
        socket.connect(host, 3);
    }catch(comno::socket_exception& ex){
        assert(ex.error_code() == EALREADY );
    }
}

void TestListen()
{
    TEST_PROMPT(__FUNCTION__);
    comno::tcp_server srv;

    try{
        srv.listen(22);
    }catch(comno::socket_exception& ex){
        assert(ex.error_code() == EACCES );
    }
    unsigned int port = srv.listen_port();
    assert(port == 0);
    srv.close();

    comno::tcp_server srv2;
    srv2.listen(20000);
    port = srv2.listen_port();
    assert(port == 20000);

    try{
        bool res = srv2.listen(20000);
        assert(res == true);
    }catch(comno::socket_exception& ex){
        assert(ex.error_code() == EINVAL );
    }

    try{
        comno::tcp_server srv3;
        bool res = srv3.listen(20000);
    }catch(comno::socket_exception& ex){
        assert(ex.error_code() == EADDRINUSE );
    }
    srv2.close();
}

#include <thread>
#include <chrono>
void TestAccept()
{
    TEST_PROMPT(__FUNCTION__);

    std::thread listen_thread([]{
        comno::tcp_server srv;
        srv.set_reuse_addr(true);
        srv.listen(20000);

        comno::tcp_client client = srv.accept();
        assert(client.dest_end_point().port == 20000);
        assert(client.dest_end_point().ip.empty());

        assert(client.src_end_point().ip == "127.0.0.1");
        assert(client.src_end_point().port == 0);
    });

    std::thread client_thread([]{
        std::this_thread::sleep_for(std::chrono::seconds(2));

        end_point host("127.0.0.1", 20000);
        comno::tcp_client client;
        client.connect(host);
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

    comno::tcp_domain_server srv;
    srv.listen(domain_path);
}
void TestDomainConnect()
{
    TEST_PROMPT(__FUNCTION__);
    const char* domain_path = "test_domain_file";

    comno::tcp_domain_client client;
    try{
        client.connect(domain_path);
    }catch(comno::socket_exception& ex){
        assert(ex.error_code() == ENOENT );
    }
}
void TestDomainAccept()
{
    TEST_PROMPT(__FUNCTION__);
    const char* domain_path = "test_domain_file";

    std::thread listen_thread([&domain_path]{
        comno::tcp_domain_server srv;
        srv.listen(domain_path);

        comno::tcp_domain_client client = srv.accept();
        assert(client.fd() != 0);
        assert(client.domain_file() == domain_path);
    });

    std::thread client_thread([&domain_path]{
        std::this_thread::sleep_for(std::chrono::seconds(2));

        comno::tcp_domain_client client;
        client.connect(domain_path);
        assert(client.domain_file() == domain_path);
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

