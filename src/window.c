#include "internal.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>
#include <photon.h>

#define MAX_WINDOWS 8

typedef struct {
  SDL_Window *window;
} window_data;

window_data g_windows[MAX_WINDOWS];

SDL_WindowFlags ph_window_flags_to_sdl(ph_enum_window_flag flag);
SDL_WindowFlags ph_graphics_api_to_sdl(ph_enum_graphics_api api);
ph_window_handle create_window(char *title, i32 width, i32 height, SDL_WindowFlags flags);
bool is_handle_valid(ph_window_handle handle);
bool window_exists(ph_window_handle handle);

ph_window_handle ph_window_create(char *title, i32 width, i32 height) {
  SDL_WindowFlags flags = ph_graphics_api_to_sdl(g_graphics_ctx.api);
  return create_window(title, width, height, flags);
}
ph_window_handle ph_window_create_flag(char *title, ph_enum_window_flag flag) {
  SDL_WindowFlags flags = ph_graphics_api_to_sdl(g_graphics_ctx.api);
  flags |= ph_window_flags_to_sdl(flag);
  return create_window(title, 800, 600, flags);
}
void ph_window_close(ph_window_handle handle) {
  if (window_exists(handle)) {
    SDL_DestroyWindow(g_windows[handle].window);
    g_windows[handle].window = NULL;
  } else {
    PH_LOG_ERROR("Failed to find window with handle: %d", handle);
  }
}
bool ph_window_is_closed(ph_window_handle handle) { 
  return is_handle_valid(handle) && g_windows[handle].window == NULL;
}

// === INTERNALS

void init_sdl() {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    PH_LOG_ERROR("Failed to initialize SDL: %s", SDL_GetError());
    return;
  }
}
void quit_sdl() { 
  for (i32 i = 0; i < MAX_WINDOWS; ++i) {
    if (window_exists(i)) {
      SDL_DestroyWindow(g_windows[i].window);
      g_windows[i].window = NULL;
    }
  }
  SDL_Quit(); 
}
ph_window_handle create_window(char *title, i32 width, i32 height, SDL_WindowFlags flags) {
  ph_window_handle handle = -1;
  for (i32 i = 0; i < MAX_WINDOWS; ++i) {
    if (g_windows[i].window == NULL) {
      handle = i;
      break;
    }
  }
  if (handle < 0) {
    PH_LOG_ERROR("Reached max number of windows (%d)", MAX_WINDOWS);
    return -1;
  }
  SDL_Window *window = SDL_CreateWindow(title, width, height, flags);
  if (!window) {
    PH_LOG_ERROR("Failed to create window: %s", SDL_GetError());
    return -1;
  }
  g_windows[handle] = (window_data) {
    .window = window
  };
  return handle;
}
bool is_handle_valid(ph_window_handle handle) {
  return handle >= 0 && handle < MAX_WINDOWS;
}
bool window_exists(ph_window_handle handle) {
  return is_handle_valid(handle) && g_windows[handle].window != NULL;
}
SDL_WindowFlags ph_window_flags_to_sdl(ph_enum_window_flag flag) {
  SDL_WindowFlags sdlFlags = 0;
  if (flag & Fullscreen) {
    sdlFlags |= SDL_WINDOW_FULLSCREEN;
  }
  if (flag & Borderless) {
    sdlFlags |= SDL_WINDOW_BORDERLESS;
  }
  return sdlFlags;
}
SDL_WindowFlags ph_graphics_api_to_sdl(ph_enum_graphics_api api) {
  switch (api) {
  case Metal:
    return SDL_WINDOW_METAL;
  case Vulkan:
    return SDL_WINDOW_VULKAN;
  default:
    PH_LOG_ERROR("Failed to convert %d to sdl flag", api);
    return SDL_WINDOW_VULKAN;
  }
}