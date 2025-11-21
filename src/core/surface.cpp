#include "pch.h"
#include "core/surface.h"

namespace core {
    Surface::Surface(VkInstance _instance, GLFWwindow* window) : instance(_instance)
    {
        if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
            throw std::runtime_error("failed to create window surface");
        }
    }
    
    Surface::~Surface() {
        if (surface != VK_NULL_HANDLE) {
            vkDestroySurfaceKHR(instance, surface, nullptr);
        }
    }
    
    VkSurfaceKHR Surface::getSurface() const {
        return surface;
    }
}