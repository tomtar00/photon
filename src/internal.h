#ifndef INTERNAL_H
#define INTERNAL_H
#include "SDL3/SDL_video.h"
#include <photon.h>
#include <stddef.h>
#include <stdio.h>

// ========================================== DEBUG
#ifdef DEBUG
#include <assert.h>
#define PH_TRACE(fmt, ...) printf("[TRACE] " fmt "\n", ##__VA_ARGS__)
#define PH_DEBUG(fmt, ...) printf("[DEBUG] " fmt "\n", ##__VA_ARGS__)
#define PH_ASSERT_2(cond, msg)                                                 \
  do {                                                                         \
    if (!(cond)) {                                                             \
      fprintf(stderr, "Assertion failed: %s, file %s, line %d: %s\n", #cond,   \
              __FILE__, __LINE__, msg);                                        \
      assert(cond);                                                            \
    }                                                                          \
  } while (0)
#define PH_ASSERT_1(cond) assert(cond)
#define PH_ASSERT_CHOOSER(_1, _2, NAME, ...) NAME
#define PH_ASSERT(...)                                                         \
  PH_ASSERT_CHOOSER(__VA_ARGS__, PH_ASSERT_2, PH_ASSERT_1)(__VA_ARGS__)
#else
#define PH_TRACE(fmt, ...)
#define PH_DEBUG(fmt, ...)
#define PH_ASSERT(cond)
#endif
#define PH_WARN(fmt, ...) printf("[WARN] " fmt "\n", ##__VA_ARGS__)
#define PH_ERROR(fmt, ...) printf("[ERROR] " fmt "\n", ##__VA_ARGS__)

// ========================================== EVENTS
void enqueue_all_events();

// ========================================== WINDOW
void init_sdl();
void quit_sdl();
bool destroy_window_with_sdl_id(SDL_WindowID id);

// ========================================== GRAPHICS
typedef struct PhGraphics {
  void (*init)();
  void (*shutdown)();

  void (*begin_pass)(PhSurfaceHandle);
  void (*clear)();
  void (*end_pass)();
  void (*submit)();
} PhGraphics;

typedef struct PhGraphicsContext {
  PhEnumGraphicsAPI api;
} PhGraphicsContext;

extern PhGraphics g_graphics;
extern PhGraphicsContext g_graphicsCtx;
void set_api(PhEnumGraphicsAPI);
void create_metal_graphics();
void create_vulkan_graphics();

#endif