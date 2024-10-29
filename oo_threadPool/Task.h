#ifndef _TASK_H_
#define _TASK_H_

class Task
{
public:
    Task() {}
    virtual ~Task() {}
    virtual void process() = 0;
};

#endif