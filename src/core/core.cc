#include <core/core.hh>

#include <print>

namespace cog {
CoreWindow Core::WIN{};
CoreVulkan Core::VLK{};

Core::Core() {
    WIN.Init();
    VLK.Init();
    std::printf("Core Create.\n");
}
Core::~Core() {
    VLK.Exit();
    WIN.Exit();
    std::printf("Core Destroy.\n");
}
};

