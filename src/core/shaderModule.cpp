#include "core/shaderModule.h"

namespace core {
    ShaderModule::ShaderModule(VkDevice _logicalDevice, const std::vector<uint32_t>& spirv_code)
        :logicalDevice(_logicalDevice)
    {
        VkShaderModuleCreateInfo createInfo {
            .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .pNext = nullptr,
            .codeSize = spirv_code.size() * sizeof(uint32_t),
            .pCode = spirv_code.data()
        };
        
        if (vkCreateShaderModule(logicalDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
            throw std::runtime_error("failed to create shader module");
        }
    }

    ShaderModule::~ShaderModule() {
        if (logicalDevice != VK_NULL_HANDLE && shaderModule != VK_NULL_HANDLE) {
            vkDestroyShaderModule(logicalDevice, shaderModule, nullptr);
        }
    }
}