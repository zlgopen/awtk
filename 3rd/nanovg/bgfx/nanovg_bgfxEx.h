#ifndef NANOVG_BGFXEX_H_HEADER_GUARD
#define NANOVG_BGFXEX_H_HEADER_GUARD

#include <bgfx/bgfx.h>

namespace bx { struct AllocatorI; }

///
NVGcontext* nvgCreate(int32_t _edgeaa, bgfx::ViewId _viewId, bx::AllocatorI* _allocator);
///
void nvgSetViewId(NVGcontext* _ctx, bgfx::ViewId _viewId);

///
uint16_t nvgGetViewId(struct NVGcontext* _ctx);

#endif