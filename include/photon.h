#ifndef PHOTON_H
#define PHOTON_H

#include <stddef.h>
#include <stdint.h>

// === Naming Conventions
//  Types: PascalCase
//  Variables: camelCase
//  Functions: snake_case
//  Enums: UPPERCASE
//  Defines: UPPERCASE

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
typedef size_t usize;
typedef float f32;
typedef double f64;

typedef i32 PhWindow;
typedef void *PhSurface;
typedef void *PhFramebuffer;
typedef void *PhPipelineDescriptor;
typedef void *PhVertexInputDescriptor;
typedef void *PhPipeline;

// =========================
//          Core
// =========================
typedef enum PhEnumGraphicsAPI {
  /// Choses the graphics API automatically based on
  /// the current operating system and hardware
  AUTO_API,
  VULKAN,
  METAL
} PhEnumGraphicsAPI;
void ph_init(PhEnumGraphicsAPI api);
void ph_shutdown();
void ph_main_loop_iteration();

// =========================
//          Events
// =========================
typedef enum PhEventType { KEYBOARD_EVENT } PhEventType;
typedef enum PhKey {
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
} PhKey;
typedef enum PhMouseButton { LEFT_BTN, RIGHT_BTN, MIDDLE_BTN } PhMouseButton;
typedef union PhEvent {
  PhEventType type;
} PhEvent;
bool ph_event_is_key_down(PhKey);
bool ph_event_is_key_up(PhKey);
bool ph_event_is_mouse_button_up(PhMouseButton);
bool ph_event_is_mouse_button_down(PhMouseButton);

// =========================
//          Window
// =========================
typedef enum PhEnumWindowFlag {
  FULLSCREEN = 0,
  BORDERLESS = 1
} PhEnumWindowFlag;
PhWindow ph_window_create(char *title, i32 width, i32 height);
PhWindow ph_window_create_flag(char *title, i32 width, i32 height,
                               PhEnumWindowFlag flag);
void ph_window_close(PhWindow);
bool ph_window_is_closed(PhWindow);
PhSurface ph_window_get_surface(PhWindow);

// =========================
//         Graphics
// =========================
typedef enum PhEnumColor {
  BLACK = 0x000000FF,
  WHITE = 0xFFFFFFFF,
  RED = 0xFF0000FF,
  GREEN = 0x00FF00FF,
  BLUE = 0x0000FFFF
} PhEnumColor;
typedef enum PhEnumSize {
  FLOAT = 28,
  FLOAT2 = 29,
  FLOAT3 = 30,
  FLOAT4 = 31,
  INT = 32,
  INT2 = 33,
  INT3 = 34,
  INT4 = 35
} PhEnumSize;
typedef struct PhRenderPipelineFormula {
  PhPipelineDescriptor desc;
  PhVertexInputDescriptor vertexInput;
  char *vertCache;
  char *fragCache;
} PhRenderPipelineFormula;
typedef enum PhEnumPipelineStage { VERTEX, FRAGMENT } PhEnumPipelineStage;
typedef enum PhEnumShaderLang { MSL, GLSL, HLSL } PhEnumShaderLang;
void ph_graphics_new_pipeline_formula(PhRenderPipelineFormula*);
void ph_graphics_set_func_from_src(PhRenderPipelineFormula *,
                                   PhEnumPipelineStage, char *src,
                                   PhEnumShaderLang, char *cache);
void ph_graphics_set_func_from_file(PhRenderPipelineFormula *,
                                    PhEnumPipelineStage, char *filename,
                                    PhEnumShaderLang, char *cache);
void ph_graphics_set_func_from_cache(PhRenderPipelineFormula *,
                                     PhEnumPipelineStage, char *filename,
                                     PhEnumShaderLang);
void ph_graphics_vertex_layout(PhRenderPipelineFormula *, i32 idx, usize size);
void ph_graphics_vertex_attribute(PhRenderPipelineFormula *, i32 idx,
                                  PhEnumSize, i32 offset);
PhPipeline ph_graphics_new_pipeline(PhRenderPipelineFormula *);

// =========================
//          Render
// =========================
void ph_render_begin_pass(PhSurface);
void ph_render_clear(PhEnumColor);
void ph_render_begin_recording();
void ph_render_bind_pipeline(PhPipeline);
void ph_render_send_vertex_bytes(void *bytes, usize size);
void ph_render_draw_triangles(i32 startIdx, i32 numTriangles);
void ph_render_present();
void ph_render_end_pass();
void ph_render_submit();

#endif
