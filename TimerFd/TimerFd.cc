#include "TimerFd.h"
#include <unistd.h>
#include <poll.h>
#include <stdio.h>
#include <sys/timerfd.h>

TimerFd::TimerFd(int initSec, int periodSec, TimerFdCallback &&cb)
: _timerfd(createTimerFd())
, _initSec(initSec)
, _periodSec(periodSec)
, _isStarted(false)
, _cb(std::move(cb))//回调函数的注册
{

}

TimerFd::~TimerFd()
{
    setTimerFd(0, 0);
    close(_timerfd);
}

void TimerFd::start()
{
    struct pollfd fds;
    fds.fd = _timerfd;
    fds.events = POLLIN;

    // 设置定时器
    setTimerFd(_initSec, _periodSec);

    _isStarted = true;
    while(_isStarted)
    {
        int nready = poll(&fds, 1, 5000);
        cout << "nready = " << nready << endl;
        if(-1 == nready && errno == EINTR)
        {
            continue;
        }
        else if(-1 == nready)
        {
            cerr << "poll error -1 == nready" << endl;
        }
        else if(0 == nready)
        {
            cout << ">>poll timeout" << endl;
        }
        else
        {
            //监听的文件描述符可读
            if(fds.revents & POLLIN)
            {
                handleRead();
                if(_cb)
                {
                    _cb();//执行任务
                }
            }
        }
    }
}

void TimerFd::stop()
{
    if(!_isStarted) return;

    _isStarted = false;
    setTimerFd(0, 0);
}

void TimerFd::handleRead()
{
    uint64_t one;
    ssize_t ret = read(_timerfd, &one, sizeof(one));
    if(ret != sizeof(one))
    {
        perror("read\n");
        return;
    }
}

int TimerFd::createTimerFd()
{
    int fd = timerfd_create(CLOCK_REALTIME, 0);
    if(fd < 0)
    {
        perror("timerfd_create\n");
    }

    return fd;
}

void TimerFd::setTimerFd(int initsec, int periodSec)
{
    itimerspec value;

    // 设置初始时间
    value.it_value.tv_sec = initsec;
    value.it_value.tv_nsec = 0;

    // 设置周期时间
    value.it_interval.tv_sec = periodSec;
    value.it_interval.tv_nsec = 0;

    int ret = timerfd_settime(_timerfd, 0, &value, nullptr);
    if(ret < 0)
    {
        perror("timerfd_settime\n");
        return;
    }
}