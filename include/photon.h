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
typedef int8_t      i8;
typedef int16_t     i16;
typedef int32_t     i32;
typedef int64_t     i64;
typedef uint8_t     u8;
typedef uint16_t    u16;
typedef uint32_t    u32;
typedef uint64_t    u64;
typedef float       f32;
typedef double      f64;

typedef i32         ph_window_handle;
typedef i32         ph_surface_handle;

// =========================
//          Lifetime
// =========================
typedef enum ph_enum_graphics_api
{
    /// Choses the graphics API automatically based on
    /// the current operating system and hardware
    AutoAPI,
    Vulkan,
    DirectX12,
    Metal
} ph_enum_graphics_api;
void ph_init(ph_enum_graphics_api api);
void ph_shutdown();
void ph_main_loop_iteration();

// =========================
//          Events
// =========================
typedef enum ph_event_type {
    KEYBOARD_EVENT
} ph_event_type;
typedef enum ph_key_type {
    ESC
} ph_key_type;
typedef union ph_event {
    enum ph_event_type type;
} ph_event;
bool ph_event_is_key_down(ph_key_type);

// =========================
//          Window
// =========================
typedef enum ph_enum_window_flag
{
    Fullscreen = 0,
    Borderless = 1
} ph_enum_window_flag;

ph_window_handle ph_window_create(char* title, i32 width, i32 height);
ph_window_handle ph_window_create_flag(char* title, ph_enum_window_flag flag);
void ph_window_close(ph_window_handle);
bool ph_window_is_closed(ph_window_handle);

// =========================
//          Render
// =========================
typedef enum ph_enum_color
{
    RED,
    GREEN,
    BLUE
} ph_enum_color;
void ph_render_begin_pass(ph_surface_handle);
void ph_render_clear(ph_enum_color);
void ph_render_end_pass();
void ph_render_submit();

ph_surface_handle ph_render_get_surface(ph_window_handle);

#endif
