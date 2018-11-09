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

#ifdef __cplusplus
}
#endif

#endif // NANOVG_BGFX_H_HEADER_GUARD
