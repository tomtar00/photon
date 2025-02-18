#include "internal.h"
#include "photon.h"

void vulkan_init() {
  // if (!SDL_Vulkan_LoadLibrary(NULL)) {
  //     SDL_Log("Failed to load Vulkan library: %s", SDL_GetError());
  //     SDL_Quit();
  //     return;
  // }
}
void vulkan_shutdown() {
  // SDL_Vulkan_UnloadLibrary();
}

void create_vulkan_graphics() {
  if (g_graphics.shutdown != NULL) {
    g_graphics.shutdown();
  }
  g_graphicsCtx.api = VULKAN;
  g_graphics.init = vulkan_init;
  g_graphics.shutdown = vulkan_shutdown;
};