#include "internal.h"
#include <photon.h>

void ph_graphics_begin_pass(PhSurfaceHandle handle) {
  g_graphics.begin_pass(handle);
}
void ph_graphics_clear(PhEnumColor) {
  g_graphics.clear();
}
void ph_graphics_end_pass() {
  g_graphics.end_pass();
}
void ph_graphics_submit() {
  g_graphics.submit();
}

// === INTERNAL

void set_api_no_auto(PhEnumGraphicsAPI api) {
  bool isApple = false;
#ifdef __APPLE__
  isApple = true;
#endif
  switch (api) {
  case METAL:
    PH_ASSERT(isApple);
    create_metal_graphics();
    break;
  case VULKAN:
    PH_ASSERT(!isApple);
    create_vulkan_graphics();
  default:
    PH_ASSERT(false);
  }
}
void set_api(PhEnumGraphicsAPI api) {
  if (api == AUTO_API) {
#ifdef __APPLE__
    create_metal_graphics();
#else
    create_vulkan_graphics();
#endif
  } else {
    set_api_no_auto(api);
  }
}