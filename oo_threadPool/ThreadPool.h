#ifndef _THREADPOOL_H_
#define _THREADPOOL_H_

#include "TaskQueue.h"
#include <vector>
#include <memory>

using std::vector;
using std::unique_ptr;

class Thread;
class ThreadPool
{
    friend class WorkThread;
public:
    ThreadPool(size_t threadNum, size_t queSize);
    ~ThreadPool();

    void start();
    void stop();

    void addTask(Task* ptask);

private:
    size_t _threadNum;
    size_t _queSize; // 任务队列的大小
    vector<unique_ptr<Thread>> _threads; // 存放工作线程的容器
    TaskQueue _taskQue; // 任务队列子对象，存放任务
    bool _isExit; // 线程池退出的标志

    Task* getTask();
    void threadFunc();
};

#endif