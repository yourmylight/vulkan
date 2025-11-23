#pragma once

#include <optional>

namespace core {
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() {
            return graphicFamily.has_value() && presentFamily.has_value();
        }
    };
}

