#ifndef __CORE_WINDOW_HH__
#define __CORE_WINDOW_HH__

#define SDL_MAIN_HANDLED
#include <SDL2/SDL_vulkan.h>
#include <SDL2/SDL.h>

#include <vector>

namespace cog {
class CoreWindow {
public:
    /* Disabling copy and move semantics */
    CoreWindow& operator=(const CoreWindow&) = delete;
    CoreWindow(const CoreWindow&) = delete;
    CoreWindow& operator=(CoreWindow&&) = delete;
    CoreWindow(CoreWindow&&) = delete;

    friend class Core;
    friend class CoreVulkan;

protected:
    // Variables.
    SDL_Window *sdl_window;

    // Functions.
    std::vector<const char *> GetVulkanExtensions();
    std::pair<int, int> GetVulkanDrawableSize();

    // Initialization and constructions.
    CoreWindow();
    ~CoreWindow();

    bool Init();
    bool Exit();

public:
};
};

#endif