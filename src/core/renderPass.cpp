#include "core/renderPass.h"

namespace core {
    RenderPass::RenderPass(VkDevice _logicalDevice) : logicalDevice(_logicalDevice)
    {

    }

    RenderPass::~RenderPass() {
        if (renderPass != VK_NULL_HANDLE) {
            vkDestroyRenderPass(logicalDevice, renderPass, nullptr);
        }
    }

    RenderPass& RenderPass::beginRenderPass(VkPipelineBindPoint bindPoint) {
        colorRefsPerSubpass.emplace_back();
        VkSubpassDescription subpass{
            .pipelineBindPoint = bindPoint
        };
        subpassDescription.push_back(subpass);
    }

    RenderPass& endSubpass() {
        
    }

    RenderPass& RenderPass::addColorAttachment(uint32_t attachemnt, VkImageLayout layout) {
        colorRefsPerSubpass.back().emplace_back(VkAttachmentReference{attachemnt, layout});
    }

    RenderPass& RenderPass::addAttachment(const VkAttachmentDescription& attachmentDes) {
        attachmentDescription.emplace_back(attachmentDes);
    }

    void RenderPass::build(const std::vector<VkSubpassDependency>& subpassDependency) {
        VkRenderPassCreateInfo createInfo {

        };
    }
}