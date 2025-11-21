#pragma once

#include <vulkan/vulkan.h>

namespace core {
    class Device {
    public:
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() {
            return graphicFamily.has_value() && presentFamily.has_value();
        }
    };
    public:
        Device(VkInstance _instance, VkSurfaceKHR _surface);

        ~Device();

        VkPhysicalDevice getPhysicalDevice() const;

        VkDevice getLogicalDevice() const;
    private:
        void pickPhysicalDevice();

        void createLogicDevice();

        bool checkPhysicalDevice(const VkPhysicalDevice& device);

        QueueFamilyIndices findQueueFamily(const VkPhysicalDevice& device);
    private:
        VkInstance instance{};
        VkSurfaceKHR surface{};
        VkPhysicalDevice physicalDevice{};
        VkDevice logicalDevice{};
        VkQueue graphicQueue{};
        VkQueue presentQueue{};
    };
}