#include <core/core.hh>

#include <print>

namespace cog {
CoreWindow Core::WIN{};
CoreVulkan Core::VLK{};

Core::Core() {
    if(!WIN.Init())
        throw new std::exception("CoreWindow failed to init.");
    if(!VLK.Init())
        throw new std::exception("CoreVulkan failed to init.");
    std::printf("Core Create.\n");
}
Core::~Core() {
    VLK.Exit();
    WIN.Exit();
    std::printf("Core Destroy.\n");
}
};

