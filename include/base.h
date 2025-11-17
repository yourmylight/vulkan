#ifndef BASE_H
#define BASE_H

#include <pch.h>

class Base {
public:
    Base() = default;
    Base(int _width, int _height);
    virtual void prepare();
    void initVulkan();
    void setupWindow();
    void createInstance();
    virtual void run() = 0;
    virtual void renderLoop() = 0;
    virtual ~Base();
protected:
    GLFWwindow* window = nullptr;
    int width = 800, height = 600;
    VkInstance instance = VK_NULL_HANDLE;
    uint32_t targetVersion = VK_API_VERSION_1_0;
};

#endif