#pragma once

#include "vulkanHeaders.h"

struct GLFWwindow;

namespace core {
    class Surface {
    public:
        Surface(VkInstance _instance, GLFWwindow* window);
        ~Surface();
    
        VkSurfaceKHR getSurface() const;
    private:
        VkInstance instance{};
        VkSurfaceKHR surface{};
    };
}