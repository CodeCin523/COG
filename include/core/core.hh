#ifndef __CORE_HH__
#define __CORE_HH__

#include <vulkan/vulkan.h>

class Core {
public:
    Core();
    ~Core();

    /* Disabling copy and move semantics */
    Core& operator=(const Core&) = delete;
    Core(const Core&) = delete;
    Core& operator=(Core&&) = delete;
    Core(Core&&) = delete;

public:
    class WindowManager {

    };
    static WindowManager WINm;

    class VulkanManager {
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
    static VulkanManager VLKm;
};

#endif