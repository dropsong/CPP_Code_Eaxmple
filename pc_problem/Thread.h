#ifndef __THREAD_H__
#define __THREAD_H__

#include "NonCopyable.h"
#include <pthread.h>

class Thread
: NonCopyable
{
public:
    Thread();
    virtual ~Thread();
    void start();
    void join();

private:
    static void* threadFunc(void* arg); // 线程的入口函数
    virtual void run() = 0;      // 线程的执行任务

    pthread_t _thid;  // 线程 id
    bool _isRunning; // 线程运行的标志位
};

#endif