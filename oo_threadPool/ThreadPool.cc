#include "ThreadPool.h"
#include "WorkThread.h"
#include "Thread.h"
#include <unistd.h>

ThreadPool::ThreadPool(size_t threadNum, size_t queSize)
: _threadNum(threadNum)
, _queSize(queSize)
, _taskQue(_queSize)
, _isExit(false)
{
    // 提前预留空间
    _threads.reserve(_threadNum);
}

ThreadPool::~ThreadPool()
{
    if(!_isExit)
    {
        // 让工作线程退出
        stop();
    }
}

void ThreadPool::start()
{
    // 将所有的工作线程存放在 vector
    for(size_t idx = 0; idx != _threadNum; ++idx)
    {
        unique_ptr<Thread> up(new WorkThread(*this));
        _threads.push_back(std::move(up)); // 不能传左值
    }

    for(auto &th : _threads)
    {
        // 让每个线程跑起来
        th->start();
    }
}

void ThreadPool::stop()
{
    // 只要任务队列中有数据，就不该让工作线程退出
    while(!_taskQue.empty())
    {
        sleep(0.5);
    }

    _isExit = true;
    _taskQue.notify_notEmpty_all();
    for(auto &th : _threads)
    {
        th->join();
    }
}

void ThreadPool::addTask(Task* ptask)
{
    if(ptask)
    {
        _taskQue.push(ptask);
    }
}

Task* ThreadPool::getTask()
{
    return _taskQue.pop();
}

void ThreadPool::threadFunc()
{
    // 只要任务队列中有任务...
    while (!_isExit)
    {
        Task* ptask = getTask(); // 此处调用了pop, wait
        if(ptask)
        {
            ptask->process(); // 会发生多态
            // sleep(1);  // bad practice
        }
    }
    /*
    如果 process 执行地非常快，_isExit 就没有足够时间
    设置为 true，就会进入 while，若此时又没有任务，getTask 
    就会调用 _taskQue.pop(), 会卡在 _notEmpty.wait()
    程序就卡死了
    */
}