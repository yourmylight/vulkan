#include "core/commandPool.h"

namespace core {
    CommandPool::CommandPool(VkDevice _logicalDevice, uint32_t _queueFamilyIndex, Type _type /*= Type::GRAPHICS*/)
        : logicalDevice(_logicalDevice)
    {
        VkCommandPoolCreateFlags flags{};
        switch (_type) {
            case Type::GRAPHICS:
                flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
                break;
            case Type::TRANSFER:
                flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
                break;
            case Type::COMPUTE:
                // flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
                // TODO: set appropriate flags for compute if needed
                break;
        }
        VkCommandPoolCreateInfo poolInfo{
            .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .flags = flags,
            .queueFamilyIndex = _queueFamilyIndex
        };

        if (vkCreateCommandPool(logicalDevice, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create command pool!");
        }
    }

    CommandPool::~CommandPool()
    {
        if (commandPool != VK_NULL_HANDLE) {
            vkDestroyCommandPool(logicalDevice, commandPool, nullptr);
        }
    }

    VkCommandPool CommandPool::getCommandPool() const
    {
        return commandPool;
    }
}