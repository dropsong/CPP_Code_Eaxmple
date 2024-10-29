#ifndef _MUTEXLOCK_H_
#define _MUTEXLOCK_H_

#include "NonCopyable.h"
#include <pthread.h>

class MutexLock
: NonCopyable
{
public:
    MutexLock();
    ~MutexLock();
    void lock();
    void trylock();
    void unlock();

    pthread_mutex_t* get_mutex_ptr()
    {
        return &_mutex;
    }

private:
    pthread_mutex_t _mutex;
};

// 利用 RAII 的思想
class MutexLockGuard
{
public:
    MutexLockGuard(MutexLock& mutex)
    : _mutex(mutex)
    {
        _mutex.lock();
    }

    ~MutexLockGuard()
    {
        _mutex.unlock();
    }

private:
    MutexLock& _mutex;
};

#endif