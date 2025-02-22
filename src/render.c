#include "internal.h"
#include <photon.h>

PhPipelineFormula ph_graphics_new_pipeline_formula() {
  return g_graphics.new_pipeline_formula();
}
void ph_graphics_set_func_from_src(PhPipelineFormula form,
                                   PhEnumPipelineStage stage, char *src,
                                   PhEnumShaderLang lang) {
  g_graphics.set_func_from_src(form, stage, src, lang);
}
PhVertexInput ph_graphics_new_vertex_input() {
  return g_graphics.new_vertex_input();
}
void ph_graphics_vertex_layout(PhVertexInput input, i32 idx, usize size) {
  g_graphics.vertex_layout(input, idx, size);
}
void ph_graphics_vertex_attribute(PhVertexInput input, i32 idx, PhEnumSize size,
                                  i32 offset) {
  g_graphics.vertex_attribute(input, idx, size, offset);
}
PhPipeline ph_graphics_new_pipeline(PhPipelineFormula form,
                                    PhVertexInput input) {
  return g_graphics.new_pipeline(form, input);
}

void ph_render_begin_pass(PhSurface handle) { g_graphics.begin_pass(handle); }
void ph_render_clear(PhEnumColor color) { g_graphics.clear(color); }
void ph_render_begin_recording() { g_graphics.begin_recording(); }
void ph_render_bind_pipeline(PhPipeline pipeline) {
  g_graphics.bind_pipeline(pipeline);
}
void ph_render_send_vertex_bytes(void *bytes, usize size) {
  g_graphics.send_vertex_bytes(bytes, size);
}
void ph_render_draw_triangles(i32 startIdx, i32 numTriangles) {
  g_graphics.draw_triangles(startIdx, numTriangles);
}
void ph_render_present() { g_graphics.present(); }
void ph_render_end_pass() { g_graphics.end_pass(); }
void ph_render_submit() { g_graphics.submit(); }

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