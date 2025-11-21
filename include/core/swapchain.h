#pragma once

#include <vulkan/vulkan.h>
#include "core/queueFamilyIndices.h"

struct SwapchainSupportDetails;

struct GLFWwindow;

namespace core {
    class Swapchain {
    public:
        Swapchain(
            GLFWwindow* _window,
            VkPhysicalDevice _physicalDevice, 
            VkDevice _logicalDevice, 
            VkSurfaceKHR _surface,
            const QueueFamilyIndices& _queueFamilyIndices);
        ~Swapchain();
        VkSwapchainKHR getSwapchain() const;
    private:
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    private:
        GLFWwindow* window;
        VkPhysicalDevice physicalDevice{};
        VkDevice logicalDevice{};
        VkSurfaceKHR surface{};
        QueueFamilyIndices queueFamilyIndices;
        VkSwapchainKHR swapchain{};
    };
}