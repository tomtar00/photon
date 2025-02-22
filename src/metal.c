#include "cmt/command_enc_render.h"
#include "cmt/enums.h"
#include "cmt/kernels/library.h"
#include "cmt/memory/vertex.h"
#include "cmt/rendering/pipeline.h"
#include "cmt/types_foundation.h"
#include "cmt/types_metal.h"
#include "internal.h"
#include "photon.h"
#include <cmt/cmt.h>
#include <cmt/command_queue.h>

typedef struct PhMetalContext {
  MtDevice *device;
  MtCommandQueue *commandQueue;
  MtCommandBuffer *commandBuffer;
  MtLibrary *lib;
  MtFunction *vertFunc, *fragFunc;
  MtRenderPipeline *pip;
  MtRenderDesc *pipDesc;
  MtRenderPassDesc *passDesc;
  MtRenderCommandEncoder *encoder;
  MtDrawable *drawable;
} PhMetalContext;

PhMetalContext g_metal;

void metal_init() {
  NsError err = NULL;
  g_metal.device = mtCreateSystemDefaultDevice();
  PH_ASSERT(g_metal.device);
  g_metal.commandQueue = mtNewCommandQueue(g_metal.device);
  PH_ASSERT(g_metal.commandQueue);
  PH_TRACE("Metal initialized successfully");
}
void metal_shutdown() {
  mtRelease(g_metal.pip);
  g_metal.pip = NULL;
  mtRelease(g_metal.pipDesc);
  g_metal.pipDesc = NULL;
  mtRelease(g_metal.commandBuffer);
  g_metal.commandBuffer = NULL;
  mtRelease(g_metal.commandQueue);
  g_metal.commandQueue = NULL;
  mtRelease(g_metal.lib);
  g_metal.lib = NULL;
  mtRelease(g_metal.device);
  g_metal.device = NULL;
  PH_TRACE("Metal shutdown complete");
}

PhPipelineFormula metal_new_pipeline_formula() {
  // TODO: turn format into param
  return mtNewRenderPipeline(MtPixelFormatBGRA8Unorm);
}
void metal_set_func_from_src(PhPipelineFormula desc, PhEnumPipelineStage stage,
                             char *src, PhEnumShaderLang lang) {
  PH_ASSERT(lang == MSL, "You have to use MSL for Metal");
  NsError err;
  MtLibrary *lib = mtNewLibraryWithSource(g_metal.device, src, NULL, &err);
  PH_ASSERT(lib, mtErrorLocalizedDescription(err));
  if (stage == VERTEX) {
    MtFunction *func = mtNewFunctionWithName(lib, "vertexShader");
    mtSetFunc(desc, func, MT_FUNC_VERT);
  } else if (stage == FRAGMENT) {
    MtFunction *func = mtNewFunctionWithName(lib, "fragmentShader");
    mtSetFunc(desc, func, MT_FUNC_FRAG);
  } else {
    PH_ASSERT(false);
  }
}
PhVertexInput metal_new_vertex_input() { return mtVertexDescNew(); }
void metal_vertex_layout(PhVertexInput desc, i32 idx, usize size) {
  mtVertexLayout(desc, idx, size, 1, MtVertexStepFunctionPerVertex);
}
void metal_vertex_attribute(PhVertexInput desc, i32 idx, PhEnumSize size,
                            i32 offset) {
  mtVertexAttrib(desc, idx, (MtVertexFormat)size, offset, 0);
}
PhPipeline metal_new_pipeline(PhPipelineFormula desc, PhVertexInput input) {
  NsError err;
  mtSetVertexDesc(desc, input);
  MtRenderPipeline *pipeline = mtNewRenderState(g_metal.device, desc, &err);
  PH_ASSERT(pipeline, mtErrorLocalizedDescription(err));
  return pipeline;
}

// void metal_begin_pass_framebuffer(PhFramebuffer drawable) {
//   g_metal.drawable = drawable;
//   g_metal.passDesc = mtNewPass();
//   mtPassTexture(g_metal.passDesc, 0, mtDrawableTexture(drawable));
// }
void metal_begin_pass(PhSurface surface) {
  g_metal.drawable = mtNextDrawable(surface, g_metal.device);
  PH_ASSERT(g_metal.drawable);
  g_metal.passDesc = mtNewPass();
  mtPassTexture(g_metal.passDesc, 0, mtDrawableTexture(g_metal.drawable));
}
void metal_clear(PhEnumColor color) {
  i32 red = (color >> 24) & 0xFF;
  i32 green = (color >> 16) & 0xFF;
  i32 blue = (color >> 8) & 0xFF;
  i32 alpha = color & 0xFF;
  mtPassLoadAction(g_metal.passDesc, 0, MtLoadActionClear);
  mtPassClearColor(g_metal.passDesc, 0, red, green, blue, alpha);
}
void metal_begin_recording() {
  g_metal.commandBuffer = mtNewCommandBuffer(g_metal.commandQueue);
  PH_ASSERT(g_metal.commandBuffer);
  g_metal.encoder =
      mtNewRenderCommandEncoder(g_metal.commandBuffer, g_metal.passDesc);
  PH_ASSERT(g_metal.encoder);
}
void metal_bind_pipeline(PhPipeline pipeline) {
  mtSetRenderState(g_metal.encoder, pipeline);
}
void metal_send_vertex_bytes(void *data, usize size) {
  mtVertexBytes(g_metal.encoder, data, size, 0);
}
void metal_draw_triangles(i32 startIdx, i32 numTriangles) {
  mtDrawPrims(g_metal.encoder, MtPrimitiveTypeTriangle, startIdx, numTriangles);
}
void metal_present() {
  mtCommandBufferPresentDrawable(g_metal.commandBuffer, g_metal.drawable);
}
void metal_end_pass() { mtCommandEncoderEndEncoding(g_metal.encoder); }
void metal_submit() { mtCommandBufferCommit(g_metal.commandBuffer); }

void create_metal_graphics() {
  if (g_graphics.shutdown != NULL) {
    g_graphics.shutdown();
  }
  g_graphicsCtx.api = METAL;
  g_graphics.init = metal_init;
  g_graphics.shutdown = metal_shutdown;

  g_graphics.new_pipeline_formula = metal_new_pipeline_formula;
  g_graphics.set_func_from_src = metal_set_func_from_src;
  g_graphics.new_vertex_input = metal_new_vertex_input;
  g_graphics.vertex_layout = metal_vertex_layout;
  g_graphics.vertex_attribute = metal_vertex_attribute;
  g_graphics.new_pipeline = metal_new_pipeline;

  g_graphics.begin_pass = metal_begin_pass;
  g_graphics.clear = metal_clear;
  g_graphics.begin_recording = metal_begin_recording;
  g_graphics.bind_pipeline = metal_bind_pipeline;
  g_graphics.send_vertex_bytes = metal_send_vertex_bytes;
  g_graphics.draw_triangles = metal_draw_triangles;
  g_graphics.present = metal_present;
  g_graphics.end_pass = metal_end_pass;
  g_graphics.submit = metal_submit;
};