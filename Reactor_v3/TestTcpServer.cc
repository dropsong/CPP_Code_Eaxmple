#include "TcpServer.h"
#include "TcpConnection.h"
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
    TcpServer server("127.0.0.1", 8888);
    server.setAllCallback(std::move(onConnection)
                          , std::move(onMessage)
                          , std::move(onClose));
    server.start();

    return 0;
}

