#include <photon.h>

char *vertexSrc = "#include <metal_stdlib>\n"
                  "using namespace metal;\n"
                  "struct VertexIn {\n"
                  "  float2 position [[attribute(0)]];\n"
                  "  float4 color [[attribute(1)]];\n"
                  "};\n"
                  "struct VertexOut {\n"
                  "  float4 position [[position]];\n"
                  "  float4 color;\n"
                  "};\n"
                  "vertex VertexOut vertexShader(VertexIn in [[stage_in]]) {\n"
                  "  VertexOut out;\n"
                  "  out.position = float4(in.position, 0.0, 1.0);\n"
                  "  out.color = in.color;\n"
                  "  return out;\n"
                  "}\n";

char *fragSrc = "#include <metal_stdlib>\n"
                "using namespace metal;\n"
                "fragment float4 fragmentShader(float4 color [[stage_in]]) {\n"
                "  return color;\n"
                "}\n";

typedef struct {
  float position[2];
  float color[4];
} Vertex;

Vertex triangleVertices[] = {
    {{0.0, 0.5}, {1.0, 0.0, 0.0, 1.0}},   // Red vertex at top-center
    {{-0.5, -0.5}, {0.0, 1.0, 0.0, 1.0}}, // Green vertex at bottom-left
    {{0.5, -0.5}, {0.0, 0.0, 1.0, 1.0}}   // Blue vertex at bottom-right
};

i32 main() {
  ph_init(AUTO_API);

  PhWindowHandle window = ph_window_create("window", 800, 600);
  PhSurface surface = ph_window_get_surface(window);

  PhPipelineFormula formula = ph_graphics_new_pipeline_formula();
  ph_graphics_set_func_from_src(formula, VERTEX, vertexSrc, MSL);
  ph_graphics_set_func_from_src(formula, FRAGMENT, fragSrc, MSL);
  PhVertexInput input = ph_graphics_new_vertex_input();
  ph_graphics_vertex_layout(input, 0, sizeof(Vertex));
  ph_graphics_vertex_attribute(input, 0, FLOAT2, offsetof(Vertex, position));
  ph_graphics_vertex_attribute(input, 1, FLOAT4, offsetof(Vertex, color));
  PhPipeline pipeline = ph_graphics_new_pipeline(formula, input);

  while (!ph_window_is_closed(window)) {
    ph_main_loop_iteration();
    // events handling
    if (ph_event_is_key_down(ESCAPE)) {
      ph_window_close(window);
    }
    // rendering
    ph_render_begin_pass(surface);
    ph_render_clear(0xFF00FFFF);
    ph_render_begin_recording();
    ph_render_bind_pipeline(pipeline);
    ph_render_send_vertex_bytes(triangleVertices, sizeof(triangleVertices));
    ph_render_draw_triangles(0, 3);
    ph_render_end_pass();
    ph_render_present();
    ph_render_submit();
  }

  ph_shutdown();
}
