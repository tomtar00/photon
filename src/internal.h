#ifndef INTERNAL_H
#define INTERNAL_H
#include "SDL3/SDL_video.h"
#include <photon.h>

// ========================================== FILE
bool file_exists(const char *path);
char *load_file(const char *filename);

// ========================================== DEBUG
#ifdef DEBUG
#include <assert.h>
#include <stdio.h>
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
#define PH_ASSERT_2(cond, msg)
#define PH_ASSERT_1(cond)
#define PH_ASSERT_CHOOSER(_1, _2, NAME, ...) NAME
#define PH_ASSERT(...)                                                         \
  PH_ASSERT_CHOOSER(__VA_ARGS__, PH_ASSERT_2, PH_ASSERT_1)(__VA_ARGS__)
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

  void (*new_pipeline_formula)(PhRenderPipelineFormula *);
  void (*set_func_from_src)(PhRenderPipelineFormula *, PhEnumPipelineStage,
                            char *, PhEnumShaderLang, char *cache);
  void (*set_func_from_cache)(PhRenderPipelineFormula *, PhEnumPipelineStage,
                              char *, PhEnumShaderLang);
  void (*vertex_layout)(PhRenderPipelineFormula *, i32 idx, usize size);
  void (*vertex_attribute)(PhRenderPipelineFormula *, i32 idx, PhEnumSize,
                           i32 offset);
  PhPipeline (*new_pipeline)(PhRenderPipelineFormula *);

  void (*begin_pass)(PhSurface);
  void (*clear)(PhEnumColor);
  void (*begin_recording)();
  void (*bind_pipeline)(PhPipeline);
  void (*send_vertex_bytes)(void *, usize);
  void (*draw_triangles)(i32, i32);
  void (*present)();
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