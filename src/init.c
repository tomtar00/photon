#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

int init_window() {
    // Initialize SDL with video support
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return -1;
    }

    // Set Vulkan as the video driver
    if (!SDL_Vulkan_LoadLibrary(NULL)) {
        SDL_Log("Failed to load Vulkan library: %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    // Create an SDL window with Vulkan support
    SDL_Window* window = SDL_CreateWindow(
        "Vulkan Window",
        800,
        600,
        SDL_WINDOW_VULKAN
    );

    if (!window) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        SDL_Vulkan_UnloadLibrary();
        SDL_Quit();
        return -1;
    }

    // Main loop
    SDL_Event event;
    int running = 1;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = 0;
            }
        }
        // Rendering and other logic can be added here
    }

    // Cleanup
    SDL_DestroyWindow(window);
    SDL_Vulkan_UnloadLibrary();
    SDL_Quit();

    return 0;
}