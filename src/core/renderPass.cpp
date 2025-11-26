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

    RenderPass& RenderPass::beginSubPass(VkPipelineBindPoint bindPoint) {
        colorRefsPerSubpass.emplace_back();
        depthRefsPerSubpass.emplace_back();
        inputRefsPerSubpass.emplace_back();
        resolveRefsPerSubpass.emplace_back();
        preserveRefsPerSubpass.emplace_back();

        VkSubpassDescription subpass{
            .pipelineBindPoint = bindPoint
        };
        subpassDescription.push_back(subpass);
        return *this;
    }

    RenderPass& RenderPass::endSubpass() {
        auto& subpassDes = subpassDescription.back();
        subpassDes.inputAttachmentCount = static_cast<uint32_t>(inputRefsPerSubpass.back().size());
        subpassDes.pInputAttachments = 
            inputRefsPerSubpass.back().empty() ? nullptr : inputRefsPerSubpass.back().data();
        subpassDes.colorAttachmentCount = static_cast<uint32_t>(colorRefsPerSubpass.back().size());
        subpassDes.pColorAttachments = 
            colorRefsPerSubpass.back().empty() ? nullptr : colorRefsPerSubpass.back().data();
        subpassDes.pResolveAttachments = 
            resolveRefsPerSubpass.back().empty() ? nullptr : resolveRefsPerSubpass.back().data();
        subpassDes.pDepthStencilAttachment = 
            depthRefsPerSubpass.back() ? &depthRefsPerSubpass.back().value() : nullptr;
        subpassDes.preserveAttachmentCount = static_cast<uint32_t>(preserveRefsPerSubpass.back().size());
        subpassDes.pPreserveAttachments = 
            preserveRefsPerSubpass.back().empty() ? nullptr : preserveRefsPerSubpass.back().data();
        return *this;
    }

    RenderPass& RenderPass::addColorAttachment(uint32_t attachemnt, VkImageLayout layout) {
        colorRefsPerSubpass.back().emplace_back(VkAttachmentReference{attachemnt, layout});
        return *this;
    }

    RenderPass& RenderPass::addResolveAttachment(uint32_t attachemnt, VkImageLayout layout) {
        resolveRefsPerSubpass.back().emplace_back(VkAttachmentReference{attachemnt, layout});
        return *this;
    }

    RenderPass& RenderPass::addDepthAttachment(uint32_t attachemnt, VkImageLayout layout) {
        depthRefsPerSubpass.back() = VkAttachmentReference{attachemnt, layout};
        return *this;
    }

    RenderPass& RenderPass::addInputAttachment(uint32_t attachemnt, VkImageLayout layout) {
        inputRefsPerSubpass.back().emplace_back(VkAttachmentReference{attachemnt, layout});
        return *this;
    }

    RenderPass& RenderPass::addPreserveAttachment(uint32_t attachemnt) {
        preserveRefsPerSubpass.back().emplace_back(attachemnt);
        return *this;
    }

    RenderPass& RenderPass::addAttachment(const VkAttachmentDescription& attachmentDes) {
        attachmentDescription.emplace_back(attachmentDes);
        return *this;
    }

    void RenderPass::build(const std::vector<VkSubpassDependency>& subpassDependency) {
        VkRenderPassCreateInfo createInfo {
            .sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
            .pNext           = nullptr,
            .flags           = 0,
            .attachmentCount = static_cast<uint32_t>(attachmentDescription.size()),
            .pAttachments    = attachmentDescription.empty() ? nullptr : attachmentDescription.data(),
            .subpassCount    = static_cast<uint32_t>(subpassDescription.size()),
            .pSubpasses      = subpassDescription.empty() ? nullptr : subpassDescription.data(),
            .dependencyCount = static_cast<uint32_t>(subpassDependency.size()),
            .pDependencies   = subpassDependency.empty() ? nullptr : subpassDependency.data()
        };

        if (vkCreateRenderPass(logicalDevice, &createInfo, nullptr, &renderPass) != VK_SUCCESS) {
            throw std::runtime_error("failed to create renderPass");
        }
    }
}