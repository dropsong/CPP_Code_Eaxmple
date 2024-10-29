#ifndef _TASKQUEUE_H_
#define _TASKQUEUE_H_

#include "MutexLock.h"
#include "Condition.h"
#include <queue>

using std::queue;

class TaskQueue
{
public:
    TaskQueue(size_t queSize);
    ~TaskQueue();
    bool empty() const;
    bool full() const;
    void push(const int& value);
    int pop();

private:
    size_t _queSize;
    queue<int> _que;
    MutexLock _mutex;
    Condition _notEmpty;
    Condition _notFull;
};

#endif