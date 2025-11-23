#pragma once

#include <vulkan/vulkan.h>
#include "common/queueFamilyIndices.h"

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
        const VkFormat& getSwapchainImageFormat() const;
        const size_t getSwapchainImageCount() const;
        const std::vector<VkImage>& getImages() const;
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
        std::vector<VkImage> swapchainImages;
        VkFormat swapchainImageFormat;
        VkExtent2D swapchainExtent;
    };
}