# COG

`COG` is a simple vulkan game that I am making for the fun of it. The name is not static and may change.

## Notes
- There is a problem currently on windows where cmake finds the x32 version of Vulkan and not x64. Manually switching the value after cmake compilation is the sole way I found to fix it.
- If cmake is unable to find the libraries, you need to manually change the default paths. A better solution will be implemented, someday.

## Needed
- `c++23`
- `cmake`
- `Vulkan`
- `SDL2`
- `hate for self`