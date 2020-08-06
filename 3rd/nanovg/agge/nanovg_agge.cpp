#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <agge/path.h>
#include <agge/stroke.h>
#include <agge/bitmap.h>
#include <agge/clipper.h>
#include <agge/renderer.h>
#include <agge/rasterizer.h>
#include <agge/raw_bitmap.h>
#include <agge/nanovg_agge.h>
#include <agge/nanovg_vertex.h>
#include <agge/filling_rules.h>
#include <agge/stroke_features.h>
#include <agge/blenders_generic.h>
#include <agge/vector_rasterizer.h>
#include <agge/nanovg_image_blender.h>
#include <agge/blender_linear_gradient.h>
#include <agge/blender_radial_gradient.h>

template <typename T>
agge::rect<T> mkrect(T x1, T y1, T x2, T y2) {
  agge::rect<T> r = {x1, y1, x2, y2};

  return r;
}

struct AGGENVGtexture {
  int id;
  int tex;
  int width;
  int height;
  int type;
  int flags;
  int stride;
  const uint8_t* data;
};
typedef struct AGGENVGtexture AGGENVGtexture;

struct AGGENVGcontext {
  AGGENVGcontext() {
    this->w = 0;
    this->h = 0;
    this->data = NULL;
    this->textures = NULL;
    this->ntextures = 0;
    this->ctextures = 0;
    this->textureId = 0;

    this->line_style.set_cap(agge::caps::butt());
    this->line_style.set_join(agge::joins::bevel());
  }

  ~AGGENVGcontext() {
    free(this->textures);
    this->textures = NULL;
  }

  int ntextures;
  int ctextures;
  int textureId;
  AGGENVGtexture* textures;

  /*fill/stroke color*/
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;

  /*frame buffer*/
  uint32_t w;
  uint32_t h;
  uint32_t stride;
  uint8_t* data;
  enum NVGtexture format;

  /*agge related*/
  agge::renderer ren;
  agge::stroke line_style;
  agge::rasterizer<agge::clipper<int> > ras;
};

static int aggenvg__renderFindTexture(void* uptr, const void* data);

static int aggenvg__maxi(int a, int b) {
  return a > b ? a : b;
}

static void aggenvg__setLineJoin(void* uptr, int lineCap)
{
    AGGENVGcontext* agge = (AGGENVGcontext*)uptr;
    if(lineCap == NVG_MITER)
      agge->line_style.set_join(agge::joins::miter());
    else if(lineCap == NVG_ROUND)
      agge->line_style.set_join(agge::joins::round());
    else if(lineCap == NVG_BEVEL)
      agge->line_style.set_join(agge::joins::bevel());
}

static void aggenvg__setLineCap(void* uptr, int lineCap)
{
    AGGENVGcontext* agge = (AGGENVGcontext*)uptr;
    if(lineCap == NVG_BUTT)
      agge->line_style.set_cap(agge::caps::butt());
    else if(lineCap == NVG_ROUND)
      agge->line_style.set_cap(agge::caps::round());
    else if(lineCap == NVG_SQUARE)
      agge->line_style.set_cap(agge::caps::square());
}

static void aggenvg__clearCacheTexture(AGGENVGcontext* agge) {
  AGGENVGtexture* textures = NULL;
  int ntextures = agge->ntextures;
  int size = sizeof(AGGENVGtexture) * ntextures;

  if (agge->textures != NULL && ntextures > 0) {
    textures = (AGGENVGtexture*)malloc(size);
    memcpy(textures, agge->textures, size);

    free(agge->textures);
    agge->textures = textures;
    agge->ntextures = ntextures;
    agge->ctextures = ntextures;
  }
}

static AGGENVGtexture* aggenvg__allocTexture(AGGENVGcontext* agge) {
  int i;
  AGGENVGtexture* tex = NULL;

  for (i = 0; i < agge->ntextures; i++) {
    if (agge->textures[i].id == 0) {
      tex = &agge->textures[i];
      break;
    }
  }

  if (tex == NULL) {
    if (agge->ntextures + 1 > agge->ctextures) {
      AGGENVGtexture* textures;
      int ctextures =
          aggenvg__maxi(agge->ntextures + 1, 4) + agge->ctextures / 2;  // 1.5x Overallocate
      textures = (AGGENVGtexture*)realloc(agge->textures, sizeof(AGGENVGtexture) * ctextures);
      if (textures == NULL) return NULL;
      agge->textures = textures;
      agge->ctextures = ctextures;
    }
    tex = &agge->textures[agge->ntextures++];
  }

  memset(tex, 0, sizeof(*tex));
  tex->id = ++agge->textureId;

  return tex;
}

static AGGENVGtexture* aggenvg__findTexture(AGGENVGcontext* agge, int id) {
  int i;
  for (i = 0; i < agge->ntextures; i++)
    if (agge->textures[i].id == id) return &agge->textures[i];
  return NULL;
}

static int aggenvg__deleteTexture(AGGENVGcontext* agge, int id) {
  int i;
  for (i = 0; i < agge->ntextures; i++) {
    if (agge->textures[i].id == id) {
      memset(&agge->textures[i], 0, sizeof(agge->textures[i]));
      return 1;
    }
  }
  return 0;
}

static int aggenvg__renderCreate(void* uptr) {
  NVG_NOTUSED(uptr);
  return 1;
}

static int aggenvg__renderCreateTexture(void* uptr, int type, int w, int h, int stride, int imageFlags,
                                        const unsigned char* data) {
  AGGENVGtexture* tex = NULL;
  AGGENVGcontext* agge = (AGGENVGcontext*)uptr;
  int id = aggenvg__renderFindTexture(agge, data);
  if(id > 0) {
    tex = aggenvg__findTexture(agge, id);
  }
  if (tex == NULL) {
    tex = aggenvg__allocTexture(agge);
  }

  if (tex == NULL) return 0;

  tex->width = w;
  tex->height = h;
  tex->type = type;
  tex->data = data;
  tex->stride = stride;
  tex->flags = imageFlags;

  return tex->id;
}

static int aggenvg__renderDeleteTexture(void* uptr, int image) {
  AGGENVGcontext* agge = (AGGENVGcontext*)uptr;
  return aggenvg__deleteTexture(agge, image);
}

static int aggenvg__renderUpdateTexture(void* uptr, int image, int x, int y, int w, int h,
                                        const unsigned char* data) {
  NVG_NOTUSED(x);
  NVG_NOTUSED(y);
  NVG_NOTUSED(w);
  NVG_NOTUSED(h);
  AGGENVGcontext* agge = (AGGENVGcontext*)uptr;
  AGGENVGtexture* tex = aggenvg__findTexture(agge, image);
  if (tex == NULL) return 0;

  tex->data = data;

  return 1;
}

static int aggenvg__renderFindTexture(void* uptr, const void* data) {
  int i = 0;
  AGGENVGtexture* tex = NULL;
  AGGENVGcontext* agge = (AGGENVGcontext*)uptr;
  for (; i < agge->ntextures; i++) {
    if (agge->textures[i].data == data) {
      tex = &agge->textures[i];
      break;
    } 
  }
  if (tex == NULL) {
    return -1;
  } 
  return tex->id;
}

static int aggenvg__renderGetTextureSize(void* uptr, int image, int* w, int* h) {
  AGGENVGcontext* agge = (AGGENVGcontext*)uptr;
  AGGENVGtexture* tex = aggenvg__findTexture(agge, image);
  if (tex == NULL) return 0;
  *w = tex->width;
  *h = tex->height;

  return 1;
}

static void aggenvg__renderViewport(void* uptr, float width, float height, float devicePixelRatio) {
  NVG_NOTUSED(uptr);
  NVG_NOTUSED(width);
  NVG_NOTUSED(height);
  NVG_NOTUSED(devicePixelRatio);
}

static void aggenvg__renderCancel(void* uptr) {
  NVG_NOTUSED(uptr);
}

static void aggenvg__renderFlush(void* uptr) {
  NVG_NOTUSED(uptr);
}

static void prepareRasterizer(AGGENVGcontext* agge, NVGscissor* scissor, NVGpaint* paint) {
  agge::rasterizer<agge::clipper<int> >& ras = agge->ras;

  agge::real_t clip_w = scissor->extent[0] * 2;
  agge::real_t clip_h = scissor->extent[1] * 2;
  agge::real_t clip_x = scissor->xform[4] - scissor->extent[0];
  agge::real_t clip_y = scissor->xform[5] - scissor->extent[1];
  agge::rect<agge::real_t> clip_r = mkrect(clip_x, clip_y, clip_x + clip_w, clip_y + clip_h);

  ras.reset();
  if (clip_w > 0 && clip_h > 0) {
    ras.set_clipping(clip_r);
  }

  agge->r = paint->innerColor.r * 0xff;
  agge->g = paint->innerColor.g * 0xff;
  agge->b = paint->innerColor.b * 0xff;
  agge->a = paint->innerColor.a * 0xff;
}

static agge::pixel32_rgba to_pixel32_rgba(NVGcolor rgba) {
  return agge::pixel32_rgba(rgba.r * 255, rgba.g * 255, rgba.b * 255, rgba.a * 255);
}

template <typename PixelT>
void renderPaint(AGGENVGcontext* agge, NVGpaint* paint) {
  agge::renderer& ren = agge->ren;
  agge::rasterizer<agge::clipper<int> >& ras = agge->ras;
  agge::bitmap<PixelT, agge::raw_bitmap> surface(agge->w, agge->h, agge->stride, agge->data);

  if (paint->image > 0) {
    float invxform[6];
    AGGENVGtexture* tex = aggenvg__findTexture(agge, paint->image);
    if(tex == NULL) return;
    nvgTransformInverse(invxform, paint->xform);

    switch (tex->type) {
      case NVG_TEXTURE_RGBA: {
        typedef agge::bitmap<agge::pixel32_rgba, agge::raw_bitmap> rgba_bitmap_t;
        rgba_bitmap_t src(tex->width, tex->height, tex->stride, tex->flags, (uint8_t*)(tex->data));
        agge::nanovg_image_blender<PixelT, rgba_bitmap_t> color(&src, (float*)invxform, paint->innerColor.a);
        ren(surface, 0, ras, color, agge::winding<>());
        break;
      }
      case NVG_TEXTURE_BGRA: {
        typedef agge::bitmap<agge::pixel32_bgra, agge::raw_bitmap> bgra_bitmap_t;
        bgra_bitmap_t src(tex->width, tex->height, tex->stride, tex->flags, (uint8_t*)(tex->data));
        agge::nanovg_image_blender<PixelT, bgra_bitmap_t> color(&src, (float*)invxform, paint->innerColor.a);
        ren(surface, 0, ras, color, agge::winding<>());
        break;
      }
      case NVG_TEXTURE_BGR565: {
        typedef agge::bitmap<agge::pixel16_bgr565, agge::raw_bitmap> bgr565_bitmap_t;
        bgr565_bitmap_t src(tex->width, tex->height, tex->stride, (uint8_t*)(tex->data));
        agge::nanovg_image_blender<PixelT, bgr565_bitmap_t> color(&src, (float*)invxform, paint->innerColor.a);
        ren(surface, 0, ras, color, agge::winding<>());
        break;
      }
      case NVG_TEXTURE_RGB: {
        typedef agge::bitmap<agge::pixel24_rgb, agge::raw_bitmap> rgb_bitmap_t;
        rgb_bitmap_t src(tex->width, tex->height, tex->stride, (uint8_t*)(tex->data));
        agge::nanovg_image_blender<PixelT, rgb_bitmap_t> color(&src, (float*)invxform, paint->innerColor.a);
        ren(surface, 0, ras, color, agge::winding<>());
        break;
      }
      default: {
        assert(!"not supported format");
        break;
      }
    }
  } else {
    if(memcmp(&(paint->innerColor), &(paint->outerColor), sizeof(paint->outerColor)) == 0) {
      agge::blender_solid_color_rgb<PixelT> color(agge->r, agge->g, agge->b, agge->a);
      ren(surface, 0, ras, color, agge::winding<>());
    } else if(paint->radius == 0) {
      const float large = 1e5;
      float dx = paint->xform[2];
      float dy = paint->xform[3];
      float d = (paint->extent[1] - large) * 2;
      float sx = paint->xform[4] + dx*large;
      float sy = paint->xform[5] + dy*large;
      float ex = sx + d * dx;
      float ey = sy + d * dy;
      agge::pixel32_rgba ic = to_pixel32_rgba(paint->innerColor);
      agge::pixel32_rgba oc = to_pixel32_rgba(paint->outerColor);
      agge::blender_linear_gradient<PixelT> color(sx, sy, ex, ey, ic, oc);

      ren(surface, 0, ras, color, agge::winding<>());
    } else {
      float cx = paint->xform[4];
      float cy = paint->xform[5];
      float inr = paint->radius - paint->feather/2;
      float outr = paint->radius + paint->feather/2;
      
      agge::pixel32_rgba ic = to_pixel32_rgba(paint->innerColor);
      agge::pixel32_rgba oc = to_pixel32_rgba(paint->outerColor);
      agge::blender_radial_gradient<PixelT> color(cx, cy, inr, outr, ic, oc);

      ren(surface, 0, ras, color, agge::winding<>());
    }
  }
}

template <typename PixelT>
void renderFill(void* uptr, NVGpaint* paint, NVGcompositeOperationState compositeOperation,
                NVGscissor* scissor, float fringe, const float* bounds, const NVGpath* paths,
                int npaths) {
  AGGENVGcontext* agge = (AGGENVGcontext*)uptr;
  agge::rasterizer<agge::clipper<int> >& ras = agge->ras;

  prepareRasterizer(agge, scissor, paint);

  for (int i = 0; i < npaths; i++) {
    const NVGpath* p = paths + i;
    for (int j = 0; j < p->nfill; j++) {
      const NVGvertex* v = p->fill + j;
      if (j == 0) {
        ras.move_to(v->x, v->y);
      } else {
        ras.line_to(v->x, v->y);
      }
    }
    ras.close_polygon();
  }

  ras.sort();
  renderPaint<PixelT>(agge, paint);
}

template <typename PixelT>
void renderStroke(void* uptr, NVGpaint* paint, NVGcompositeOperationState compositeOperation,
                  NVGscissor* scissor, float fringe, float strokeWidth, const NVGpath* paths,
                  int npaths) {
  if(paths->count <= 0) return; 
  AGGENVGcontext* agge = (AGGENVGcontext*)uptr;
  agge::stroke& line_style = agge->line_style;
  agge::rasterizer<agge::clipper<int> >& ras = agge->ras;

  line_style.width(strokeWidth);
  prepareRasterizer(agge, scissor, paint);

  for (int i = 0; i < npaths; i++) {
    const NVGpath* p = paths + i;
    agge::nanovg_vertex::iterator iter(p->stroke, p->nstroke);
    agge::path_generator_adapter<agge::nanovg_vertex::iterator, agge::stroke> path_gen(iter,
                                                                                       line_style);
    add_path(ras, path_gen);
  }

  ras.sort();
  renderPaint<PixelT>(agge, paint);
}

template <typename PixelT>
void renderTriangles(void* uptr, NVGpaint* paint, NVGcompositeOperationState compositeOperation,
                     NVGscissor* scissor, const NVGvertex* verts, int nverts) {
  /*XXX: not used yet*/
}

static void aggenvg__renderDelete(void* uptr) {
  AGGENVGcontext* agge = (AGGENVGcontext*)uptr;
  if (agge == NULL) return;

  delete agge;
}

static int aggenvg__clearCache(void* uptr) {
  AGGENVGcontext* agge = (AGGENVGcontext*)uptr;
  if (agge == NULL) return -1;
  agge::rasterizer<agge::clipper<int> >& ras = agge->ras;
  ras.clear_cache();
  aggenvg__clearCacheTexture(agge);
  return 0;
}

static void nvgInitAGGE(AGGENVGcontext* agge, NVGparams* params, uint32_t w, uint32_t h, uint32_t stride,
                        enum NVGtexture format, uint8_t* data) {
  agge->w = w;
  agge->h = h;
  agge->data = data;
  agge->stride = stride;
  agge->format = format;
  params->renderTriangles = NULL;

  switch (agge->format) {
    case NVG_TEXTURE_RGBA: {
      params->renderStroke = renderStroke<agge::pixel32_rgba>;
      params->renderFill = renderFill<agge::pixel32_rgba>;
      break;
    }
    case NVG_TEXTURE_BGRA: {
      params->renderStroke = renderStroke<agge::pixel32_bgra>;
      params->renderFill = renderFill<agge::pixel32_bgra>;
      break;
    }
    case NVG_TEXTURE_RGB: {
      params->renderStroke = renderStroke<agge::pixel24_rgb>;
      params->renderFill = renderFill<agge::pixel24_rgb>;
      break;
    }
    case NVG_TEXTURE_BGR: {
      params->renderStroke = renderStroke<agge::pixel24_bgr>;
      params->renderFill = renderFill<agge::pixel24_bgr>;
      break;
    }
    case NVG_TEXTURE_BGR565: {
      params->renderStroke = renderStroke<agge::pixel16_bgr565>;
      params->renderFill = renderFill<agge::pixel16_bgr565>;
      break;
    }
    case NVG_TEXTURE_RGB565: {
      params->renderStroke = renderStroke<agge::pixel16_rgb565>;
      params->renderFill = renderFill<agge::pixel16_rgb565>;
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
  AGGENVGcontext* agge = (AGGENVGcontext*)(params->userPtr);

  nvgInitAGGE(agge, params, w, h, stride, format, data);
}

NVGcontext* nvgCreateAGGE(uint32_t w, uint32_t h, uint32_t stride, enum NVGtexture format, uint8_t* data) {
  NVGparams params;
  NVGcontext* ctx = NULL;
  AGGENVGcontext* agge = new AGGENVGcontext();

  if (agge == NULL) goto error;

  memset(&params, 0, sizeof(params));
  params.setLineJoin = aggenvg__setLineJoin;
  params.setLineCap = aggenvg__setLineCap;
  params.renderCreate = aggenvg__renderCreate;
  params.findTexture = aggenvg__renderFindTexture;
  params.renderCreateTexture = aggenvg__renderCreateTexture;
  params.renderDeleteTexture = aggenvg__renderDeleteTexture;
  params.renderUpdateTexture = aggenvg__renderUpdateTexture;
  params.renderGetTextureSize = aggenvg__renderGetTextureSize;
  params.renderViewport = aggenvg__renderViewport;
  params.renderCancel = aggenvg__renderCancel;
  params.renderFlush = aggenvg__renderFlush;
  params.renderDelete = aggenvg__renderDelete;
  params.clearCache = aggenvg__clearCache;
  params.userPtr = agge;
  params.edgeAntiAlias = 1;

  nvgInitAGGE(agge, &params, w, h, stride, format, data);

  ctx = nvgCreateInternal(&params);
  if (ctx == NULL) goto error;

  return ctx;

error:
  if (ctx != NULL) nvgDeleteInternal(ctx);
  return NULL;
}

void nvgDeleteAGGE(NVGcontext* ctx) {
  nvgDeleteInternal(ctx);
}
