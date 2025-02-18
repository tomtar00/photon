#include "cmt/command_enc_render.h"
#include "cmt/kernels/library.h"
#include "cmt/memory/vertex.h"
#include "cmt/types_foundation.h"
#include "cmt/types_metal.h"
#include "internal.h"
#include <cmt/cmt.h>
#include <cmt/command_queue.h>

char *librarySrc =
    "#include <metal_stdlib>\n"
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
    "}\n"
    "#include <metal_stdlib>\n"
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

typedef struct PhMetalContext {
  MtDevice *device;
  MtCommandQueue *commandQueue;
  MtCommandBuffer *commandBuffer;
  MtLibrary *lib;
  MtFunction *vertFunc, *fragFunc;
  MtRenderPipeline *pip;
  MtRenderDesc *pipDesc;
} PhMetalContext;

PhMetalContext g_metal;

void metal_init() {
  NsError err = NULL;
  g_metal.device = mtCreateSystemDefaultDevice();
  PH_ASSERT(g_metal.device);
  g_metal.lib = mtNewLibraryWithSource(g_metal.device, librarySrc, NULL, &err);
  PH_ASSERT(g_metal.lib, mtErrorLocalizedDescription(err));
  g_metal.commandQueue = mtNewCommandQueue(g_metal.device);
  PH_ASSERT(g_metal.commandQueue);
  
  g_metal.pipDesc = mtNewRenderPipeline(MtPixelFormatBGRA8Unorm);
  PH_ASSERT(g_metal.pipDesc);
  g_metal.vertFunc = mtNewFunctionWithName(g_metal.lib, "vertexShader");
  g_metal.fragFunc = mtNewFunctionWithName(g_metal.lib, "fragmentShader");
  mtSetFunc(g_metal.pipDesc, g_metal.vertFunc, MT_FUNC_VERT);
  mtSetFunc(g_metal.pipDesc, g_metal.fragFunc, MT_FUNC_FRAG);
  
  MtVertexDescriptor *vertDesc = mtVertexDescNew();
  mtVertexLayout(vertDesc, 0, sizeof(Vertex), 1, MtVertexStepFunctionPerVertex);
  mtVertexAttrib(vertDesc, 0, MtVertexFormatFloat2, offsetof(Vertex, position), 0);
  mtVertexAttrib(vertDesc, 1, MtVertexFormatFloat4, offsetof(Vertex, color), 0);
  mtSetVertexDesc(g_metal.pipDesc, vertDesc);

  g_metal.pip = mtNewRenderState(g_metal.device, g_metal.pipDesc, &err);
  PH_ASSERT(g_metal.pip, mtErrorLocalizedDescription(err));
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

void metal_begin_pass(PhSurfaceHandle surface) {
  g_metal.commandBuffer = mtNewCommandBuffer(g_metal.commandQueue);
  PH_ASSERT(g_metal.commandBuffer);

  // Get the drawable first.
  MtDrawable* drawable = mtNextDrawable(surface, g_metal.device);
  PH_ASSERT(drawable);

  // Create and configure the render pass descriptor using the drawable's texture.
  MtRenderPassDesc *passDesc = mtNewPass();
  mtPassTexture(passDesc, 0, mtDrawableTexture(drawable)); // Set the render target
  mtPassLoadAction(passDesc, 0, MtLoadActionClear);
  mtPassClearColor(passDesc, 0, 1, 0, 0, 1.0);

  // Now create the encoder.
  MtRenderCommandEncoder *encoder = mtNewRenderCommandEncoder(g_metal.commandBuffer, passDesc);
  PH_ASSERT(encoder);

  mtSetRenderState(encoder, g_metal.pip);
  mtVertexBytes(encoder, triangleVertices, sizeof(triangleVertices), 0);
  mtDrawPrims(encoder, MtPrimitiveTypeTriangle, 0, 3);

  mtCommandBufferPresentDrawable(g_metal.commandBuffer, drawable);
  mtCommandEncoderEndEncoding(encoder);
  mtCommandBufferCommit(g_metal.commandBuffer);
}

void metal_clear() {
  // mtPassLoadAction(rpd, 0, MTLLoadActionClear);
}
void metal_end_pass() {}
void metal_submit() {
  // MtCommandEncoder* ce = mtNewRenderCommandEncoder(g_metal.commandBuffer,
  // rpd); mtCommandEncoderEndEncoding(ce);
  // mtCommandBufferPresentDrawable(g_metal.commandBuffer, g_metal.drawable);
  // mtCommandBufferCommit(g_metal.commandBuffer);
}

void create_metal_graphics() {
  if (g_graphics.shutdown != NULL) {
    g_graphics.shutdown();
  }
  g_graphicsCtx.api = METAL;
  g_graphics.init = metal_init;
  g_graphics.shutdown = metal_shutdown;

  g_graphics.begin_pass = metal_begin_pass;
  g_graphics.clear = metal_clear;
  g_graphics.end_pass = metal_end_pass;
  g_graphics.submit = metal_submit;
};