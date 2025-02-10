#include <photon.h>

i32 main() {
    ph_init(AutoAPI);
    ph_window_handle window = ph_window_create("window", 800, 600);
    ph_surface_handle surface = ph_render_get_surface(window);
    ph_event event;
    while (!ph_window_is_closed(window)) {
        ph_main_loop_iteration();
        // events handling
        if (ph_event_is_key_down(ESC)) {
            ph_window_close(window);
        }
        // rendering
        ph_render_begin_pass(surface);
        ph_render_clear(RED);
        ph_render_end_pass();
        ph_render_submit();
    }
    ph_shutdown();
    return 0;
}
