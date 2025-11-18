#ifndef BASE_H
#define BASE_H

#include <pch.h>

class Base {
public:
    Base(int _width = 800, int _height = 600);
    virtual void prepare();
    void initVulkan();
    void setupWindow();
    void createInstance();
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
    void setupDebugMessenger();
    void run();
    virtual void renderLoop() = 0;
    virtual ~Base();
private:
    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };
protected:
    GLFWwindow* window = nullptr;
    int width = 800, height = 600;
    VkInstance instance = VK_NULL_HANDLE;
    uint32_t targetVersion = VK_API_VERSION_1_0;
private:
#ifdef NDEBUG
    constexpr static bool enableValidationLayers = false;
#else
    constexpr static bool enableValidationLayers = true;
#endif
    VkDebugUtilsMessengerEXT debugMessenger{};
};

#endif