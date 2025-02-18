#include "internal.h"
#include <photon.h>

void ph_init(PhEnumGraphicsAPI api) {
  PH_TRACE("Photon init...");
  init_sdl();
  set_api(api);
  g_graphics.init();
}
void ph_shutdown() {
  PH_TRACE("Photon shutdown...");
  PH_ASSERT(g_graphics.shutdown != NULL);
  quit_sdl();
  g_graphics.shutdown();
}

void ph_main_loop_iteration() { enqueue_all_events(); }
