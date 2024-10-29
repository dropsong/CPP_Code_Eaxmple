#ifndef __TIMERFD_H__
#define __TIMERFD_H__

#include <iostream>
#include <functional>

using std::cout;
using std::cerr;
using std::endl;
using std::function;

class TimerFd
{
    using TimerFdCallback =  function<void()>;
public:
    TimerFd(int initSec, int periodSec, TimerFdCallback &&cb);
    ~TimerFd();
    void start();
    void stop();

private:
    //处理读操作
    void handleRead();
    //创建文件描述符
    int createTimerFd();
    void setTimerFd(int initsec, int periodSec);

private:
    int _timerfd;
    int _initSec;
    int _periodSec;
    bool _isStarted;//标志位
    TimerFdCallback _cb;
};

#endif
