#include <core/core.hh>

#include <print>

Core::WindowManager Core::WINm{};

Core::VulkanManager Core::VLKm{};

Core::Core() {
    std::printf("Core Create.\n");
}
Core::~Core() {
    std::printf("Core Destroy.\n");
}