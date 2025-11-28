#include "core/framebuffer.h"

namespace core {
    Framebuffer::Framebuffer(VkDevice _logicalDevice) : logicalDevice(_logicalDevice)
    {

    }

    Framebuffer::~Framebuffer() {
        if (framebuffer != VK_NULL_HANDLE && logicalDevice != VK_NULL_HANDLE) {
            vkDestroyFramebuffer(logicalDevice, framebuffer, nullptr);
        }
        framebuffer = VK_NULL_HANDLE;
    }

    Framebuffer& Framebuffer::setRenderPass(VkRenderPass _renderPass) {
        renderPass = _renderPass;
        return *this;
    }

    Framebuffer& Framebuffer::setSize(uint32_t _width, uint32_t _height, uint32_t _layers) {
        width = _width;
        height = _height;
        layers = _layers;
        return *this;
    }

    Framebuffer& Framebuffer::addAttachmentView(VkImageView _view) {
        attachments.emplace_back(_view);
        return *this;
    }

    Framebuffer& Framebuffer::addAttachmentViews(const std::vector<VkImageView>& _views) {
        attachments = _views;
        return *this;
    }

    VkFramebuffer Framebuffer::create() {
        if (logicalDevice == VK_NULL_HANDLE) {
            throw std::runtime_error("failed to get logical device when creating framebuffer");
        }
        if (renderPass == VK_NULL_HANDLE) {
            throw std::runtime_error("failed to get renderpass when creating framebuffer");
        }
        if (width == 0 || height == 0) {
            throw std::runtime_error("failed to get framebuffer size when creating");
        }
        if (attachments.empty()) {
            throw std::runtime_error("failed to get image view when creating framebuffer");
        }
        VkFramebufferCreateInfo createInfo {
            .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .pNext = nullptr,
            .renderPass = renderPass,
            .attachmentCount = static_cast<uint32_t>(attachments.size()),
            .pAttachments = attachments.data(),
            .width = width,
            .height = height,
            .layers = layers
        };

        if (vkCreateFramebuffer(logicalDevice, &createInfo, nullptr, &framebuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer");
        }
    }

    VkFramebuffer Framebuffer::getFramebuffer() const {
        return framebuffer;
    }
}