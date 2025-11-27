#pragma once

#include <vulkan/vulkan.h>

namespace core {
    class Framebuffer {
    public:
        Framebuffer(VkDevice _logicalDevice);
        ~Framebuffer();

        Framebuffer& setRenderPass(VkRenderPass _renderPass);

        Framebuffer& setSize(uint32_t _width, uint32_t _height, uint32_t _layers = 1);

        Framebuffer& addAttachmentView(VkImageView _view);

        Framebuffer& addAttachmentViews(const std::vector<VkImageView>& _views);

        VkFramebuffer create();

        VkFramebuffer getFramebuffer() const;
    private:
        VkDevice logicalDevice{};
        VkRenderPass renderPass{};
        std::vector<VkImageView> attachments;
        uint32_t width = 0;
        uint32_t height = 0;
        uint32_t layers = 1;
        VkFramebuffer framebuffer{};
    };
}