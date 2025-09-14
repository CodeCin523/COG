#ifndef __CORE_HH__
#define __CORE_HH__

#include "coreWindow.hh"
#include "coreVulkan.hh"

namespace cog {
class Core {
public:
    Core(
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
};
};

#endif