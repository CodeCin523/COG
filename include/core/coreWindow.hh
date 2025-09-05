#ifndef __CORE_WINDOW_HH__
#define __CORE_WINDOW_HH__

#define SDL_MAIN_HANDLED
#include <SDL2/SDL_vulkan.h>
#include <SDL2/SDL.h>

#include <vector>

namespace cog {
class CoreWindow {
public:
    CoreWindow();
    ~CoreWindow();

    /* Disabling copy and move semantics */
    CoreWindow& operator=(const CoreWindow&) = delete;
    CoreWindow(const CoreWindow&) = delete;
    CoreWindow& operator=(CoreWindow&&) = delete;
    CoreWindow(CoreWindow&&) = delete;

    friend class Core;
    friend class CoreVulkan;

protected:
    SDL_Window *sdl_window;

    std::vector<const char *> GetVulkanExtensions();

public:
    bool Init();
    bool Exit();
};
};

#endif