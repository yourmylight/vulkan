#include "core/swapchain.h"
#include "core/device.h"
#include "common/swapchainSupportDetails.h"

namespace core {
    Swapchain::Swapchain(
        GLFWwindow* _window, VkPhysicalDevice _physicalDevice, 
        VkDevice _logicalDevice, VkSurfaceKHR _surface, const common::QueueFamilyIndices& _queueFamilyIndices) 
        : window(_window), physicalDevice(_physicalDevice), 
        logicalDevice(_logicalDevice), surface(_surface), queueFamilyIndices(_queueFamilyIndices)
    {
        common::SwapchainSupportDetails swapchainSupportDetails = 
            common::SwapchainSupportDetails::querySwapchainSupport(physicalDevice ,surface);
        
        VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapchainSupportDetails.formats);
        VkPresentModeKHR presentMode = chooseSwapPresentMode(swapchainSupportDetails.presentModes);
        VkExtent2D extent = chooseSwapExtent(swapchainSupportDetails.capabilities);

        uint32_t imageCount = swapchainSupportDetails.capabilities.minImageCount + 1;

        if (swapchainSupportDetails.capabilities.maxImageCount > 0 && 
            imageCount > swapchainSupportDetails.capabilities.maxImageCount) 
        {
            imageCount = swapchainSupportDetails.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR swapchainCreateInfo {
            .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            .pNext = nullptr,
            .surface = surface,
            .minImageCount = imageCount,
            .imageFormat = surfaceFormat.format,
            .imageColorSpace = surfaceFormat.colorSpace,
            .imageExtent = extent,
            .imageArrayLayers = 1,
            .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
        };
        
        uint32_t indices[] = {queueFamilyIndices.graphicFamily.value(), queueFamilyIndices.presentFamily.value()};

        if (queueFamilyIndices.graphicFamily != queueFamilyIndices.presentFamily) {
            swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            swapchainCreateInfo.queueFamilyIndexCount = sizeof(indices) / sizeof(uint32_t);
            swapchainCreateInfo.pQueueFamilyIndices = indices;
        } else {
            swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            swapchainCreateInfo.queueFamilyIndexCount = 0;
            swapchainCreateInfo.pQueueFamilyIndices = nullptr;
        }

        swapchainCreateInfo.preTransform = swapchainSupportDetails.capabilities.currentTransform;
        swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        swapchainCreateInfo.presentMode = presentMode;
        swapchainCreateInfo.clipped = VK_TRUE;
        swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

        if (vkCreateSwapchainKHR(logicalDevice, &swapchainCreateInfo, nullptr, &swapchain) != VK_SUCCESS) {
            throw std::runtime_error("failed to create swap chain!");
        }

        uint32_t swapchainImageCount = 0;
        vkGetSwapchainImagesKHR(logicalDevice, swapchain, &swapchainImageCount, nullptr);
        if (swapchainImageCount) {
            swapchainImages.resize(swapchainImageCount);
            vkGetSwapchainImagesKHR(logicalDevice, swapchain, &swapchainImageCount, swapchainImages.data());
        }

        swapchainImageFormat = surfaceFormat.format;
        swapchainExtent = extent;
    }

    Swapchain::~Swapchain() {
        if (swapchain != VK_NULL_HANDLE && logicalDevice != VK_NULL_HANDLE) {
            vkDestroySwapchainKHR(logicalDevice, swapchain, nullptr);
        }
    }

    VkSwapchainKHR Swapchain::getSwapchain() const {
        return swapchain;
    }

    VkSurfaceFormatKHR Swapchain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
        for (const auto& availableFormat : availableFormats) {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && 
                availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return availableFormat;
            }
        }

        return availableFormats[0];
    }

    VkPresentModeKHR Swapchain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
        for (const auto& availablePresentMode : availablePresentModes) {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return availablePresentMode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D Swapchain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
            return capabilities.currentExtent;
        } else {
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);

            VkExtent2D actualExtent = {
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)
            };

            actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

            return actualExtent;
        }
    }

    const VkFormat& Swapchain::getSwapchainImageFormat() const {
        return swapchainImageFormat;
    }

    const size_t Swapchain::getSwapchainImageCount() const {
        return swapchainImages.size();
    }

    const std::vector<VkImage>& Swapchain::getImages() const {
        return swapchainImages;
    }
}