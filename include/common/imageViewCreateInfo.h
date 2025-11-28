#pragma once

#include "core/vulkanHeaders.h"

namespace common {
    struct ImageViewCreateInfo {
        VkImageViewType viewType {VK_IMAGE_VIEW_TYPE_2D};
        VkFormat format {VK_FORMAT_UNDEFINED};
        VkComponentMapping components {
            .r = VK_COMPONENT_SWIZZLE_IDENTITY,
            .g = VK_COMPONENT_SWIZZLE_IDENTITY,
            .b = VK_COMPONENT_SWIZZLE_IDENTITY,
            .a = VK_COMPONENT_SWIZZLE_IDENTITY
        };
        VkImageSubresourceRange subresourceRange {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1
        };
    };
}