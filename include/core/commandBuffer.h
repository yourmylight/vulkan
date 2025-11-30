#pragma once

#include "vulkanHeaders.h"

namespace core {
    class CommandBuffer {
    public:
        CommandBuffer(VkDevice _logicalDevice, VkCommandPool _commandPool);
        ~CommandBuffer();

        VkCommandBuffer getCommandBuffer() const;

        void begin(VkCommandBufferUsageFlags usageFlags = 0);
        void end();
        void reset(VkCommandBufferResetFlags flags = 0);
    private:
        VkDevice logicalDevice{};
        VkCommandPool commandPool{};
        VkCommandBuffer commandBuffer{};
    };
}