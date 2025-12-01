#pragma once

namespace core {
    class Instance;
    class Surface;
    class Device;
    class Swapchain;
    class ImageView;
}

struct GLFWwindow;

class Base {
public:
    using Instance  = core::Instance;
    using Surface   = core::Surface;
    using Device    = core::Device;
    using Swapchain = core::Swapchain;
    using ImageView = core::ImageView;
    
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
    void createSwapchain();
    void createSwapchainImageView();
    void createPipeline();
protected:
    GLFWwindow* window = nullptr;
    int width = 800, height = 600;

    std::unique_ptr<Instance> instance;
    std::unique_ptr<Surface> surface;
    std::unique_ptr<Device> device;
    std::unique_ptr<Swapchain> swapchain;
    std::vector<std::unique_ptr<ImageView>> swapchainImageViews;
private:
    VkDebugUtilsMessengerEXT debugMessenger{};
};