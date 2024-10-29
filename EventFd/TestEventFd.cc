#include "EventFd.h"
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
    EventFd evfd(std::bind(&MyTask::process, &task));

    Thread th(std::bind(&EventFd::start, &evfd));
    th.start();

    int cnt = 20;
    while(cnt-- > 0)
    {
        cout << "cnt = " << cnt << endl;
        //让主线程去唤醒Thread线程，也就是子线程
        evfd.wakeup();
        sleep(1);
    }

    evfd.stop();
    th.join();

    return 0;
}

/*
cnt = 19
nready = 1
MyTask is running...

cnt = 18
nready = 1
MyTask is running...

cnt = 17
nready = 1
MyTask is running...

cnt = 16
nready = 1
MyTask is running...

cnt = 15
nready = 1
MyTask is running...

cnt = 14
nready = 1
MyTask is running...

cnt = 13
nready = 1
MyTask is running...

cnt = 12
nready = 1
MyTask is running...

cnt = 11
nready = 1
MyTask is running...

cnt = 10
nready = 1
MyTask is running...

cnt = 9
nready = 1
MyTask is running...

cnt = 8
nready = 1
MyTask is running...

cnt = 7
nready = 1
MyTask is running...

cnt = 6
nready = 1
MyTask is running...

cnt = 5
nready = 1
MyTask is running...

cnt = 4
nready = 1
MyTask is running...

cnt = 3
nready = 1
MyTask is running...

cnt = 2
nready = 1
MyTask is running...

cnt = 1
nready = 1
MyTask is running...

cnt = 0
nready = 1
MyTask is running...

nready = 0
>>epoll_wait timeout
*/