#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <base.h>

class Triangle : public Base {
public:
    Triangle() = default;
    void prepare() override;
    void renderLoop() override;
    virtual ~Triangle() override;
};

#endif