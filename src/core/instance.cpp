#include "pch.h"
#include "core/instance.h"
#include "config.h"
#include "debugUtils.h"

namespace core {
    Instance::Instance() {
        uint32_t supportVersion = VK_API_VERSION_1_1;
        if (vkEnumerateInstanceVersion(&supportVersion) != VK_SUCCESS) {
            throw std::runtime_error("failed to enumerate vulkan instance version");
        }

        targetVersion = std::min(targetVersion, supportVersion);

        VkApplicationInfo applicationInfo {
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext = nullptr,
            .pApplicationName = "Vulkan",
            .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
            .pEngineName = nullptr,
            .apiVersion = targetVersion
        };

        uint32_t extensionCount = 0;
        const char** extensionNames = glfwGetRequiredInstanceExtensions(&extensionCount);
        if (extensionNames == nullptr && extensionCount > 0) {
            throw std::runtime_error("failed to get glfw required extensions");
        }
        std::vector<const char*> extensions(extensionNames, extensionNames + extensionCount);
        if (enableValidationLayers) {
            extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        if (enableValidationLayers) {
            uint32_t propertyCount = 0;
            vkEnumerateInstanceLayerProperties(&propertyCount, nullptr);
            bool support = true;
            if (propertyCount) {
                std::vector<VkLayerProperties> properties(propertyCount);
                vkEnumerateInstanceLayerProperties(&propertyCount, properties.data());
                for (const auto& validationLayer : validationLayers) {
                    auto it = std::find_if(properties.begin(), properties.end(), 
                    [&validationLayer](const VkLayerProperties& property) {
                        return strcmp(validationLayer, property.layerName) == 0;
                    });
                    if (it == properties.end()) {
                        support = false;
                        break;
                    }
                }
            } else {
                support = false;
            }

            if (!support) {
                throw std::runtime_error("failed to support validation layers");
            }
        }

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};

        VkInstanceCreateInfo createInfo {
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pNext = nullptr,
            .pApplicationInfo = &applicationInfo,
            .enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
            .ppEnabledExtensionNames = extensions.data()
        };
        if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();

            DebugUtils::populateDebugMessengerCreateInfo(debugCreateInfo);
            createInfo.pNext = reinterpret_cast<const void*>(&debugCreateInfo);
        } else {
            createInfo.enabledLayerCount = 0;
        }

        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
            throw std::runtime_error("failed to create instance");
        }
    }

    Instance::~Instance() {
        if (instance != VK_NULL_HANDLE) {
            vkDestroyInstance(instance, nullptr);
        }
    }

    VkInstance Instance::getInstance() const {
        return instance;
    }
}