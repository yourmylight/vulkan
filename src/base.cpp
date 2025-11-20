#include <base.h>
#include <config.h>
#include <debugUtils.h>

void Base::run() {
    setupWindow();
    initVulkan();
    prepare();
    renderLoop();
}

Base::Base(int _width, int _height) : width(_width), height(_height) {

}

Base::~Base() {
    device.reset();
    surface.reset();
    if (enableValidationLayers && debugMessenger != VK_NULL_HANDLE && instance) {
        DebugUtils::DestroyDebugUtilsMessengerEXT(instance->getInstance(), debugMessenger, nullptr);
    }
    instance.reset();
    if (window != nullptr) {
        glfwDestroyWindow(window);
    }
    
    glfwTerminate();
}

void Base::prepare() {

}

void Base::initVulkan() {
    createInstance();
    setupDebugMessenger();
    createSurface();
    createDevice();
}

void Base::setupWindow() {
    if (!glfwInit()) {
        throw std::runtime_error("failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(width, height, "Vulkan Engine", nullptr, nullptr);
    if (window == nullptr) {
        throw std::runtime_error("failed to create GLFW window");
    }
}

void Base::createInstance() {
    instance = std::make_unique<Instance>();
}

void Base::setupDebugMessenger() {
    if (!enableValidationLayers) return;
    VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo{};
    DebugUtils::populateDebugMessengerCreateInfo(debugMessengerCreateInfo);

    if (DebugUtils::CreateDebugUtilsMessengerEXT(
        instance->getInstance(),
        &debugMessengerCreateInfo,
        nullptr,
        &debugMessenger) != VK_SUCCESS) {
        throw std::runtime_error("failed to create debug messenger");
    }
}

void Base::createSurface() {
    surface = std::make_unique<Surface>(instance->getInstance(), window);
}

void Base::createDevice() {
    device = std::make_unique<Device>(instance->getInstance(), surface->getSurface());
}