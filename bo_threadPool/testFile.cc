#include "ThreadPool.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>

using std::cout;
using std::endl;

class Mytask
{
public:
    void process()
    {
        // 此处根据需要写具体的逻辑
        ::srand(::clock());
        int number = ::rand()%100;
        cout << "number = " << number << endl;
    }
};

int main()
{
    Mytask task;
    ThreadPool pool(4, 10);
    pool.start();

    int t = 20;
    while(t--){
        pool.addTask(std::bind(&Mytask::process, &task));
        cout << "t = " << t <<endl;
    }

    pool.stop();
    return 0;
}