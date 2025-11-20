#pragma once

#include <pch.h>

class Instance {
public:
    Instance();
    ~Instance();

    VkInstance getInstance() const;
private:
    VkInstance instance;
    uint32_t targetVersion = VK_API_VERSION_1_1;
};