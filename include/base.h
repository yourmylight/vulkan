#ifndef BASE_H
#define BASE_H

#include <pch.h>

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
    
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData
    );

    VkResult CreateDebugUtilsMessengerEXT(
        VkInstance instance, 
        const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
        const VkAllocationCallbacks* pAllocator, 
        VkDebugUtilsMessengerEXT* pDebugMessenger
    );

    void DestroyDebugUtilsMessengerEXT(
        VkInstance instance, 
        VkDebugUtilsMessengerEXT debugMessenger, 
        const VkAllocationCallbacks* pAllocator
    );

    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

    bool checkPhysicalDevice(const VkPhysicalDevice& device);

    QueueFamilyIndices findQueueFamily(const VkPhysicalDevice& device);
    
private:
    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };
protected:
    GLFWwindow* window = nullptr;
    int width = 800, height = 600;

    VkInstance instance{};
    uint32_t targetVersion = VK_API_VERSION_1_1;

    VkSurfaceKHR surface{};

    VkPhysicalDevice physicalDevice{};

    VkDevice logicalDevice{};

    VkQueue graphicQueue{};
    VkQueue presentQueue{};
private:
#ifdef NDEBUG
    constexpr static bool enableValidationLayers = false;
#else
    constexpr static bool enableValidationLayers = true;
#endif
    VkDebugUtilsMessengerEXT debugMessenger{};
};

#endif