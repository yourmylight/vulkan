#include "core/commandBuffer.h"

namespace core {
    CommandBuffer::CommandBuffer(VkDevice _logicalDevice, VkCommandPool _commandPool)
        : logicalDevice(_logicalDevice), commandPool(_commandPool)
    {
        VkCommandBufferAllocateInfo allocateInfo {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .commandPool = commandPool,
            .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY, // TODO: secondary if needed
            .commandBufferCount = 1
        };

        if (vkAllocateCommandBuffers(logicalDevice, &allocateInfo, &commandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffer!");
        }
    }

    CommandBuffer::~CommandBuffer() {
        if (commandBuffer != VK_NULL_HANDLE) {
            vkFreeCommandBuffers(logicalDevice, commandPool, 1, &commandBuffer);
        }
    }

    VkCommandBuffer CommandBuffer::getCommandBuffer() const {
        return commandBuffer;
    }

    void CommandBuffer::begin(VkCommandBufferUsageFlags usageFlags) {
        VkCommandBufferBeginInfo beginInfo{
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .flags = usageFlags
        };

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }
    }

    void CommandBuffer::end() {
        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }
    }

    void CommandBuffer::reset(VkCommandBufferResetFlags flags) {
        if (vkResetCommandBuffer(commandBuffer, flags) != VK_SUCCESS) {
            throw std::runtime_error("failed to reset command buffer!");
        }
    }
}