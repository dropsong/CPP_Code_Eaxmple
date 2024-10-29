#ifndef _THREADPOOL_H_
#define _THREADPOOL_H_

#include "TaskQueue.h"
#include <vector>
#include <memory>
#include <functional>

using std::vector;
using std::unique_ptr;
using std::bind;
using std::function;

class Thread;
using Task = function<void()>;
class ThreadPool
{
public:
    ThreadPool(size_t threadNum, size_t queSize);
    ~ThreadPool();

    void start();
    void stop();

    void addTask(Task&& taskcb);

private:
    size_t _threadNum;
    size_t _queSize; // 任务队列的大小
    vector<unique_ptr<Thread>> _threads; // 存放工作线程的容器
    TaskQueue _taskQue; // 任务队列子对象，存放任务
    bool _isExit; // 线程池退出的标志

    Task getTask();
    void threadFunc();
};

#endif