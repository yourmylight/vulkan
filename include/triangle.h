#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <base.h>

class Triangle : public Base {
public:
    Triangle() = default;
    void run() override;
    void prepare() override;
    void renderLoop() override;
    virtual ~Triangle();
};

#endif