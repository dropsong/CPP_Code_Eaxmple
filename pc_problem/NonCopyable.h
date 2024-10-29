#ifndef _NONCOPYABLE_H_
#define _NONCOPYABLE_H_

class NonCopyable
{
public:
    NonCopyable() {}
    ~NonCopyable() {}

    NonCopyable(const NonCopyable& rhs) = delete;
    NonCopyable& operator = (const NonCopyable& rhs) = delete;
};

#endif