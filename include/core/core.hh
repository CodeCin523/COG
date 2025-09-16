#ifndef __CORE_HH__
#define __CORE_HH__

#include "coreWindow.hh"
#include "coreVulkan.hh"

#include <cstdint>
#include <string>

namespace cog {
class Core {
public:
    Core(
        std::uint32_t (*vulkanPickPhysicalDevice)(std::vector<VkPhysicalDevice>),
        std::vector<const char *> vulkanDeviceExtensions
    );
    ~Core();

    /* Disabling copy and move semantics */
    Core& operator=(const Core&) = delete;
    Core(const Core&) = delete;
    Core& operator=(Core&&) = delete;
    Core(Core&&) = delete;

    friend class CoreWindow;
    friend class CoreVulkan;

public:
    static CoreWindow WIN;
    static CoreVulkan VLK;

    static void Log(std::string msg);
};
};

#endif