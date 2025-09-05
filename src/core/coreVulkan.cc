#include <core/coreVulkan.hh>
#include <core/core.hh>
#include <gbl.hh>

#include <vector>

namespace Vk {
    static inline void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& _info) {
        _info = {
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
            // .pNext = nullptr,
            .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
            .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
            .pfnUserCallback = nullptr //VlkDebugCallback,
            // .pUserData = nullptr
        };
    }

};

namespace cog {
    static const std::vector<const char *> VK_VALIDATION_LAYERS = {
        "VK_LAYER_KHRONOS_validation"
    };
    static const std::vector<const char *> VK_DEVICE_EXTENSIONS = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    static bool CheckValidationLayerSupport() {
        std::uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
        for (const char* layerName : VK_VALIDATION_LAYERS) {
            bool layerFound = false;
            for (const auto& layerProperties : availableLayers)
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            if (!layerFound)
                return false;
        }
        return true;
    }

    CoreVulkan::CoreVulkan()  { }
    CoreVulkan::~CoreVulkan() { }

    bool CoreVulkan::Init() {
        if(GBL_DEBUG)
            if(!CheckValidationLayerSupport())
                return false;
        
        {   // INSTANCE
            // extensions
            auto extensions = Core::WIN.GetVulkanExtensions();

            if (GBL_DEBUG)
                extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

            // create info
            VkApplicationInfo app_info = {
                .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                .pNext = nullptr,
                .pApplicationName = GBL_STR(GBL_PROJECT_NAME),
                .applicationVersion = VK_MAKE_VERSION(GBL_PROJECT_VERSION_MAJOR, GBL_PROJECT_VERSION_MINOR, GBL_PROJECT_VERSION_PATCH),
                .pEngineName = "NoEngine",
                .engineVersion = VK_MAKE_VERSION(1,0,0),
                .apiVersion = VK_API_VERSION_1_3
            };
            VkInstanceCreateInfo inst_info = {
                .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0,
                .pApplicationInfo = &app_info,
                .enabledLayerCount = 0,
                .ppEnabledLayerNames = nullptr,
                .enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
                .ppEnabledExtensionNames = extensions.data()
            };

            VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
            if(GBL_DEBUG) {
                Vk::PopulateDebugMessengerCreateInfo(debugCreateInfo);
                inst_info.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;

                inst_info.enabledLayerCount = static_cast<uint32_t>(VK_VALIDATION_LAYERS.size());;
                inst_info.ppEnabledLayerNames = VK_VALIDATION_LAYERS.data();
            }

            if(vkCreateInstance(&inst_info, nullptr, &vk_inst) != VK_SUCCESS)
                return false;
        }
        if(GBL_DEBUG) {   // DEBUG MESSAGER
            VkDebugUtilsMessengerCreateInfoEXT createInfo;
            Vk::PopulateDebugMessengerCreateInfo(createInfo);

            auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(vk_inst, "vkCreateDebugUtilsMessengerEXT");
            if(func == nullptr)
                return false;
            if(func(vk_inst, &createInfo, nullptr, &vk_debugMessenger) != VK_SUCCESS)
                return false;
        }
        {   // PHYSICAL DEVICE
            std::uint32_t deviceCount = 0;
            vkEnumeratePhysicalDevices(vk_inst, &deviceCount, nullptr);
            if(deviceCount == 0) return false;

            std::vector<VkPhysicalDevice> phy_devices{deviceCount};
            vkEnumeratePhysicalDevices(vk_inst, &deviceCount, phy_devices.data());

            const auto pickFunc = [phy_devices]() -> std::uint32_t {
                // SCORE
                std::vector<int> phy_scores;

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
            };

            std::uint32_t index = pickFunc();

            vk_phy_device = phy_devices[index];
        }

        // SURFACE
        SDL_Vulkan_CreateSurface(Core::WIN.sdl_window, vk_inst, &vk_surface);

        uint32_t queue_familyIndex = UINT32_MAX;
        VkQueueFamilyProperties queue_properties{};
        {   // QUEUE FAMILIES
            uint32_t count;
            vkGetPhysicalDeviceQueueFamilyProperties(vk_phy_device, &count, nullptr);
            std::vector<VkQueueFamilyProperties> queues{count};
            vkGetPhysicalDeviceQueueFamilyProperties(vk_phy_device, &count, queues.data());

            for(int i = 0; i < queues.size(); ++i) {
                if(queues[i].queueCount <= 0) continue;

                if( queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT    &&
                    queues[i].queueFlags & VK_QUEUE_COMPUTE_BIT     &&
                    queues[i].queueFlags & VK_QUEUE_TRANSFER_BIT
                ) {
                    VkBool32 support;
                    vkGetPhysicalDeviceSurfaceSupportKHR(vk_phy_device, i, vk_surface, &support);
                    if(!support) continue;
                    
                    queue_familyIndex = i;
                    queue_properties = queues[i];
                    break;
                }
            }

            if(queue_familyIndex == UINT32_MAX)
                return false;
        }
        {   // LOGICAL DEVICE
            const float PRIORITIES[] = {1.0f};
            VkDeviceQueueCreateInfo queueInfo = {
                .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0,
                .queueFamilyIndex = queue_familyIndex,
                .queueCount = 1,
                .pQueuePriorities = PRIORITIES
            };

            VkPhysicalDeviceFeatures deviceFeatures = {};
            VkDeviceCreateInfo createInfo = {
                .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0,
                .queueCreateInfoCount = 1,
                .pQueueCreateInfos = &queueInfo,
                .enabledLayerCount = 0,
                .ppEnabledLayerNames = nullptr,
                .enabledExtensionCount = (uint32_t) VK_DEVICE_EXTENSIONS.size(),
                .ppEnabledExtensionNames = VK_DEVICE_EXTENSIONS.data(),
                .pEnabledFeatures = &deviceFeatures
            };

            if(vkCreateDevice(vk_phy_device, &createInfo, nullptr, &vk_log_device) != VK_SUCCESS)
                return false;

            vkGetDeviceQueue(vk_log_device, queue_familyIndex, 0, &vk_queue);
        }

        return true;
    }
    bool CoreVulkan::Exit() {
        if(vk_log_device != VK_NULL_HANDLE) {
            vkDestroyDevice(vk_log_device, nullptr);
            vk_log_device = VK_NULL_HANDLE;
            vk_queue = VK_NULL_HANDLE;
        }

        if(vk_phy_device != VK_NULL_HANDLE)
            vk_phy_device = VK_NULL_HANDLE;

        if(vk_debugMessenger != VK_NULL_HANDLE) {
            auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(vk_inst, "vkDestroyDebugUtilsMessengerEXT");
            if (func != nullptr) {
                func(vk_inst, vk_debugMessenger, nullptr);
            }
        }

        if(vk_inst != VK_NULL_HANDLE) {
            vkDestroyInstance(vk_inst, nullptr);
            vk_inst = VK_NULL_HANDLE;
        }

        return true;
    }
};