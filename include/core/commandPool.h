#pragma once

#include "vulkanHeaders.h"

namespace core {
    class CommandPool
    {
    public:
        enum class Type {
            GRAPHICS,
            TRANSFER,
            COMPUTE
        };
    public:
        CommandPool(VkDevice _logicalDevice, uint32_t _queueFamilyIndex, Type _type = Type::GRAPHICS);
        ~CommandPool();

        VkCommandPool getCommandPool() const;   
    private:
        VkDevice logicalDevice{};
        VkCommandPool commandPool{};
    };
}