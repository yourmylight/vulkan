#ifndef BASE_H
#define BASE_H

#include <pch.h>

class Base {
public:
    Base() = default;
    virtual void prepare();
    void initVulkan();
    void setupWindow();
    virtual void run() = 0;
    virtual void renderLoop() = 0;
    virtual ~Base();
private:

protected:
    GLFWwindow* window = nullptr;
    int width = 800, height = 600;
};

#endif