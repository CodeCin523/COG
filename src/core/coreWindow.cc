#include <core/coreWindow.hh>
#include <gbl.hh>

namespace cog {
    CoreWindow::CoreWindow()  { }
    CoreWindow::~CoreWindow() { }

    std::vector<const char *> CoreWindow::GetVulkanExtensions() {
        uint32_t sdlExtensionCount = 0;
            SDL_Vulkan_GetInstanceExtensions(sdl_window, &sdlExtensionCount, nullptr);
            std::vector<const char*> extensions(sdlExtensionCount);
            SDL_Vulkan_GetInstanceExtensions(sdl_window, &sdlExtensionCount, extensions.data());
            return extensions;
    }

    bool CoreWindow::Init() {
        if(SDL_Init(SDL_INIT_VIDEO) != 0)
            return false;
        
        sdl_window = SDL_CreateWindow(
            GBL_STR(GBL_PROJECT_NAME),
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            600,
            400,
            SDL_WINDOW_VULKAN
        );
        if(!sdl_window)
            return false;
            
        return true;
    }
    bool CoreWindow::Exit() {
        if(sdl_window == nullptr) return true;
        
        SDL_DestroyWindow(sdl_window);
        sdl_window = nullptr;
        
        SDL_Quit();

        return true;
    }
}