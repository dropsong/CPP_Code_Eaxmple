#include "EventFd.h"
#include <unistd.h>
#include <poll.h>
#include <stdio.h>
#include <sys/eventfd.h>

EventFd::EventFd(EventFdCallback &&cb)
: _evtfd(createEventFd())
, _isStarted(false)
, _cb(std::move(cb))//回调函数的注册
{

}

EventFd::~EventFd()
{
    close(_evtfd);
}

void EventFd::start()
{

    struct pollfd fds;
    fds.fd = _evtfd;
    fds.events = POLLIN;

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

void EventFd::stop()
{
    _isStarted = false;
}

void EventFd::wakeup()
{
    uint64_t one;
    ssize_t ret = write(_evtfd, &one, sizeof(one));
    if(ret != sizeof(one))
    {
        perror("write\n");
        return;
    }
}

void EventFd::handleRead()
{
    uint64_t one;
    ssize_t ret = read(_evtfd, &one, sizeof(one));
    if(ret != sizeof(one))
    {
        perror("read\n");
        return;
    }
}

int EventFd::createEventFd()
{
    int fd = eventfd(10, 0);
    if(fd < 0)
    {
        perror("eventfd\n");
    }

    return fd;
}
