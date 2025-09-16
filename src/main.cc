#include <core/core.hh>

#include <chrono>
#include <thread>

static const cog::Core core {
    [](std::vector<VkPhysicalDevice> phy_devices) -> std::uint32_t {
        // SCORE
        std::vector<int> phy_scores(phy_devices.size());

        for(int i = 0; i < phy_devices.size(); ++i) {
            auto& device = phy_devices[i];
            auto& score = phy_scores[i];

            VkPhysicalDeviceProperties deviceProperties;
            VkPhysicalDeviceFeatures deviceFeatures;
            vkGetPhysicalDeviceProperties(device, &deviceProperties);
            vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

            // Discrete GPUs have a significant performance advantage
            if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
                score += 1000;
            // Maximum possible size of textures affects graphics quality
            score += deviceProperties.limits.maxImageDimension2D;
            // Application can't function without geometry shaders
            if (!deviceFeatures.geometryShader)
                score = 0;
        }

        // RATE
        int best_score = 0;
        std::uint32_t best_index = UINT32_MAX;
        for(int i = 0; i < phy_devices.size(); ++i)
            if(phy_scores[i] > best_score) best_score=phy_scores[best_index=i];

        return best_index;
    },
    std::vector<const char *> {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    }
};

int main(int argc, char **argv) {
    std::this_thread::sleep_for(std::chrono::seconds(1));

    return 0;
}