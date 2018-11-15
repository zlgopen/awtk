/*
 * Copyright 2011-2018 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#ifndef NANOVG_BGFX_H_HEADER_GUARD
#define NANOVG_BGFX_H_HEADER_GUARD
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "nanovg.h"
#include <SDL.h>

#ifndef M_PI
#define M_PI 3.1415926f
#endif /*M_PI*/

typedef struct _NVGLUframebuffer_bgfx
{
  NVGcontext* ctx;
  uint16_t handle;
  int image;
  uint16_t viewId;
} NVGLUframebuffer_bgfx ;

#ifdef __cplusplus
extern "C" {
#endif

///sdl entry
NVGcontext* nvgCreateBGFX(int32_t _edgeaa, uint16_t _viewId,uint32_t _width, uint32_t _height, SDL_Window* _window);

///
void nvgDeleteBGFX(NVGcontext* _ctx);

uint32_t renderBGFXFrame(int32_t _msecs);

void setBGFXViewRect(uint16_t _viewId, uint16_t _x, uint16_t _y, uint16_t _width, uint16_t _height);

void touchBGFX(uint16_t _viewId);

uint32_t frameBGFX(bool _capture);

void resetBGFX(uint32_t _width, uint32_t _height, uint32_t _flags);

// Helper functions to create bgfx framebuffer to render to.
// Example:
//		float scale = 2;
//		NVGLUframebuffer_bgfx* fb = nvgluCreateFramebuffer(ctx, 100 * scale, 100 * scale, 0);
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
NVGLUframebuffer_bgfx* nvgluCreateFramebufferByViewId(NVGcontext* _ctx, int32_t _width, int32_t _height, int32_t _imageFlags, uint16_t _viewId);

///
NVGLUframebuffer_bgfx* nvgluCreateFramebuffer(NVGcontext* _ctx, int32_t _width, int32_t _height, int32_t _imageFlags);

///
void nvgluBindFramebuffer(NVGLUframebuffer_bgfx* _framebuffer_bgfx);

///
void nvgluDeleteFramebuffer(NVGLUframebuffer_bgfx* _framebuffer_bgfx);

///
void nvgluSetViewFramebuffer(uint16_t _viewId, NVGLUframebuffer_bgfx* _framebuffer_bgfx);

#ifdef __cplusplus
}
#endif

#endif // NANOVG_BGFX_H_HEADER_GUARD
