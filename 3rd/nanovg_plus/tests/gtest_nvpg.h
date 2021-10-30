#ifndef __GTEST_NVPG_H__
#define __GTEST_NVPG_H__
#include "nanovg_plus.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _gtest_nvgp_ctx_t {
  void* data;
  int type;
  int x;
  int y;
  int w; 
  int h; 
  int stride; 
  int image_flags;
  int image;
  int edge_anti_alias;
  float width;
  float height; 
  float pixel_ratio;
  int line_cap;
  int line_join;
  nvgp_paint_t paint;
  nvgp_scissor_t scissor;
  float fringe;
  float bounds[4];
  nvgp_darray_t paths;
  float stroke_width;
  nvgp_vertex_t* verts;
  uint32_t nverts;
  int clear_cache;

  int test_info;
} gtest_nvgp_ctx_t;

#define END_FRAME_INFO        0xff
#define RENDER_CANCEL_INFO    0xee
#define DESTROY_INFO          0xcc

const nvgp_vtable_t* nvgp_gtest_vtable();

#ifdef __cplusplus
}
#endif

#endif