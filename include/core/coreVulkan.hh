#ifndef __CORE_VULKAN_HH__
#define __CORE_VULKAN_HH__

#include <vulkan/vulkan.h>

namespace cog {
class CoreVulkan {
public:
    CoreVulkan();
    ~CoreVulkan();

    /* Disabling copy and move semantics */
    CoreVulkan& operator=(const CoreVulkan&) = delete;
    CoreVulkan(const CoreVulkan&) = delete;
    CoreVulkan& operator=(CoreVulkan&&) = delete;
    CoreVulkan(CoreVulkan&&) = delete;

public:
    enum commitType { // Guaranteed VkQueueFlagBits 
        COMMIT_TYPE_PRESENT  = 0, // Graphics + Compute + Transfer
        COMMIT_TYPE_GRAPHICS = 1, // Graphics + Compute + Transfer
        COMMIT_TYPE_COMPUTE  = 2, // Compute + Transfer
        COMMIT_TYPE_TRANSFER = 3  // Transfer
    };
    using commitFunc = const void (*)(VkCommandBuffer cmd);

    void Commit(const commitType _type, const commitFunc _func);
};
};

#endif