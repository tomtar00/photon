#ifndef PHOTON_H
#define PHOTON_H

#include <stdint.h>

// =========================
//          Errors
// =========================
#define PH_NULL NULL

// =========================
//          Alises
// =========================
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef float f32;
typedef double f64;

typedef i32 ph_window_handle;
typedef i32 ph_surface_handle;

// =========================
//          Lifetime
// =========================
typedef enum {
  /// Choses the graphics API automatically based on
  /// the current operating system and hardware
  AutoAPI,

  Vulkan,
  Metal
} ph_enum_graphics_api;
void ph_init(ph_enum_graphics_api api);
void ph_shutdown();
void ph_main_loop_iteration();

// =========================
//          Events
// =========================
typedef enum { KEYBOARD_EVENT } ph_event_type;
typedef enum {
  A = 4,
  B = 5,
  C = 6,
  D = 7,
  E = 8,
  F = 9,
  G = 10,
  H = 11,
  I = 12,
  J = 13,
  K = 14,
  L = 15,
  M = 16,
  N = 17,
  O = 18,
  P = 19,
  Q = 20,
  R = 21,
  S = 22,
  T = 23,
  U = 24,
  V = 25,
  W = 26,
  X = 27,
  Y = 28,
  Z = 29,
  NUM_1 = 30,
  NUM_2 = 31,
  NUM_3 = 32,
  NUM_4 = 33,
  NUM_5 = 34,
  NUM_6 = 35,
  NUM_7 = 36,
  NUM_8 = 37,
  NUM_9 = 38,
  NUM_0 = 39,
  RETURN = 40,
  ESCAPE = 41,
  BACKSPACE = 42,
  TAB = 43,
  SPACE = 44,
  MINUS = 45,
  EQUALS = 46,
  LEFTBRACKET = 47,
  RIGHTBRACKET = 48,
  BACKSLASH = 49,
  NONUSHASH = 50,
  SEMICOLON = 51,
  APOSTROPHE = 52,
  GRAVE = 53,
  COMMA = 54,
  PERIOD = 55,
  SLASH = 56,
  CAPSLOCK = 57,
  F1 = 58,
  F2 = 59,
  F3 = 60,
  F4 = 61,
  F5 = 62,
  F6 = 63,
  F7 = 64,
  F8 = 65,
  F9 = 66,
  F10 = 67,
  F11 = 68,
  F12 = 69,
  PRINTSCREEN = 70,
  SCROLLLOCK = 71,
  PAUSE = 72,
  INSERT = 73,
  HOME = 74,
  PAGEUP = 75,
  DELETE = 76,
  END = 77,
  PAGEDOWN = 78,
  RIGHT = 79,
  LEFT = 80,
  DOWN = 81,
  UP = 82,
} ph_key;
typedef enum { LEFT_BTN, RIGHT_BTN, MIDDLE_BTN } ph_mouse_button;
typedef union {
  ph_event_type type;
} ph_event;
bool ph_event_is_key_down(ph_key);
bool ph_event_is_key_up(ph_key);
bool ph_event_is_mouse_button_up(ph_mouse_button);
bool ph_event_is_mouse_button_down(ph_mouse_button);

// =========================
//          Window
// =========================
typedef enum { Fullscreen = 0, Borderless = 1 } ph_enum_window_flag;
ph_window_handle ph_window_create(char *title, i32 width, i32 height);
ph_window_handle ph_window_create_flag(char *title, ph_enum_window_flag flag);
void ph_window_close(ph_window_handle);
bool ph_window_is_closed(ph_window_handle);
// ph_surface_handle ph_render_get_surface(ph_window_handle);

// =========================
//          Graphics
// =========================
typedef enum { RED, GREEN, BLUE } ph_enum_color;
// void ph_graphics_begin_pass(ph_surface_handle);
// void ph_graphics_clear(ph_enum_color);
// void ph_graphics_end_pass();
// void ph_graphics_submit();

#endif
