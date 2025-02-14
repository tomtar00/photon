#ifndef INTERNAL_H
#define INTERNAL_H
#include <photon.h>
#include <stdio.h>

// ========================================== DEBUG
#ifdef DEBUG
#include <assert.h>
#define PH_LOG_TRACE(fmt, ...) printf("[TRACE] " fmt "\n", ##__VA_ARGS__)
#define PH_LOG_DEBUG(fmt, ...) printf("[DEBUG] " fmt "\n", ##__VA_ARGS__)
#define PH_ASSERT(cond) assert(cond)
#else
#define PH_LOG_TRACE(fmt, ...)
#define PH_LOG_DEBUG(fmt, ...)
#define PH_ASSERT(cond)
#endif
#define PH_LOG_WARN(fmt, ...) printf("[WARN] " fmt "\n", ##__VA_ARGS__)
#define PH_LOG_ERROR(fmt, ...) printf("[ERROR] " fmt "\n", ##__VA_ARGS__)

// ========================================== EVENTS
void enqueue_all_events();

// ========================================== WINDOW
void init_sdl();
void quit_sdl();

// ========================================== GRAPHICS
typedef struct {
  void (*init)();
  void (*shutdown)();
} ph_graphics;

typedef struct {
  ph_enum_graphics_api api;
} ph_graphics_context;

extern ph_graphics g_graphics;
extern ph_graphics_context g_graphics_ctx;
void set_api(ph_enum_graphics_api);
void create_metal_graphics();
void create_vulkan_graphics();

#endif