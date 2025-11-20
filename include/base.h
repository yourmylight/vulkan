#pragma once

#include <pch.h>

#include <core/instance.h>
#include <core/surface.h>
#include <core/device.h>

class Base {
public:
    Base(int _width = 800, int _height = 600);

    virtual ~Base();

    void run();

    virtual void renderLoop() = 0;

    virtual void prepare();

    void setupWindow();

    void initVulkan();

    void createInstance();

    void setupDebugMessenger();

    void createSurface();

    void createDevice();
protected:
    GLFWwindow* window = nullptr;
    int width = 800, height = 600;

    std::unique_ptr<Instance> instance;

    std::unique_ptr<Surface> surface;

    std::unique_ptr<Device> device;
private:
    VkDebugUtilsMessengerEXT debugMessenger{};
};