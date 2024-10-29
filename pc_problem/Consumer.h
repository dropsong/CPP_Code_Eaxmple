#ifndef _CONSUMER_H_
#define _CONSUMER_H_

#include "Thread.h"
#include "TaskQueue.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>

using std::cout;
using std::endl;

class Consumer
: public Thread
{
public:
    Consumer(TaskQueue& taskQue)
    : _taskQue(taskQue)
    {}

    ~Consumer()
    {}

    void run() override
    {
        size_t cnt = 20;
        while(cnt--)
        {
            int num = _taskQue.pop();
            cout << ">>Consumer consume: " << num << endl;
        }
    }

private:
    TaskQueue& _taskQue;
};

#endif