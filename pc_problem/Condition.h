#ifndef _CONDITION_H_
#define _CONDITION_H_

#include "NonCopyable.h"
#include <pthread.h>

class MutexLock;

class Condition
: NonCopyable
{
public:
    Condition(MutexLock& mutex);
    ~Condition();
    void wait();
    void notify();
    void notifyAll();

private:
    MutexLock& _mutex;
    pthread_cond_t _cond;
};


#endif