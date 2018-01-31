#include <unistd.h>
#include <errno.h>
#include <sys/signal.h>
#include <cstdio>
#include <cstring>

#include <type_traits>
#include <string>
#include <vector>
#include <iostream>
#include <thread>
#include <chrono>

#include "comno_assert.h"

#include "comno/socket.h"
#include "tcp_client.h"
#include "tcp_server.h"
#include "tcp_domain_client.h"
#include "tcp_domain_server.h"

using namespace comno;

const char* PROMPT_STR = ">> ";
#define TEST_PROMPT(func) printf("[%s] --- RUNNING\n", func);
#define ERR_PROMPT() printf("line:%d, error:%s.\n", __LINE__, ex.what());

static const char* domain_file = "test_domain_file";

void TestIPEndpoint()
{
    TEST_PROMPT(__FUNCTION__);

    const char* ip = "127.0.0.1";
    const unsigned port = 20000;

    comno::address_v4 addr = comno::address_v4::from_string(ip);

    comno::tcp::endpoint ep1(addr, 20000);
    EQUAL(ep1.address().to_string(), ip);
    EQUAL(ep1.port(), port);

    ep1.port(3000);
    EQUAL(ep1.port(), 3000);

    comno::tcp::endpoint ep2(addr.to_uint(), 20000);
    EQUAL(ep2.address().to_string(), ip);
    EQUAL(ep2.port(), port);

    comno::address_v4 addr1 = comno::address_v4::from_string("0.0.0.0");
    ep2.address(addr1);
    EQUAL(ep2.address().to_string(), "0.0.0.0");

    ep2.address(addr1.to_uint());
    EQUAL(ep2.address().to_string(), "0.0.0.0");
}

void TestNetAddress()
{
    TEST_PROMPT(__FUNCTION__);
    comno::address_v4 addr;
    
    const char* ip = "127.0.0.1";
    comno::address_v4 addr_from_str = comno::address_v4::from_string(ip);
    comno::address_v4 addr_from_str_1 = comno::address_v4::from_string(std::string(ip));
    comno::address_v4 addr_default = comno::address_v4::any();
}
void TestCreateTCPSocket()
{
    TEST_PROMPT(__FUNCTION__);

    //comno::io_server io_srv;
    
}
void TestHostname()
{
    TEST_PROMPT(__FUNCTION__);

    char name[1024];
    gethostname(name, 1024);

    EQUAL(comno::host_name() , name);
}

void TestSockOpt()
{
    TEST_PROMPT(__FUNCTION__);

    const int rec_buff_size = 2048;

    comno::tcp::socket::receive_buf_size opt(rec_buff_size);
    EQUAL( opt.level() , SOL_SOCKET);
    EQUAL( opt.name() , SO_RCVBUF);
    EQUAL( opt.size() , sizeof(int) );
    NOT_NULL_POINTER( opt.data());
    EQUAL( opt.value() , rec_buff_size );

    comno::tcp::socket sock;
    sock.set_option(opt);
    EQUAL( opt.value() , rec_buff_size );
    NOT_EQUAL( opt.value() , 0 );

    sock.get_option(opt);
    NOT_EQUAL( opt.value() , rec_buff_size );
    NOT_EQUAL( opt.value() , 0 );
}

void TestTCPSockTimeout()
{
    TEST_PROMPT(__FUNCTION__);

    tcp_client client;

    time_t timeout = 3;
    client.set_recv_timeout(timeout);
    EQUAL( client.get_recv_timeout() , timeout );

    client.set_send_timeout(timeout);
    EQUAL( client.get_send_timeout() , timeout );
}

void TestConnect()
{
    TEST_PROMPT(__FUNCTION__);

    tcp_client socket;

    try{
        comno::tcp::endpoint host(comno::address_v4::from_string("192.168.2.186"), 30000);
        socket.connect(host);
    }catch(comno::exception& ex){
        EQUAL(ex.error_code() , ECONNREFUSED );
    }

    try{
        comno::tcp::endpoint host(comno::address_v4::from_string("192.168.2.600"), 30000);
        socket.connect(host);
    }catch(comno::exception& ex){
        EQUAL(ex.error_code() , EINVAL );
    }

    try{
        comno::tcp::endpoint host(comno::address_v4::from_string("192.168.2.0"), 20000); //no this host
        socket.connect(host);
    }catch(comno::exception& ex){
        EQUAL(ex.error_code() , ENETUNREACH );
    }

    try{
        comno::tcp::endpoint host(comno::address_v4::from_string("192.168.2.16"), 20000); //no this host
        socket.connect(host);
    }catch(comno::exception& ex){
        EQUAL(ex.error_code() , EHOSTUNREACH );
    }

    try{
        comno::tcp::endpoint host(comno::address_v4::from_string("192.168.2"), 20000); //no this host
        socket.connect(host);
    }catch(comno::exception& ex){
        EQUAL(ex.error_code() , ETIMEDOUT );
    }
}

void TestConnectTimeout()
{
    TEST_PROMPT(__FUNCTION__);

    tcp_client socket;

    try{
        comno::tcp::endpoint host(comno::address_v4::from_string("192.168.2.186"), 20000);
        socket.connect(host, 3);
    }catch(comno::exception& ex){
        EQUAL(ex.error_code() , ECONNREFUSED );
    }

    try{
        comno::tcp::endpoint host(comno::address_v4::from_string("192.168.2.186"), 20000);
        socket.connect(host, 3);
    }catch(comno::exception& ex){
        EQUAL(ex.error_code() , ECONNABORTED );
    }

    try{
        comno::tcp::endpoint host(comno::address_v4::from_string("192.168.2.600"), 20000); //no this host
        socket.connect(host, 3);
    }catch(comno::exception& ex){
        EQUAL(ex.error_code() , EINVAL );
    }

    try{
        comno::tcp::endpoint host(comno::address_v4::from_string("192.168.2.2"), 20000); //no this host
        socket.connect(host, 3);
    }catch(comno::exception& ex){
        EQUAL(ex.error_code() , EINPROGRESS );
    }

    try{
        comno::tcp::endpoint host(comno::address_v4::from_string("192.168.2.0"), 20000); //no this host
        socket.connect(host, 3);
    }catch(comno::exception& ex){
        EQUAL(ex.error_code() , EALREADY );
    }

    try{
        comno::tcp::endpoint host(comno::address_v4::from_string("192.168.2.16"), 20000); //no this host
        socket.connect(host, 3);
    }catch(comno::exception& ex){
        EQUAL(ex.error_code() , EALREADY );
    }
}

void TestListen()
{
    TEST_PROMPT(__FUNCTION__);
    tcp_server srv;

    EQUAL( srv.is_open(), false);

    try{
        srv.listen(22);
    }catch(comno::exception& ex){
        EQUAL(ex.error_code() , EACCES );
    }

    EQUAL( srv.is_open(), false);
    srv.close();

    EQUAL( srv.is_open(), false );

    try{
        tcp_server srv2;
        bool res = srv2.listen(-22);
        EQUAL(res , true);
    }catch(comno::exception& ex){
        EQUAL(ex.error_code() , EINVAL );
    }
}

void TestAccept()
{
    TEST_PROMPT(__FUNCTION__);

    tcp_server srv;
    srv.reuse_address(true);

    srv.listen(20000);
    EQUAL(srv.local_endpoint().port() , 20000);
    EQUAL(srv.local_endpoint().address().to_string() , "0.0.0.0");


    std::thread listen_thread([&srv]{
        tcp_client client = srv.accept();
        NOT_EQUAL(client.remote_endpoint().port() , 0);
        EQUAL(client.remote_endpoint().address().to_string() , "127.0.0.1");
        NOT_EQUAL(client.remote_endpoint().address() , comno::address_v4());

        EQUAL(client.local_endpoint().port() , 20000);
        EQUAL(client.local_endpoint().address().to_string() , "0.0.0.0");
        EQUAL(client.local_endpoint().address() , comno::address_v4());
    });

    std::thread client_thread([]{
        std::this_thread::sleep_for(std::chrono::seconds(2));

        comno::tcp::endpoint host(comno::address_v4::from_string("127.0.0.1"), 20000);
        tcp_client client;
        client.connect(host);
    });

    if( listen_thread.joinable() )
        listen_thread.join();
    if( client_thread.joinable() )
        client_thread.join();
    srv.close();
}

void TestSendAndRecv()
{
    TEST_PROMPT(__FUNCTION__);

    comno::tcp::endpoint host(comno::address_v4::from_string("127.0.0.1"), 20000);

    std::thread listen_thread([&host]{
        comno::tcp::acceptor srv(host);
        EQUAL(srv.local_endpoint(), host)

        comno::tcp::socket client = srv.accept();
        NOT_EQUAL(client.remote_endpoint().port(), 0);
        EQUAL(client.local_endpoint(), host);

        char buffer[32] = {0};
        std::size_t size = client.receive(buffer, sizeof(buffer) - 1);
        EQUAL(size, 11);
        EQUAL(std::string(buffer), "hello,comno");

        ::memset(buffer, 0, sizeof(buffer));
        size = client.receive(buffer, sizeof(buffer) - 1);
        EQUAL(size, 11);
        EQUAL(std::string(buffer), "hello,comno");
        srv.close();
    });

    std::thread client_thread([&host]{
        std::this_thread::sleep_for(std::chrono::seconds(2));

        comno::tcp::socket client;
        client.connect(host);
        NOT_EQUAL(client.local_endpoint().port(), 0);
        EQUAL(client.local_endpoint().address(), host.address());

        std::size_t size = client.send("hello,comno");
        EQUAL(size, 11);

        std::this_thread::sleep_for(std::chrono::seconds(2));
        size = client.send(std::string("hello,comno"));
        EQUAL(size, 11);

        client.close();
    });

    if( listen_thread.joinable() )
        listen_thread.join();
    if( client_thread.joinable() )
        client_thread.join();
}
void TestDomainSocket()
{
    TEST_PROMPT(__FUNCTION__);
    comno::domain::stream_protocol::socket sk;

    sk.open();
    EQUAL(sk.is_open(), true);

    sk.close();
    EQUAL(sk.is_open(), false);

    sk.open();
    EQUAL(sk.is_open(), true);
    sk.close();
}

void TestDomainListen()
{
    TEST_PROMPT(__FUNCTION__);

    tcp_domain_server srv;

    try{
        srv.listen(domain_file);
    }catch(comno::exception& ex){
        NOT_EQUAL(ex.error_code(), EADDRINUSE);
    }
}

void TestDomainConnect()
{
    TEST_PROMPT(__FUNCTION__);

    tcp_domain_client client;
    try{
        client.connect(domain_file);
    }catch(comno::exception& ex){
        EQUAL(ex.error_code() , ENOENT );
    }
}

void TestDomainAccept()
{
    TEST_PROMPT(__FUNCTION__);

    std::thread listen_thread([]{
        tcp_domain_server srv;
        srv.listen(domain_file);

        tcp_domain_client client = srv.accept();
        EQUAL(client.domain_file() , std::string(domain_file));
    });

    std::thread client_thread([]{
        std::this_thread::sleep_for(std::chrono::seconds(2));

        tcp_domain_client client;
        client.connect(domain_file);
        EQUAL(client.domain_file() , std::string(""));
    });

    if( listen_thread.joinable() )
        listen_thread.join();
    if( client_thread.joinable() )
        client_thread.join();
}

void TestDomainSendAndRecv()
{
    TEST_PROMPT(__FUNCTION__);

    ::unlink(domain_file);
    comno::domain::stream_protocol::endpoint host(domain_file);

    std::thread listen_thread([&host]{
        comno::domain::stream_protocol::acceptor srv(host);
        comno::domain::stream_protocol::socket client = srv.accept();

        char buffer[32] = {0};
        std::size_t size = client.receive(buffer, sizeof(buffer) - 1);
        EQUAL(size, 11);
        EQUAL(std::string(buffer), "hello,comno");

        ::memset(buffer, 0, sizeof(buffer));
        size = client.receive(buffer, sizeof(buffer) - 1);
        EQUAL(size, 11);
        EQUAL(std::string(buffer), "hello,comno");
        srv.close();
    });

    std::thread client_thread([&host]{
        std::this_thread::sleep_for(std::chrono::seconds(2));

        comno::domain::stream_protocol::socket client;
        client.connect(host);

        std::size_t size = client.send("hello,comno");
        EQUAL(size, 11);

        std::this_thread::sleep_for(std::chrono::seconds(2));
        size = client.send(std::string("hello,comno"));
        EQUAL(size, 11);

        client.close();
    });

    if( listen_thread.joinable() )
        listen_thread.join();
    if( client_thread.joinable() )
        client_thread.join();
}

void TestUDP()
{
    TEST_PROMPT(__FUNCTION__);

    comno::udp::endpoint host(comno::address_v4::from_string("127.0.0.1"), 20000);
    const std::string buffer("hello, comno udp.");

    comno::udp::socket svr;
    svr.bind(host);

    comno::udp::socket client;
    std::size_t size = client.send_to(host, buffer);
    EQUAL(size, buffer.size());

    char msg[64] = {0};
    comno::udp::endpoint ep;
    size = svr.receive_from(ep, msg, sizeof(msg)-1);
    EQUAL(size, buffer.size());
    EQUAL(ep.address().to_string(), "127.0.0.1");
    NOT_EQUAL(ep.port(), 0);
}

int main(int argc, char* argv[])
{
    TestNetAddress();
    TestIPEndpoint();
    TestHostname();
    TestCreateTCPSocket();
    TestSockOpt();
    TestTCPSockTimeout();

    TestListen();
    TestAccept();
    TestSendAndRecv();

    TestDomainSocket();
    TestDomainListen();
    TestDomainConnect();
    TestDomainAccept();
    TestDomainSendAndRecv();

    TestUDP();

    TestConnect();
    TestConnectTimeout();

    return 0;
}

