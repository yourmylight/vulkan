#pragma once

#include <vulkan/vulkan.h>

namespace common {
    struct ImageViewCreateInfo;
}

namespace core {
    class ImageView {
    public:
        ImageView(
            VkDevice _logicalDevice, 
            VkImage _image,
            const common::ImageViewCreateInfo& _createInfo
        );
        ~ImageView();

        VkImageView getImageView() const;
    private:
        VkDevice logicalDevice{};
        VkImage image{};
        VkImageView imageView{};
    };
}