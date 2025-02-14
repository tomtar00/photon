#include "internal.h"
#include <photon.h>

void ph_init(ph_enum_graphics_api api) {
  PH_LOG_TRACE("Photon init...");
  init_sdl();
  set_api(api);
  g_graphics.init();
}
void ph_shutdown() {
  PH_LOG_TRACE("Photon shutdown...");
  PH_ASSERT(g_graphics.shutdown != 0);
  quit_sdl();
  g_graphics.shutdown();
}

void ph_main_loop_iteration() {
  enqueue_all_events();
}
