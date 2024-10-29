#include "ThreadPool.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>

using std::cout;
using std::endl;

class Mytask
: public Task
{
public:
    void process() override
    {
        // 此处根据需要写具体的逻辑
        ::srand(::clock());
        int number = ::rand()%100;
        cout << "number = " << number << endl;
    }
};

int main()
{
    unique_ptr<Task> ptask(new Mytask());

    ThreadPool pool(4, 10);
    pool.start();

    int t = 20;
    while(t--){
        pool.addTask(ptask.get());
        cout << "t = " << t <<endl;
    }

    pool.stop();
    return 0;
}