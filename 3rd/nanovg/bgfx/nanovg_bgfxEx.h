#ifndef NANOVG_BGFXEX_H_HEADER_GUARD
#define NANOVG_BGFXEX_H_HEADER_GUARD

#include <bgfx/bgfx.h>

namespace bx { struct AllocatorI; }

typedef struct NVGcontext NVGcontext;

struct NVGLUframebuffer_bgfx
{
  NVGcontext* ctx;
  bgfx::FrameBufferHandle handle;
  int image;
  bgfx::ViewId viewId;
};

///
NVGcontext* nvgCreate(int32_t _edgeaa, bgfx::ViewId _viewId, bx::AllocatorI* _allocator);
///
void nvgSetViewId(NVGcontext* _ctx, bgfx::ViewId _viewId);

///
uint16_t nvgGetViewId(struct NVGcontext* _ctx);

// Helper functions to create bgfx framebuffer to render to.
// Example:
//		float scale = 2;
//		NVGLUframebuffer* fb = nvgluCreateFramebuffer(ctx, 100 * scale, 100 * scale, 0);
//		nvgluSetViewFramebuffer(VIEW_ID, fb);
//		nvgluBindFramebuffer(fb);
//		nvgBeginFrame(ctx, 100, 100, scale);
//		// renders anything offscreen
//		nvgEndFrame(ctx);
//		nvgluBindFramebuffer(NULL);
//
//		// Pastes the framebuffer rendering.
//		nvgBeginFrame(ctx, 1024, 768, scale);
//		NVGpaint paint = nvgImagePattern(ctx, 0, 0, 100, 100, 0, fb->image, 1);
//		nvgBeginPath(ctx);
//		nvgRect(ctx, 0, 0, 100, 100);
//		nvgFillPaint(ctx, paint);
//		nvgFill(ctx);
//		nvgEndFrame(ctx);

///
NVGLUframebuffer_bgfx* nvgluCreateFramebuffer(NVGcontext* _ctx, int32_t _width, int32_t _height, int32_t _imageFlags, bgfx::ViewId _viewId);

///
NVGLUframebuffer_bgfx* nvgluCreateFramebuffer(NVGcontext* _ctx, int32_t _width, int32_t _height, int32_t _imageFlags);

///
void nvgluBindFramebuffer(NVGLUframebuffer_bgfx* _framebuffer);

///
void nvgluDeleteFramebuffer(NVGLUframebuffer_bgfx* _framebuffer);

///
void nvgluSetViewFramebuffer(bgfx::ViewId _viewId, NVGLUframebuffer_bgfx* _framebuffer);

#endif