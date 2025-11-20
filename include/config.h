#pragma once

#include <vector>

#ifdef NDEBUG
    constexpr static bool enableValidationLayers = false;
#else
    constexpr static bool enableValidationLayers = true;
#endif

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};
const std::vector<const char*> deviceExtensions = {
    "VK_KHR_swapchain"
};