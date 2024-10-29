#include "TimerFd.h"
#include "Thread.h"
#include <unistd.h>
#include <iostream>

using std::cout;
using std::endl;

class MyTask
{
public:
    void process()
    {
        cout << "MyTask is running..." << endl << endl;
    }
};

int main(int argc, char **argv)
{
    MyTask task;
    TimerFd tfd(1, 4, std::bind(&MyTask::process, &task));

    Thread th(std::bind(&TimerFd::start, &tfd));
    th.start();

    sleep(30);

    tfd.stop();
    th.join();

    return 0;
}
/*
nready = 1
MyTask is running...

nready = 1
MyTask is running...

nready = 1
MyTask is running...

nready = 1
MyTask is running...

nready = 1
MyTask is running...

nready = 1
MyTask is running...

nready = 1
MyTask is running...

nready = 1
MyTask is running...

nready = 0
>>poll timeout
*/