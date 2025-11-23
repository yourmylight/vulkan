#pragma once

#include <vulkan/vulkan.h>

namespace common {
    struct SwapchainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;

        static SwapchainSupportDetails querySwapchainSupport(const VkPhysicalDevice& device, const VkSurfaceKHR& surface) {
            SwapchainSupportDetails swapchainSupportDetails{};

            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &swapchainSupportDetails.capabilities);

            uint32_t formatCount = 0;
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

            if (formatCount != 0) {
                swapchainSupportDetails.formats.resize(formatCount);
                vkGetPhysicalDeviceSurfaceFormatsKHR(
                    device, 
                    surface, 
                    &formatCount, 
                    swapchainSupportDetails.formats.data()
                );
            }

            uint32_t presentModeCount = 0;
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

            if (presentModeCount != 0) {
                swapchainSupportDetails.presentModes.resize(presentModeCount);
                vkGetPhysicalDeviceSurfacePresentModesKHR(
                    device, 
                    surface, 
                    &presentModeCount, 
                    swapchainSupportDetails.presentModes.data()
                );
            }
            return swapchainSupportDetails;
        }
    };
}