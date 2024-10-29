#ifndef _PRODUCER_H_
#define _PRODUCER_H_

#include "Thread.h"
#include "TaskQueue.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>

using std::cout;
using std::endl;

class Producer
: public Thread
{
public:
    Producer(TaskQueue& taskQue)
    : _taskQue(taskQue)
    {}

    ~Producer()
    {}

    void run() override
    {
        ::srand(::clock());
        size_t cnt = 20;
        while(cnt--)
        {
            int number = ::rand()%100;
            _taskQue.push(number);
            cout << ">>Producer produce: " << number << endl;
        }
    }

private:
    TaskQueue& _taskQue;
};

#endif