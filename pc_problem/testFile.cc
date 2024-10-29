#include "TaskQueue.h"
#include "Producer.h"
#include "Consumer.h"
#include <memory>

using std::unique_ptr;

void test()
{
    TaskQueue task(10);
    Producer pro(task);
    Consumer con(task);

    pro.start();
    con.start();

    pro.join();
    con.join();
}

void test2()
{
    TaskQueue task(10);
    unique_ptr<Thread> pro(new Producer(task));
    unique_ptr<Thread> con(new Consumer(task));

    pro->start();
    con->start();

    pro->join();
    con->join();
}

int main()
{
    test();
    return 0;
}

// 某一次的运行结果

/*
>>Producer produce: 39
>>Producer produce: 71
>>Producer produce: 55
>>Producer produce: 9
>>Producer produce: 94
>>Producer produce: 0
>>Producer produce: 19
>>Producer produce: 71
>>Producer produce: 59
>>Producer produce: 90
>>Consumer consume: >>Producer produce: 4
39
>>Consumer consume: 71
>>Producer produce: 97
>>Producer produce: 0
>>Consumer consume: 55
>>Consumer consume: 9
>>Consumer consume: 94
>>Consumer consume: 0
>>Consumer consume: 19
>>Producer produce: >>Consumer consume: 5771
>>Consumer consume: 59
>>Consumer consume: 90
>>Consumer consume: 4
>>Consumer consume: 97
>>Consumer consume: 0
>>Consumer consume: 57

>>Producer produce: 23
>>Producer produce: 85
>>Producer produce: 95
>>Consumer consume: >>Producer produce: 29
23>>Producer produce: 
8>>Consumer consume: 85
>>Consumer consume: 95
>>Consumer consume: 29
>>Consumer consume: 8

>>Producer produce: 54
>>Consumer consume: 54
*/

/*
可以看到两线程交替运行
空行是之前还没来得及输出的换行
cout 不是原子操作
*/

// 类图设计的思路可以多回顾 D31_4