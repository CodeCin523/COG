#include <core/core.hh>

#include <print>

namespace cog {
CoreWindow Core::WIN{};
CoreVulkan Core::VLK{};

Core::Core(
    std::vector<const char *> vulkanDeviceExtensions
) {
    if(!WIN.Init())
        throw new std::exception("CoreWindow failed to init.");
    if(!VLK.Init(vulkanDeviceExtensions))
        throw new std::exception("CoreVulkan failed to init.");
    std::printf("Core Create.\n");
}
Core::~Core() {
    VLK.Exit();
    WIN.Exit();
    std::printf("Core Destroy.\n");
}
};

