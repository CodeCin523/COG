#include <core/core.hh>

#include <print>

namespace cog {
CoreWindow Core::WIN{};
CoreVulkan Core::VLK{};

Core::Core() {
    std::printf("Core Create.\n");
}
Core::~Core() {
    std::printf("Core Destroy.\n");
}
};

