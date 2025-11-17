#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <base.h>

class Triangle : public Base {
public:
    Triangle(int _width = 800, int _height = 600);
    void prepare() override;
    void renderLoop() override;
    virtual ~Triangle() override;
};

#endif