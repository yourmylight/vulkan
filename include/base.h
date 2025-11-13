#ifndef BASE_H
#define BASE_H

#include <pch.h>

class Base {
public:
    Base() = default;
    void prepare();
    void initVulkan();
    virtual void run() = 0;
    virtual ~Base();
private:

};

#endif