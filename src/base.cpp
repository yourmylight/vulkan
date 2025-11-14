#include <base.h>

void Base::prepare() {
    
}

void Base::initVulkan() {

}

void Base::setupWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(width, height, "Hello, Vulkan", nullptr, nullptr);
}

Base::~Base() {
    glfwDestroyWindow(window);
    glfwTerminate();
}