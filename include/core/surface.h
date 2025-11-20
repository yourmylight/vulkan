#pragma once

#include <pch.h>

class Surface {
public:
    Surface(VkInstance _instance, GLFWwindow* window);
    ~Surface();

    VkSurfaceKHR getSurface() const;
private:
    VkInstance instance{};
    VkSurfaceKHR surface{};
};