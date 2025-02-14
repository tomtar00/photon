#include "internal.h"
#include <photon.h>

void set_api_no_auto(ph_enum_graphics_api api) {
  bool isApple = false;
#ifdef __APPLE__
  isApple = true;
#endif
  switch (api) {
  case Metal:
    PH_ASSERT(isApple);
    create_metal_graphics();
    break;
  case Vulkan:
    PH_ASSERT(!isApple);
    create_vulkan_graphics();
  default:
    PH_ASSERT(false);
  }
}
void set_api(ph_enum_graphics_api api) {
  if (api == AutoAPI) {
#ifdef __APPLE__
    create_metal_graphics();
#else
    create_vulkan_graphics();
#endif
  } else {
    set_api_no_auto(api);
  }
}