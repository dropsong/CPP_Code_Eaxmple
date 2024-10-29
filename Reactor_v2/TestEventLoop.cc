#include "EventLoop.h"
#include "TcpConnection.h"
#include "Acceptor.h"
#include <iostream>

using std::cout;
using std::endl;

void onConnection(const TcpConnectionPtr &con)
{
    cout << con->toString() << " has connected!" << endl;
}

void onMessage(const TcpConnectionPtr &con)
{
    //接收客户端的数据
    string msg = con->receive();
    cout << ">>recv msg from client: " << msg << endl;

    //发送数据给客户端
    con->send(msg);
}

void onClose(const TcpConnectionPtr &con)
{
    cout << con->toString() << " has closed!" << endl;
}

int main(int argc, char **argv)
{
    /* Acceptor acceptor(argv[1], atoi(argv[2])); */
    Acceptor acceptor("127.0.0.1", 8888);
    acceptor.ready();//让服务器处于监听状态
    
    EventLoop loop(acceptor);
    loop.setConnectionCallback(std::move(onConnection));
    loop.setMessageCallback(std::move(onMessage));
    loop.setCloseCallback(std::move(onClose));
    loop.loop();

    return 0;
}

// 类外的onConnection--->EventLoop--->setConnectionCallback(cb)---->cb_onConnection---
// >TcpConnection
