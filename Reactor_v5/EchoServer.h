#ifndef _ECHOSERVER_H_
#define _ECHOSERVER_H_

#include "TcpServer.h"
#include "TcpConnection.h"
#include "ThreadPool.h"
#include <iostream>

using std::cout;
using std::endl;

class MyTask
{
public:
    MyTask(const string &msg, const TcpConnectionPtr &con)
    : _msg(msg)
    , _con(con)
    {
    }

    ~MyTask() {}

    void process()
    {
        _con->sendInLoop(_msg);
    }

private:
    string _msg;
    TcpConnectionPtr _con;
};

class EchoServer
{
public:
    EchoServer(size_t threadNum, size_t queSize,
               const string& ip, unsigned short port)
    : _pool(threadNum, queSize)
    , _server(ip, port)
    {}

    ~EchoServer() {}

    void start()
    {
        // 这部分代码对比着 v4 看，总之就是要符合之前写的函数的形式。
        _pool.start();

        using namespace std::placeholders;
        _server.setAllCallback(std::bind(&EchoServer::onConnection, this, _1)
                             , std::bind(&EchoServer::onMessage, this, _1)
                             , std::bind(&EchoServer::onClose, this, _1));
        _server.start();
    }

    void stop()
    {
        _pool.stop();
        _server.stop();
    }

    void onConnection(const TcpConnectionPtr &con)
    {
        cout << con->toString() << " has connected!" << endl;
    }

    void onMessage(const TcpConnectionPtr &con)
    {
        string msg = con->receive();
        cout << ">>recv msg from client: " << msg << endl;

        MyTask task(msg, con);
        _pool.addTask(std::bind(&MyTask::process, task));//bind的地址传递与值传递
    }

    void onClose(const TcpConnectionPtr &con)
    {
        cout << con->toString() << " has closed!" << endl;
    }

private:
    ThreadPool _pool;
    TcpServer _server;
};

#endif