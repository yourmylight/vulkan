#pragma once

#include <instance.h>

class Base {
public:
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() {
            return graphicFamily.has_value() && presentFamily.has_value();
        }
    };
public:
    Base(int _width = 800, int _height = 600);

    virtual ~Base();

    void run();

    virtual void renderLoop() = 0;

    virtual void prepare();

    void setupWindow();

    void initVulkan();

    void createInstance();

    void createSurface();

    void setupDebugMessenger();

    void pickPhysicalDevice();

    void createLogicDevice();

    bool checkPhysicalDevice(const VkPhysicalDevice& device);

    QueueFamilyIndices findQueueFamily(const VkPhysicalDevice& device);
protected:
    GLFWwindow* window = nullptr;
    int width = 800, height = 600;

    // VkInstance instance{};
    // uint32_t targetVersion = VK_API_VERSION_1_1;

    std::unique_ptr<Instance> instance;

    VkSurfaceKHR surface{};

    VkPhysicalDevice physicalDevice{};

    VkDevice logicalDevice{};

    VkQueue graphicQueue{};
    VkQueue presentQueue{};
private:
    VkDebugUtilsMessengerEXT debugMessenger{};
};