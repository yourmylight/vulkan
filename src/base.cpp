#include <base.h>

void Base::run() {
    setupWindow();
    initVulkan();
    prepare();
    renderLoop();
}

Base::Base(int _width, int _height) : width(_width), height(_height) {

}

Base::~Base() {
    if (logicalDevice != VK_NULL_HANDLE) {
        vkDestroyDevice(logicalDevice, nullptr);
    }
    if (enableValidationLayers && debugMessenger != VK_NULL_HANDLE) {
        DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
    }
    if (surface != VK_NULL_HANDLE) {
        vkDestroySurfaceKHR(instance, surface, nullptr);
    }
    if (instance != VK_NULL_HANDLE) {
        vkDestroyInstance(instance, nullptr);
    }
    if (window != nullptr) {
        glfwDestroyWindow(window);
    }
    
    glfwTerminate();
}

void Base::prepare() {

}

void Base::initVulkan() {
    createInstance();
    setupDebugMessenger();
    createSurface();
    pickPhysicalDevice();
    createLogicDevice();
}

void Base::setupWindow() {
    if (!glfwInit()) {
        throw std::runtime_error("failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(width, height, "Vulkan Engine", nullptr, nullptr);
    if (window == nullptr) {
        throw std::runtime_error("failed to create GLFW window");
    }
}

void Base::createInstance() {
    uint32_t supportVersion = VK_API_VERSION_1_1;
    if (vkEnumerateInstanceVersion(&supportVersion) != VK_SUCCESS) {
        throw std::runtime_error("failed to enumerate vulkan instance version");
    }

    targetVersion = std::min(targetVersion, supportVersion);

    VkApplicationInfo applicationInfo {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = nullptr,
        .pApplicationName = "Vulkan",
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName = nullptr,
        .apiVersion = targetVersion
    };

    uint32_t extensionCount = 0;
    const char** extensionNames = glfwGetRequiredInstanceExtensions(&extensionCount);
    if (extensionNames == nullptr && extensionCount > 0) {
        throw std::runtime_error("failed to get glfw required extensions");
    }
    std::vector<const char*> extensions(extensionNames, extensionNames + extensionCount);
    if (enableValidationLayers) {
        extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    if (enableValidationLayers) {
        uint32_t propertyCount = 0;
        vkEnumerateInstanceLayerProperties(&propertyCount, nullptr);
        bool support = true;
        if (propertyCount) {
            std::vector<VkLayerProperties> properties(propertyCount);
            vkEnumerateInstanceLayerProperties(&propertyCount, properties.data());
            for (const auto& validationLayer : validationLayers) {
                auto it = std::find_if(properties.begin(), properties.end(), 
                [&validationLayer](const VkLayerProperties& property) {
                    return strcmp(validationLayer, property.layerName) == 0;
                });
                if (it == properties.end()) {
                    support = false;
                    break;
                }
            }
        } else {
            support = false;
        }

        if (!support) {
            throw std::runtime_error("failed to support validation layers");
        }
    }

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};

    VkInstanceCreateInfo createInfo {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = nullptr,
        .pApplicationInfo = &applicationInfo,
        .enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
        .ppEnabledExtensionNames = extensions.data()
    };
    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = static_cast<VkDebugUtilsMessengerCreateInfoEXT*>(&debugCreateInfo);
    } else {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance");
    }
}

void Base::setupDebugMessenger() {
    if (!enableValidationLayers) return;

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(createInfo);

    if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
        throw std::runtime_error("failed to create debug messenger");
    }
}

void Base::createSurface() {
    if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface");
    }
}

void Base::pickPhysicalDevice() {
    uint32_t physicalDeviceCount = 0;
    if (vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr) != VK_SUCCESS) {
        throw std::runtime_error("failed to enumerate physical device");
    }
    if (!physicalDeviceCount) {
        throw std::runtime_error("failed to find gpu with Vulkan support");
    }
    std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
    if (vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to enumerate physical device");
    }
    for (const VkPhysicalDevice& device : physicalDevices) {
        if (checkPhysicalDevice(device)) {
            physicalDevice = device;
            break;
        }
    }
    if (physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("failed to pick physical device");
    }
}

void Base::createLogicDevice() {
    QueueFamilyIndices queueFamilyIndices = findQueueFamily(physicalDevice);

    std::set<uint32_t> uniqueQueueFamilyIndices {
        queueFamilyIndices.graphicFamily.value(),
        queueFamilyIndices.presentFamily.value()
    };

    std::vector<VkDeviceQueueCreateInfo> deviceQueueCreateInfos(uniqueQueueFamilyIndices.size());

    const float priority = 1.0f;
    uint32_t i = 0;
    for (const uint32_t index : uniqueQueueFamilyIndices) {
        deviceQueueCreateInfos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        deviceQueueCreateInfos[i].pNext = nullptr;
        deviceQueueCreateInfos[i].queueFamilyIndex = index;
        deviceQueueCreateInfos[i].queueCount = 1;
        deviceQueueCreateInfos[i].pQueuePriorities = &priority;
        i++;
    }

    VkDeviceCreateInfo deviceCreateInfo {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = nullptr,
        .queueCreateInfoCount = static_cast<uint32_t>(deviceQueueCreateInfos.size()),
        .pQueueCreateInfos = deviceQueueCreateInfos.data()
    };
    if (enableValidationLayers) {
        deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        deviceCreateInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        deviceCreateInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &logicalDevice) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device");
    }

    vkGetDeviceQueue(logicalDevice, queueFamilyIndices.graphicFamily.value(), 0, &graphicQueue);
    vkGetDeviceQueue(logicalDevice, queueFamilyIndices.presentFamily.value(), 0, &presentQueue);
}

bool Base::checkPhysicalDevice(const VkPhysicalDevice& device) {
    QueueFamilyIndices queueFamilyIndices = findQueueFamily(device);

    return queueFamilyIndices.isComplete();
}

Base::QueueFamilyIndices Base::findQueueFamily(const VkPhysicalDevice& device) {
    QueueFamilyIndices queueFamilyIndices{};

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties2(device, &queueFamilyCount, nullptr);
    if (queueFamilyCount > 0) {
        std::vector<VkQueueFamilyProperties2> queueFamilyProperties2(queueFamilyCount);
        for (uint32_t i = 0; i < queueFamilyCount; i++) {
            queueFamilyProperties2[i].sType = VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2;
        }
        vkGetPhysicalDeviceQueueFamilyProperties2(device, &queueFamilyCount, queueFamilyProperties2.data());

        VkBool32 supported = false; 
        for (uint32_t i = 0; i < queueFamilyCount; i++) {
            if (queueFamilyProperties2[i].queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                queueFamilyIndices.graphicFamily = i;
            }
            supported = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &supported);
            if (supported) {
                queueFamilyIndices.presentFamily = i;
            }
            if (queueFamilyIndices.isComplete()) {
                break;
            }
        }
    }

    return queueFamilyIndices;
}

VKAPI_ATTR VkBool32 VKAPI_CALL Base::debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) 
{
    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
    return VK_FALSE;
}

void Base::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = 
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | 
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | 
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = 
        VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | 
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | 
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
}

VkResult Base::CreateDebugUtilsMessengerEXT(
    VkInstance instance, 
    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
    const VkAllocationCallbacks* pAllocator, 
    VkDebugUtilsMessengerEXT* pDebugMessenger)
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(
        instance, 
        "vkCreateDebugUtilsMessengerEXT"
    );
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void Base::DestroyDebugUtilsMessengerEXT(
    VkInstance instance, 
    VkDebugUtilsMessengerEXT debugMessenger, 
    const VkAllocationCallbacks* pAllocator)
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}