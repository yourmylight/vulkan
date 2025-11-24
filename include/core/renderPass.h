#pragma once 

#include <vulkan/vulkan.h>

namespace core {
    class RenderPass {
    public:
        RenderPass();
        ~RenderPass();
    private:
        VkDevice logicalDevice{};
        std::vector<VkAttachmentDescription> attachmentDescription;
        std::vector<VkSubpassDescription> subpassDescription;
        std::vector<VkSubpassDependency> subpassDependency;
        
        std::vector<std::vector<VkAttachmentReference>> colorRefsPerSubpass;
        std::vector<std::optional<VkAttachmentReference>> depthRefsPerSubpass;
        std::vector<std::vector<VkAttachmentReference>> inputRefsPerSubpass;
        std::vector<std::vector<VkAttachmentReference>> resolveRefsPerSubpass;
        std::vector<std::vector<uint32_t>> preserveRefsPerSubpass;
    };
}