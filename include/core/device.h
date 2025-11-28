#pragma once

#include "vulkanHeaders.h"
#include "common/queueFamilyIndices.h"

namespace core {
    class Device {    
    public:
        Device(VkInstance _instance, VkSurfaceKHR _surface);

        ~Device();

        VkPhysicalDevice getPhysicalDevice() const;

        VkDevice getLogicalDevice() const;
        const common::QueueFamilyIndices& getQueueFamilyIndices() const;
    private:
        void pickPhysicalDevice();

        void createLogicDevice();

        bool checkPhysicalDevice(const VkPhysicalDevice& device);

        common::QueueFamilyIndices findQueueFamily(const VkPhysicalDevice& device);

        bool checkDeviceExtensionSupport(const VkPhysicalDevice& device);
    private:
        VkInstance instance{};
        VkSurfaceKHR surface{};
        VkPhysicalDevice physicalDevice{};
        VkDevice logicalDevice{};
        VkQueue graphicQueue{};
        VkQueue presentQueue{};
        common::QueueFamilyIndices queueFamilyIndices{};
    };
}