#ifndef __THREAD_H__
#define __THREAD_H__

#include <pthread.h>
#include <functional>

using std::function;

class Thread {
    using ThreadCallBack = function<void()>;
public:
    Thread(ThreadCallBack&& cb); // 移动语义是为了提高效率
    ~Thread();
    void start();
    void join();

private:
    static void* threadFunc(void* arg); // 线程的入口函数

    pthread_t _thid;     // 线程 id
    bool _isRunning;     // 线程运行的标志位
    ThreadCallBack _cb;  // 封装的任务
};

#endif