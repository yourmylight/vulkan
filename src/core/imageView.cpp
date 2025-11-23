#include "core/imageView.h"
#include "common/imageViewCreateInfo.h"

namespace core {
    ImageView::ImageView(VkDevice _logicalDevice, VkImage _image, const common::ImageViewCreateInfo& _createInfo) 
        : logicalDevice(_logicalDevice), image(_image)
    {
        VkImageViewCreateInfo createInfo {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext = nullptr,
            .image = image,
            .viewType = _createInfo.viewType,
            .format = _createInfo.format,
            .components = _createInfo.components,
            .subresourceRange = _createInfo.subresourceRange
        };

        if (vkCreateImageView(logicalDevice, &createInfo, nullptr, &imageView) != VK_SUCCESS) {
            throw std::runtime_error("failed to create image view");
        }
    }

    ImageView::~ImageView() {
        if (imageView != VK_NULL_HANDLE) {
            vkDestroyImageView(logicalDevice, imageView, nullptr);
        }
    }

    VkImageView ImageView::getImageView() const {
        return imageView;
    }
}