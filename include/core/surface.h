#pragma once

#include <vulkan/vulkan.h>

struct GLFWwindow;

class Surface {
public:
    Surface(VkInstance _instance, GLFWwindow* window);
    ~Surface();

    VkSurfaceKHR getSurface() const;
private:
    VkInstance instance{};
    VkSurfaceKHR surface{};
};