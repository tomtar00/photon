#include "SDL3/SDL_events.h"
#include "internal.h"
#include <photon.h>

#define MAX_KEYS 512
#define MAX_MOUSE_BTNS 8

bool g_key_down[MAX_KEYS] = {false};
bool g_key_up[MAX_KEYS] = {false};

bool g_mouse_btn_down[MAX_KEYS] = {false};
bool g_mouse_btn_up[MAX_KEYS] = {false};

void enqueue_all_events() {
  // reset arrays
  memset(g_key_down, 0, sizeof(g_key_down));
  memset(g_key_up, 0, sizeof(g_key_up));
  memset(g_mouse_btn_down, 0, sizeof(g_mouse_btn_down));
  memset(g_mouse_btn_up, 0, sizeof(g_mouse_btn_up));

  // poll events
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
      destroy_window_with_sdl_id(event.window.windowID);
      break;
    case SDL_EVENT_KEY_DOWN:
      g_key_down[event.key.scancode] = true;
      break;
    case SDL_EVENT_KEY_UP:
      g_key_up[event.key.scancode] = true;
      break;
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
      g_mouse_btn_down[event.button.button] = true;
      break;
    case SDL_EVENT_MOUSE_BUTTON_UP:
      g_mouse_btn_up[event.button.button] = true;
      break;
    }
  }
}

// keyboard
bool ph_event_is_key_down(PhKey key) { return g_key_down[key]; }
bool ph_event_is_key_up(PhKey key) { return g_key_up[key]; }

// mouse button
bool ph_event_is_mouse_button_down(PhMouseButton btn) {
  return g_mouse_btn_down[btn];
}
bool ph_event_is_mouse_button_up(PhMouseButton btn) {
  return g_mouse_btn_up[btn];
}

// mouse motion

// mouse wheel