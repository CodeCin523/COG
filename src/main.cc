#include <core/core.hh>

#include <chrono>
#include <print>
#include <thread>

static const cog::Core core{
    std::vector<const char *> {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    }
};

int main(int argc, char **argv) {
    std::this_thread::sleep_for(std::chrono::seconds(1));

    return 0;
}