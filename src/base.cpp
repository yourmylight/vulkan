#include <base.h>

void Base::prepare() {
    
}

void Base::initVulkan() {
    createInstance();
}

void Base::createInstance() {
    uint32_t supportVersion = VK_API_VERSION_1_0;
    if (vkEnumerateInstanceVersion(&supportVersion) != VK_SUCCESS) {
        throw std::runtime_error("failed enumerate vulkan instance version");
    }

    targetVersion = std::min(targetVersion, supportVersion);

    VkApplicationInfo applicationInfo{};
    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.pNext = nullptr;
    applicationInfo.pApplicationName = "Vulkan";
    applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    applicationInfo.pEngineName = nullptr;
    applicationInfo.apiVersion = targetVersion;

    uint32_t extensionCount = 0;
    const char** extensionNames = glfwGetRequiredInstanceExtensions(&extensionCount);
    if (extensionNames == nullptr && extensionCount > 0) {
        throw std::runtime_error("failed glfw get required extensions");
    }

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.pApplicationInfo = &applicationInfo;
    createInfo.enabledExtensionCount = extensionCount;
    createInfo.ppEnabledExtensionNames = extensionNames;
    createInfo.enabledLayerCount = 0;

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance");
    }
}

void Base::setupWindow() {
    if (!glfwInit()) {
        throw std::runtime_error("failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(width, height, "Hello, Vulkan", nullptr, nullptr);
    if (window == nullptr) {
        glfwTerminate();
        throw std::runtime_error("failed to create GLFW window");
    }
}

Base::Base(int _width, int _height) : width(_width), height(_height) {

}

Base::~Base() {
    if (instance != VK_NULL_HANDLE) {
        vkDestroyInstance(instance, nullptr);
    }

    if (window != nullptr) {
        glfwDestroyWindow(window);
    }
    
    glfwTerminate();
}