#include "internal.h"
#include <cmt/cmt.h>
#include <cmt/command_queue.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  MtDevice *device;
  MtCommandQueue *commandQueue;
} ph_metal_context;

ph_metal_context g_metal;

void metal_init() {
  g_metal.device = mtCreateSystemDefaultDevice();
  if (!g_metal.device) {
    PH_LOG_ERROR("Failed to create Metal device");
    return;
  }
  g_metal.commandQueue = mtNewCommandQueue(g_metal.device);
  if (!g_metal.commandQueue) {
    PH_LOG_ERROR("Failed to create command queue");
    mtRelease(g_metal.device);
    g_metal.device = NULL;
    return;
  }
  PH_LOG_TRACE("Metal initialized successfully");
}
void metal_shutdown() {
  if (g_metal.commandQueue) {
    mtRelease(g_metal.commandQueue);
    g_metal.commandQueue = NULL;
  }
  if (g_metal.device) {
    mtRelease(g_metal.device);
    g_metal.device = NULL;
  }
  PH_LOG_TRACE("Metal shutdown complete");
}

void create_metal_graphics() {
  if (g_graphics.shutdown != NULL) {
    g_graphics.shutdown();
  }
  g_graphics_ctx.api = Metal;
  g_graphics.init = metal_init;
  g_graphics.shutdown = metal_shutdown;
};