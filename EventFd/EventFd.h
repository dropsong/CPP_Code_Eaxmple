#ifndef __EVENTFD_H__
#define __EVENTFD_H__

#include <iostream>
#include <functional>

using std::cout;
using std::cerr;
using std::endl;
using std::function;


class EventFd
{
    using EventFdCallback =  function<void()>;
public:
    EventFd(EventFdCallback &&cb);
    ~EventFd();
    void start();
    void stop();
    //处理写操作
    void wakeup();

private:
    //处理读操作
    void handleRead();
    //创建文件描述符_evtfd
    int createEventFd();

private:
    int _evtfd;//evntfd的返回的文件描述符
    bool _isStarted;//标志位
    EventFdCallback _cb;


};

#endif
