#include <photon.h>

i32 main() {
  ph_init(AUTO_API);
  PhWindowHandle window = ph_window_create("window", 800, 600);
  PhSurfaceHandle surface = ph_render_get_surface(window);
  PhEvent event;
  while (!ph_window_is_closed(window)) {
    ph_main_loop_iteration();
    // events handling
    if (ph_event_is_key_down(ESCAPE)) {
      ph_window_close(window);
    }
    // rendering
    ph_graphics_begin_pass(surface);
    // ph_graphics_clear(RED);
    // ph_graphics_end_pass();
    // ph_graphics_submit();
  }
  ph_shutdown();
  return 0;
}
