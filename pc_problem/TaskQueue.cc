#include "TaskQueue.h"

TaskQueue::TaskQueue(size_t queSize)
: _queSize(queSize)
, _que()
, _mutex()
, _notEmpty(_mutex)
, _notFull(_mutex)  // 注意要按照顺序初始化
{
}

// _queSize 和 _que 不用管
// _mutex, _notEmpty, _notFull 生命周期结束自动执行各自的析构函数
TaskQueue::~TaskQueue() = default;

bool TaskQueue::empty() const
{
    return 0 == _que.size();
}

bool TaskQueue::full() const
{
    return _que.size() == _queSize;
    // 这里的 _queSize 其实是指队列的最大容量
}

void TaskQueue::push(const int& value)
{
    // RAII 的思想：
    // 在构造函数中初始化资源，析构函数中释放
    // 实质：用栈对象的生命周期管理资源
    MutexLockGuard autolock(_mutex);

    while(full())
    {
        _notFull.wait();
        // 这个变量命名有点奇怪，可以这样理解：
        // 等待不满的时候
    }

    _que.push(value);
    _notEmpty.notify(); // 唤醒消费者
}
    
int TaskQueue::pop()
{
    MutexLockGuard autolock(_mutex);

    while(empty())
    {
        _notEmpty.wait();
    }

    int tmp = _que.front();
    _que.pop();
    _notFull.notify(); // 唤醒生产者
    return tmp;
}