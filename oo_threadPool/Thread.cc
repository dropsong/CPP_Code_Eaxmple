#include "Thread.h"
#include <stdio.h>

Thread::Thread()
: _thid(0)
, _isRunning(false)
{
}

Thread::~Thread()
{
    if(_isRunning)
    {
        // 让子线程与主线程分离，主线程不管子线程的回收
        pthread_detach(_thid);
        _isRunning = false;
    }
}

// 线程，启动！
void Thread::start()
{
    int ret = pthread_create(&_thid, nullptr, threadFunc, this);
    if(ret)
    {
        perror("pthread_create\n");
        return;
    }
    _isRunning = true;
}

void Thread::join()
{
    if(_isRunning)
    {
        int ret = pthread_join(_thid, nullptr);
        if(ret)
        {
            perror("pthread_join\n");
            return;
        }
        _isRunning = false;
    }
}

void* Thread::threadFunc(void* arg)
{
    Thread* pth = static_cast<Thread*>(arg);
    if(pth)
    {
        // threadFunc 是 static 的，没有 this 指针
        // 故在 pthread_create 中将 this 传进来
        pth->run(); // 体现多态
    }
    // 让子线程主动退出
    pthread_exit(nullptr);
}