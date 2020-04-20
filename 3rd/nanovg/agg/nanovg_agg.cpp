#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "nanovg_agg.h"
#include "nanovg_vertex.h"

#include "agg_basics.h"
#include "agg_conv_stroke.h"
#include "agg_scanline_u.h"
#include "agg_pixfmt_rgb.h"
#include "agg_pixfmt_rgba.h"
#include "agg_pixfmt_rgb_packed.h"

#include "agg_platform_support.h"
#include "agg_rendering_buffer.h"
#include "agg_renderer_scanline.h"
#include "agg_rasterizer_scanline_aa.h"

#include "agg_image_accessors.h"
#include "agg_span_allocator.h"
#include "agg_span_image_filter_rgb.h"
#include "agg_span_image_filter_rgba.h"

struct AGGNVGtexture {
  int id;
  int tex;
  int width;
  int height;
  int stride;
  int type;
  int flags;
  const uint8_t* data;
};
typedef struct AGGNVGtexture AGGNVGtexture;

struct AGGNVGcontext {
  AGGNVGcontext() {
    this->w = 0;
    this->h = 0;
    this->data = NULL;
    this->textures = NULL;
    this->ntextures = 0;
    this->ctextures = 0;
    this->textureId = 0;
  }

  ~AGGNVGcontext() {
    free(this->textures);
    this->textures = NULL;
  }

  int ntextures;
  int ctextures;
  int textureId;
  AGGNVGtexture* textures;

  /*fill/stroke color*/
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;

  /*frame buffer*/
  int32_t w;
  int32_t h;
  int32_t stride;
  uint8_t* data;
  enum NVGtexture format;

  /*agg related*/
  agg::rendering_buffer rbuf;
  agg::rasterizer_scanline_aa<> ras;
};

static int aggnvg__maxi(int a, int b) {
  return a > b ? a : b;
}

static AGGNVGtexture* aggnvg__allocTexture(AGGNVGcontext* agg) {
  int i;
  AGGNVGtexture* tex = NULL;

  for (i = 0; i < agg->ntextures; i++) {
    if (agg->textures[i].id == 0) {
      tex = &agg->textures[i];
      break;
    }
  }

  if (tex == NULL) {
    if (agg->ntextures + 1 > agg->ctextures) {
      AGGNVGtexture* textures;
      int ctextures =
          aggnvg__maxi(agg->ntextures + 1, 4) + agg->ctextures / 2;  // 1.5x Overallocate
      textures = (AGGNVGtexture*)realloc(agg->textures, sizeof(AGGNVGtexture) * ctextures);
      if (textures == NULL) return NULL;
      agg->textures = textures;
      agg->ctextures = ctextures;
    }
    tex = &agg->textures[agg->ntextures++];
  }

  memset(tex, 0, sizeof(*tex));
  tex->id = ++agg->textureId;

  return tex;
}

static AGGNVGtexture* aggnvg__findTexture(AGGNVGcontext* agg, int id) {
  int i;
  for (i = 0; i < agg->ntextures; i++)
    if (agg->textures[i].id == id) return &agg->textures[i];
  return NULL;
}

static int aggnvg__deleteTexture(AGGNVGcontext* agg, int id) {
  int i;
  for (i = 0; i < agg->ntextures; i++) {
    if (agg->textures[i].id == id) {
      memset(&agg->textures[i], 0, sizeof(agg->textures[i]));
      return 1;
    }
  }
  return 0;
}

static int aggnvg__renderCreate(void* uptr) {
  NVG_NOTUSED(uptr);
  return 1;
}

static int aggnvg__renderCreateTexture(void* uptr, int type, int w, int h, int stride, int imageFlags,
                                        const unsigned char* data) {
  AGGNVGcontext* agg = (AGGNVGcontext*)uptr;
  AGGNVGtexture* tex = aggnvg__allocTexture(agg);

  if (tex == NULL) return 0;

  tex->width = w;
  tex->height = h;
  tex->type = type;
  tex->data = data;
  tex->stride = stride;
  tex->flags = imageFlags;

  return tex->id;
}

static int aggnvg__renderDeleteTexture(void* uptr, int image) {
  AGGNVGcontext* agg = (AGGNVGcontext*)uptr;
  return aggnvg__deleteTexture(agg, image);
}

static int aggnvg__renderUpdateTexture(void* uptr, int image, int x, int y, int w, int h,
                                        const unsigned char* data) {
  NVG_NOTUSED(x);
  NVG_NOTUSED(y);
  NVG_NOTUSED(w);
  NVG_NOTUSED(h);
  AGGNVGcontext* agg = (AGGNVGcontext*)uptr;
  AGGNVGtexture* tex = aggnvg__findTexture(agg, image);
  if (tex == NULL) return 0;

  tex->data = data;

  return 1;
}

static int aggnvg__renderGetTextureSize(void* uptr, int image, int* w, int* h) {
  AGGNVGcontext* agg = (AGGNVGcontext*)uptr;
  AGGNVGtexture* tex = aggnvg__findTexture(agg, image);
  if (tex == NULL) return 0;
  *w = tex->width;
  *h = tex->height;

  return 1;
}

static void aggnvg__renderViewport(void* uptr, float width, float height, float devicePixelRatio) {
  NVG_NOTUSED(uptr);
  NVG_NOTUSED(width);
  NVG_NOTUSED(height);
  NVG_NOTUSED(devicePixelRatio);
}

static void aggnvg__renderCancel(void* uptr) {
  NVG_NOTUSED(uptr);
}

static void aggnvg__renderFlush(void* uptr) {
  NVG_NOTUSED(uptr);
}

static void prepareRasterizer(AGGNVGcontext* agg, NVGscissor* scissor, NVGpaint* paint) {
  agg::rasterizer_scanline_aa<>& ras = agg->ras;
  agg::rendering_buffer& rbuf = agg->rbuf;
  float_t clip_w = scissor->extent[0] * 2;
  float_t clip_h = scissor->extent[1] * 2;
  float_t clip_x = scissor->xform[4] - scissor->extent[0];
  float_t clip_y = scissor->xform[5] - scissor->extent[1];

  rbuf.attach(agg->data, agg->w, agg->h, agg->stride);

  ras.reset();
  if(clip_w > 0 && clip_h > 0) {
    ras.clip_box(clip_x, clip_y, clip_x+clip_w, clip_y+clip_h);
  }

  agg->r = paint->innerColor.r * 0xff;
  agg->g = paint->innerColor.g * 0xff;
  agg->b = paint->innerColor.b * 0xff;
  agg->a = paint->innerColor.a * 0xff;
}

template <typename PixelT>
void renderPaint(AGGNVGcontext* agg, NVGpaint* paint) {
  agg::scanline_u8 sl;
  PixelT pixf(agg->rbuf);
  agg::span_allocator<agg::rgba8> sa; 
  agg::renderer_base<PixelT> ren(pixf);
  agg::rasterizer_scanline_aa<>& ras = agg->ras;
  
  if (paint->image > 0) {
    AGGNVGtexture* tex = aggnvg__findTexture(agg, paint->image);
    float invxform[6];
    int32_t img_w = tex->width;
    int32_t img_h = tex->height;
    int32_t img_stride = tex->stride;
    uint8_t* img_data = (uint8_t*)(tex->data);
    
    nvgTransformInverse(invxform, paint->xform);
    agg::trans_affine img_mtx(invxform[0], invxform[1], invxform[2], invxform[3], invxform[4], invxform[5]);
    agg::span_interpolator_linear<> interpolator(img_mtx);

    switch (tex->type) {
      case NVG_TEXTURE_RGBA: {
        agg::rendering_buffer img_rbuf(img_data, img_w, img_h, img_stride);
        agg::pixfmt_rgba32 img_pixf(img_rbuf);
        agg::span_image_filter_rgba_bilinear_clip<agg::pixfmt_rgba32, agg::span_interpolator_linear<> > sg(img_pixf, agg::rgba(0, 0, 0, 0), interpolator);
  
        agg::render_scanlines_aa(ras, sl, ren, sa, sg);
        break;
      }
      case NVG_TEXTURE_BGRA: {
        agg::rendering_buffer img_rbuf(img_data, img_w, img_h, img_stride);
        agg::pixfmt_bgra32 img_pixf(img_rbuf);
        agg::span_image_filter_rgba_bilinear_clip<agg::pixfmt_bgra32, agg::span_interpolator_linear<> > sg(img_pixf, agg::rgba(0, 0, 0, 0), interpolator);
  
        agg::render_scanlines_aa(ras, sl, ren, sa, sg);
        break;
      }
      case NVG_TEXTURE_BGR565: {
      /*FIXME:*/
        assert(!"not supported format");
        break;
      }
      case NVG_TEXTURE_RGB: {
        agg::rendering_buffer img_rbuf(img_data, img_w, img_h, img_stride);
        agg::pixfmt_rgb24 img_pixf(img_rbuf);
        agg::span_image_filter_rgb_bilinear_clip<agg::pixfmt_rgb24, agg::span_interpolator_linear<> > sg(img_pixf, agg::rgba(0, 0, 0, 0), interpolator);
  
        agg::render_scanlines_aa(ras, sl, ren, sa, sg);
        break;
      }
      case NVG_TEXTURE_BGR: {
        agg::rendering_buffer img_rbuf(img_data, img_w, img_h, img_stride);
        agg::pixfmt_bgr24 img_pixf(img_rbuf);
        agg::span_image_filter_rgb_bilinear_clip<agg::pixfmt_bgr24, agg::span_interpolator_linear<> > sg(img_pixf, agg::rgba(0, 0, 0, 0), interpolator);
  
        agg::render_scanlines_aa(ras, sl, ren, sa, sg);
        break;
      }
      default: {
        assert(!"not supported format");
        break;
      }
    }
  } else {
    agg::render_scanlines_aa_solid(ras, sl, ren, agg::srgba8(agg->r,agg->g,agg->b, agg->a));
  }

}

template <typename PixelT>
void renderFill(void* uptr, NVGpaint* paint, NVGcompositeOperationState compositeOperation,
                NVGscissor* scissor, float fringe, const float* bounds, const NVGpath* paths,
                int npaths) {
  AGGNVGcontext* agg = (AGGNVGcontext*)uptr;
  
  prepareRasterizer(agg, scissor, paint);
  
  agg::rasterizer_scanline_aa<>& ras = agg->ras;

  ras.reset();
  for (int i = 0; i < npaths; i++) {
    const NVGpath* p = paths + i;
    for (int j = 0; j < p->nfill; j++) {
      const NVGvertex* v = p->fill + j;
      if (j == 0) {
        ras.move_to_d(v->x, v->y);
      } else {
        ras.line_to_d(v->x, v->y);
      }
    }
    ras.close_polygon();
  }
  
  ras.sort();
  renderPaint<PixelT>(agg, paint);
}

template <typename PixelT>
void renderStroke(void* uptr, NVGpaint* paint, NVGcompositeOperationState compositeOperation,
                  NVGscissor* scissor, float fringe, float strokeWidth, const NVGpath* paths,
                  int npaths) {
  AGGNVGcontext* agg = (AGGNVGcontext*)uptr;
  
  prepareRasterizer(agg, scissor, paint);
  
  agg::rasterizer_scanline_aa<>& ras = agg->ras;

  ras.reset();
  for (int i = 0; i < npaths; i++) {
    const NVGpath* p = paths + i;

    agg::nanovg_vertex v(p->stroke, p->nstroke);
    agg::conv_stroke<agg::nanovg_vertex> path(v);
    path.line_cap(agg::round_cap);
    path.width(strokeWidth);
    ras.add_path(path);
  }

  ras.sort();
  renderPaint<PixelT>(agg, paint);
}

template <typename PixelT>
void renderTriangles(void* uptr, NVGpaint* paint, NVGcompositeOperationState compositeOperation,
                     NVGscissor* scissor, const NVGvertex* verts, int nverts) {
}

static void aggnvg__renderDelete(void* uptr) {
  AGGNVGcontext* agg = (AGGNVGcontext*)uptr;
  if (agg == NULL) return;

  delete agg;
}

static void nvgInitAGG(AGGNVGcontext* agg, NVGparams* params, uint32_t w, uint32_t h, uint32_t stride,
                        enum NVGtexture format, uint8_t* data) {
  agg->w = w;
  agg->h = h;
  agg->data = data;
  agg->format = format;
  agg->stride = stride;
  params->renderTriangles = NULL;

  switch (agg->format) {
    case NVG_TEXTURE_BGRA: {
      params->renderStroke = renderStroke<agg::pixfmt_bgra32>;
      params->renderFill = renderFill<agg::pixfmt_bgra32>;
      break;
    }
    case NVG_TEXTURE_RGBA: {
      params->renderStroke = renderStroke<agg::pixfmt_rgba32>;
      params->renderFill = renderFill<agg::pixfmt_rgba32>;
      break;
    }
    case NVG_TEXTURE_BGR: {
      params->renderStroke = renderStroke<agg::pixfmt_bgr24>;
      params->renderFill = renderFill<agg::pixfmt_bgr24>;
      break;
    }
    case NVG_TEXTURE_RGB: {
      params->renderStroke = renderStroke<agg::pixfmt_rgb24>;
      params->renderFill = renderFill<agg::pixfmt_rgb24>;
      break;
    }
    case NVG_TEXTURE_BGR565: {
      params->renderStroke = renderStroke<agg::pixfmt_rgb565>;
      params->renderFill = renderFill<agg::pixfmt_rgb565>;
      break;
    }
    default: {
      assert(!"not supported format");
      break;
    }
  }
}

void nvgReinitAgge(NVGcontext* ctx, uint32_t w, uint32_t h, uint32_t stride, enum NVGtexture format,
                   uint8_t* data) {
  NVGparams* params = nvgGetParams(ctx);
  AGGNVGcontext* agg = (AGGNVGcontext*)(params->userPtr);

  nvgInitAGG(agg, params, w, h, stride, format, data);
}

NVGcontext* nvgCreateAGG(uint32_t w, uint32_t h, uint32_t stride, enum NVGtexture format, uint8_t* data) {
  NVGparams params;
  NVGcontext* ctx = NULL;
  AGGNVGcontext* agg = new AGGNVGcontext();

  if (agg == NULL) goto error;

  memset(&params, 0, sizeof(params));
  params.renderCreate = aggnvg__renderCreate;
  params.renderCreateTexture = aggnvg__renderCreateTexture;
  params.renderDeleteTexture = aggnvg__renderDeleteTexture;
  params.renderUpdateTexture = aggnvg__renderUpdateTexture;
  params.renderGetTextureSize = aggnvg__renderGetTextureSize;
  params.renderViewport = aggnvg__renderViewport;
  params.renderCancel = aggnvg__renderCancel;
  params.renderFlush = aggnvg__renderFlush;
  params.renderDelete = aggnvg__renderDelete;
  params.userPtr = agg;
  params.edgeAntiAlias = 1;

  nvgInitAGG(agg, &params, w, h, stride, format, data);

  ctx = nvgCreateInternal(&params);
  if (ctx == NULL) goto error;

  return ctx;

error:
  if (ctx != NULL) nvgDeleteInternal(ctx);
  return NULL;
}

void nvgDeleteAGG(NVGcontext* ctx) {
  nvgDeleteInternal(ctx);
}
