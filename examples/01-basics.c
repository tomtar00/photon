#include <photon.h>

i32 main() {
  ph_init(AUTO_API);
  
  PhWindowHandle window = ph_window_create("window", 800, 600);
  PhSurface surface = ph_window_get_surface(window);
  
  while (!ph_window_is_closed(window)) {
    ph_main_loop_iteration();
    // events handling
    if (ph_event_is_key_down(ESCAPE)) {
      ph_window_close(window);
    }
    // rendering
    ph_render_begin_pass(surface);
    ph_render_clear(GREEN);
    ph_render_begin_recording();
    ph_render_end_pass();
    ph_render_present();
    ph_render_submit();
  }

  ph_shutdown();
}
