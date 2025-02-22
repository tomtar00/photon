#include "SDL3/SDL_metal.h"
// #include "SDL3/SDL_vulkan.h"
#include "internal.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>
#include <photon.h>

#define MAX_WINDOWS 8

typedef struct WindowData {
  SDL_Window *window;
  void *surface;
} WindowData;

WindowData g_windows[MAX_WINDOWS];

SDL_WindowFlags ph_window_flags_to_sdl(PhEnumWindowFlag flag);
SDL_WindowFlags ph_graphics_api_to_sdl(PhEnumGraphicsAPI api);
PhWindowHandle create_window(char *title, i32 width, i32 height,
                       SDL_WindowFlags flags);
bool is_handle_valid(PhWindowHandle handle);
bool window_exists(PhWindowHandle handle);
bool destroy_window(PhWindowHandle handle);

PhWindowHandle ph_window_create(char *title, i32 width, i32 height) {
  SDL_WindowFlags flags = ph_graphics_api_to_sdl(g_graphicsCtx.api);
  return create_window(title, width, height, flags);
}

PhWindowHandle ph_window_create_flag(char *title, i32 width, i32 height,
                               PhEnumWindowFlag flag) {
  SDL_WindowFlags flags = ph_graphics_api_to_sdl(g_graphicsCtx.api);
  flags |= ph_window_flags_to_sdl(flag);
  return create_window(title, width, height, flags);
}

void ph_window_close(PhWindowHandle handle) {
  if (!destroy_window(handle)) {
    PH_ERROR("Failed to find window with handle: %d", handle);
  }
}

bool ph_window_is_closed(PhWindowHandle handle) {
  return is_handle_valid(handle) && g_windows[handle].window == NULL;
}

PhSurface ph_window_get_surface(PhWindowHandle handle) {
  if (window_exists(handle)) {
    switch (g_graphicsCtx.api) {
    case METAL:
      if (g_windows[handle].surface == NULL) {
        g_windows[handle].surface =
            SDL_Metal_CreateView(g_windows[handle].window);
      }
      return SDL_Metal_GetLayer(g_windows[handle].surface);
    case VULKAN:
      return NULL;
    default:
      PH_ASSERT(false);
    }
  } else {
    PH_ERROR("Failed to find window with handle: %d", handle);
    return NULL;
  }
}

// === INTERNALS

void init_sdl() {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    PH_ERROR("Failed to initialize SDL: %s", SDL_GetError());
    return;
  }
}

void quit_sdl() {
  for (i8 i = 0; i < MAX_WINDOWS; ++i) {
    destroy_window(i);
  }
  SDL_Quit();
}

PhWindowHandle create_window(char *title, i32 width, i32 height,
                       SDL_WindowFlags flags) {
  PhWindowHandle handle = -1;
  for (i8 i = 0; i < MAX_WINDOWS; ++i) {
    if (g_windows[i].window == NULL) {
      handle = i;
      break;
    }
  }
  if (handle < 0) {
    PH_ERROR("Reached max number of windows (%d)", MAX_WINDOWS);
    return -1;
  }
  SDL_Window *window = SDL_CreateWindow(title, width, height, flags);
  if (!window) {
    PH_ERROR("Failed to create window: %s", SDL_GetError());
    return -1;
  }
  g_windows[handle] = (WindowData){.window = window};
  return handle;
}
bool destroy_window(PhWindowHandle handle) {
  if (window_exists(handle)) {
    if (g_windows[handle].surface != NULL) {
      switch (g_graphicsCtx.api) {
      case METAL:
        SDL_Metal_DestroyView(g_windows[handle].surface);
        break;
      case VULKAN:
        // SDL_Vulkan_DestroySurface(VkInstance instance, VkSurfaceKHR surface,
        // const struct VkAllocationCallbacks *allocator)
        break;
      default:
        PH_ASSERT(false);
      }
    }
    SDL_DestroyWindow(g_windows[handle].window);
    g_windows[handle].window = NULL;
    return true;
  } else
    return false;
}
bool destroy_window_with_sdl_id(SDL_WindowID id) {
  for (int i = 0; i < MAX_WINDOWS; ++i) {
    if (g_windows[i].window != NULL &&
        SDL_GetWindowID(g_windows[i].window) == id) {
      destroy_window(i);
      return true;
    }
  }
  return false;
}

bool is_handle_valid(PhWindowHandle handle) {
  return handle >= 0 && handle < MAX_WINDOWS;
}

bool window_exists(PhWindowHandle handle) {
  return is_handle_valid(handle) && g_windows[handle].window != NULL;
}
SDL_WindowFlags ph_window_flags_to_sdl(PhEnumWindowFlag flag) {
  SDL_WindowFlags sdlFlags = 0;
  if (flag & FULLSCREEN) {
    sdlFlags |= SDL_WINDOW_FULLSCREEN;
  }
  if (flag & BORDERLESS) {
    sdlFlags |= SDL_WINDOW_BORDERLESS;
  }
  return sdlFlags;
}

SDL_WindowFlags ph_graphics_api_to_sdl(PhEnumGraphicsAPI api) {
  switch (api) {
  case METAL:
    return SDL_WINDOW_METAL;
  case VULKAN:
    return SDL_WINDOW_VULKAN;
  default:
    PH_ERROR("Failed to convert %d to sdl flag", api);
    return SDL_WINDOW_VULKAN;
  }
}