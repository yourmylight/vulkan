#include <base.h>
#include <config.h>
#include <debugUtils.h>

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
        DebugUtils::DestroyDebugUtilsMessengerEXT(instance->getInstance(), debugMessenger, nullptr);
    }
    surface.reset();
    instance.reset();
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
    instance = std::make_unique<Instance>();
}

void Base::setupDebugMessenger() {
    if (!enableValidationLayers) return;
    VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo{};
    DebugUtils::populateDebugMessengerCreateInfo(debugMessengerCreateInfo);

    if (DebugUtils::CreateDebugUtilsMessengerEXT(
        instance->getInstance(),
        &debugMessengerCreateInfo,
        nullptr,
        &debugMessenger) != VK_SUCCESS) {
        throw std::runtime_error("failed to create debug messenger");
    }
}

void Base::createSurface() {
    surface = std::make_unique<Surface>(instance->getInstance(), window);
}

void Base::pickPhysicalDevice() {
    uint32_t physicalDeviceCount = 0;
    if (vkEnumeratePhysicalDevices(instance->getInstance(), &physicalDeviceCount, nullptr) != VK_SUCCESS) {
        throw std::runtime_error("failed to enumerate physical device");
    }
    if (!physicalDeviceCount) {
        throw std::runtime_error("failed to find gpu with Vulkan support");
    }
    std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
    if (vkEnumeratePhysicalDevices(instance->getInstance(), &physicalDeviceCount, physicalDevices.data()) != VK_SUCCESS) {
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

    std::vector<float> priorities(uniqueQueueFamilyIndices.size(), 1.0f);
    uint32_t i = 0;
    for (const uint32_t index : uniqueQueueFamilyIndices) {
        deviceQueueCreateInfos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        deviceQueueCreateInfos[i].pNext = nullptr;
        deviceQueueCreateInfos[i].queueFamilyIndex = index;
        deviceQueueCreateInfos[i].queueCount = 1;
        deviceQueueCreateInfos[i].pQueuePriorities = &priorities[i];
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
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface->getSurface(), &supported);
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