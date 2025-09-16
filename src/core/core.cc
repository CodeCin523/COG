#include <core/core.hh>

#include <exception>
#include <stdexcept>
#include <unistd.h>
// #include <print> NOT SUPPORTED BY C++ 13.2.0

namespace cog {
CoreWindow Core::WIN{};
CoreVulkan Core::VLK{};

extern std::uint32_t (*pVkCheckPhysicalDeviceFunc)(std::vector<VkPhysicalDevice>);
extern std::vector<const char *> *pVkDeviceExtensionNames;

Core::Core(
    std::uint32_t (*vulkanPickPhysicalDevice)(std::vector<VkPhysicalDevice>),
    std::vector<const char *> vulkanDeviceExtensions
) {
    pVkCheckPhysicalDeviceFunc = vulkanPickPhysicalDevice;
    pVkDeviceExtensionNames = &vulkanDeviceExtensions;

    if(!WIN.Init())
        throw new std::runtime_error("CoreWindow failed to init.");
    if(!VLK.Init())
        throw new std::runtime_error("CoreVulkan failed to init.");
    // std::printf("Core Create.\n");
}
Core::~Core() {
    VLK.Exit();
    WIN.Exit();
    // std::printf("Core Destroy.\n");
}

void Core::Log(std::string msg) {
    write(STDOUT_FILENO, msg.c_str(), msg.size());
}
};

