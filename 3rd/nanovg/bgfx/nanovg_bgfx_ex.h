#ifndef NANOVG_BGFX_EX_H_
#define NANOVG_BGFX_EX_H_

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


struct NVGLUframebuffer;

#ifdef __cplusplus
extern "C" {
#endif

///sdl entry
NVGcontext* nvgCreateBGFX(int32_t _edgeaa, uint16_t _viewId,uint32_t _width, uint32_t _height, SDL_Window* _window);

void nvgDeleteBGFX(NVGcontext* _ctx);

uint32_t renderBGFXFrame(int32_t _msecs);

void setBGFXViewRect(uint16_t _viewId, uint16_t _x, uint16_t _y, uint16_t _width, uint16_t _height);

void touchBGFX(uint16_t _viewId);

uint32_t frameBGFX(bool _capture);

void resetBGFX(uint32_t _width, uint32_t _height, uint32_t _flags);

void* nvgluCreateFramebufferByViewId(NVGcontext* _ctx, int32_t _width, int32_t _height, int32_t _imageFlags, uint16_t _viewId);

///
void nvgluBindFramebufferEx(struct NVGLUframebuffer* _framebuffer);

///
void nvgluDeleteFramebufferEx(struct NVGLUframebuffer* _framebuffer);

uint32_t getBgfxFboViewId(struct NVGLUframebuffer* _framebuffer);

uint32_t getBgfxFboImage(struct NVGLUframebuffer* _framebuffer);


#ifdef __cplusplus
}
#endif

#endif 