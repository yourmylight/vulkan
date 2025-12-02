#pragma once

#include "vulkanHeaders.h"

namespace core {
    class ShaderModule {
    public:
        ShaderModule(VkDevice _logicalDevice, const std::vector<uint32_t>& spirv_code);
        ~ShaderModule();
    private:
        VkDevice logicalDevice{};
        VkShaderModule shaderModule{};
    };
}