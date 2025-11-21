#pragma once

#include <vulkan/vulkan.h>

namespace core {
    class Instance {
    public:
        Instance();
        ~Instance();

        VkInstance getInstance() const;
    private:
        VkInstance instance{};
        uint32_t targetVersion = VK_API_VERSION_1_1;
    };
}