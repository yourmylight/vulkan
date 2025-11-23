#include "pch.h"
#include "core/device.h"
#include "common/swapchainSupportDetails.h"
#include "config.h"

namespace core {
    Device::Device(VkInstance _instance, VkSurfaceKHR _surface) :
        instance(_instance), surface(_surface)
    {
        pickPhysicalDevice();
        createLogicDevice();
    }

    Device::~Device() {
        if (logicalDevice != VK_NULL_HANDLE) {
            vkDestroyDevice(logicalDevice, nullptr);
        }
    }

    VkPhysicalDevice Device::getPhysicalDevice() const {
        return physicalDevice;
    }

    VkDevice Device::getLogicalDevice() const {
        return logicalDevice;
    }

    void Device::pickPhysicalDevice() {
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

    void Device::createLogicDevice() {
        common::QueueFamilyIndices queueFamilyIndices = findQueueFamily(physicalDevice);

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
            .pQueueCreateInfos = deviceQueueCreateInfos.data(),
            .enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size()),
            .ppEnabledExtensionNames = deviceExtensions.data()
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

    bool Device::checkPhysicalDevice(const VkPhysicalDevice& device) {
        common::QueueFamilyIndices queueFamilyIndices = findQueueFamily(device);

        bool extensionSupport = checkDeviceExtensionSupport(device);
        bool swapchainAdequate = false;
        if (extensionSupport) {
            common::SwapchainSupportDetails swapchainSupportDetails = 
                common::SwapchainSupportDetails::querySwapchainSupport(device, surface);
            swapchainAdequate = 
                !swapchainSupportDetails.formats.empty() && 
                !swapchainSupportDetails.presentModes.empty();
        }

        return queueFamilyIndices.isComplete() && extensionSupport && swapchainAdequate;
    }

    common::QueueFamilyIndices Device::findQueueFamily(const VkPhysicalDevice& device) {
        if (this->queueFamilyIndices.isComplete()) {
            return this->queueFamilyIndices;
        }
        common::QueueFamilyIndices queueFamilyIndices{};

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
        this->queueFamilyIndices = queueFamilyIndices;
        return queueFamilyIndices;
    }

    bool Device::checkDeviceExtensionSupport(const VkPhysicalDevice& device) {
        uint32_t propertyCount = 0;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &propertyCount, nullptr);
        if (propertyCount) {
            std::vector<VkExtensionProperties> extensionProperties(propertyCount);
            vkEnumerateDeviceExtensionProperties(device, nullptr, &propertyCount, extensionProperties.data());
            bool support = true;
            for (const char* extension : deviceExtensions) {
                auto it = find_if(extensionProperties.begin(), extensionProperties.end(),
                [&extension](const VkExtensionProperties& extensionProperty) {
                    return strcmp(extension, extensionProperty.extensionName) == 0;
                });
                if (it == extensionProperties.end()) {
                    support = false;
                    break;
                }
            }
            return support;
        }
        return deviceExtensions.empty();
    }

    const common::QueueFamilyIndices& Device::getQueueFamilyIndices() const {
        return queueFamilyIndices;
    }
}