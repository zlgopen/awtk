#include "tkc/utils.h"

static enum NVGtexture bitmap_format_to_nanovg(bitmap_format_t format) {
  enum NVGtexture f = NVG_TEXTURE_BGRA;

  switch (format) {
    case BITMAP_FMT_RGBA8888: {
      f = NVG_TEXTURE_RGBA;
      break;
    }
    case BITMAP_FMT_BGRA8888: {
      f = NVG_TEXTURE_BGRA;
      break;
    }
    case BITMAP_FMT_BGR888: {
      f = NVG_TEXTURE_BGR;
      break;
    }
    case BITMAP_FMT_RGB888: {
      f = NVG_TEXTURE_RGB;
      break;
    }
    case BITMAP_FMT_BGR565: {
      f = NVG_TEXTURE_BGR565;
      break;
    }
    case BITMAP_FMT_RGB565: {
      f = NVG_TEXTURE_RGB565;
      break;
    }
    default: {
      assert(!"not supported format");
      break;
    }
  }

  return f;
}

static ret_t vgcanvas_nanovg_create_fbo(vgcanvas_t* vgcanvas, framebuffer_object_t* fbo) {
  return RET_NOT_IMPL;
}

static ret_t vgcanvas_nanovg_destroy_fbo(vgcanvas_t* vgcanvas, framebuffer_object_t* fbo) {
  return RET_NOT_IMPL;
}

static ret_t vgcanvas_nanovg_bind_fbo(vgcanvas_t* vgcanvas, framebuffer_object_t* fbo) {
  return RET_NOT_IMPL;
}

static ret_t vgcanvas_nanovg_fbo_to_imageData(vgcanvas_t* vgcanvas, framebuffer_object_t* fbo,
                                              bitmap_t* img, rect_t* r) {
  return RET_NOT_IMPL;
}

static ret_t vgcanvas_nanovg_unbind_fbo(vgcanvas_t* vgcanvas, framebuffer_object_t* fbo) {
  return RET_NOT_IMPL;
}

static ret_t vgcanvas_nanovg_fbo_to_bitmap(vgcanvas_t* vgcanvas, framebuffer_object_t* fbo,
                                           bitmap_t* img, rect_t* r) {
  return RET_NOT_IMPL;
}

static ret_t vgcanvas_nanovg_reinit(vgcanvas_t* vgcanvas, uint32_t w, uint32_t h, uint32_t stride,
                                    bitmap_format_t format, void* data) {
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vgcanvas;
  NVGcontext* vg = canvas->vg;

  vgcanvas->w = w;
  vgcanvas->h = h;
  vgcanvas->format = format;
  vgcanvas->stride = stride;
  vgcanvas->buff = (uint32_t*)data;
  nvgReinitAgge(vg, w, h, stride, bitmap_format_to_nanovg(format), data);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_begin_frame(vgcanvas_t* vgcanvas, rect_t* dirty_rect) {
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vgcanvas;
  NVGcontext* vg = canvas->vg;

  nvgBeginFrame(vg, vgcanvas->w, vgcanvas->h, vgcanvas->ratio);

  return RET_OK;
}

static ret_t vgcanvas_nanovg_end_frame(vgcanvas_t* vgcanvas) {
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vgcanvas;
  NVGcontext* vg = canvas->vg;

  nvgEndFrame(vg);

  return RET_OK;
}

static int vgcanvas_nanovg_ensure_image(vgcanvas_nanovg_t* canvas, bitmap_t* img) {
  int32_t i = 0;
  int32_t j = 0;
  int32_t f = 0;
  uint32_t size = 0;
  uint8_t* img_data = NULL;
  uint32_t bpp = bitmap_get_bpp(img);

  if (img->flags & BITMAP_FLAG_TEXTURE) {
    if (img->specific != 0) {
      i = nvgFindTextureRaw(canvas->vg, img->specific);
      if (i > 0) {
        return i;
      }
    }
  }

  switch (img->format) {
    case BITMAP_FMT_RGBA8888: {
      f = NVG_TEXTURE_RGBA;
      break;
    }
    case BITMAP_FMT_BGRA8888: {
      f = NVG_TEXTURE_BGRA;
      break;
    }
    case BITMAP_FMT_BGR565: {
      f = NVG_TEXTURE_BGR565;
      break;
    }
    case BITMAP_FMT_RGB888: {
      f = NVG_TEXTURE_RGB;
      break;
    }
    default: { assert(!"not supported format"); }
  }

  img_data = bitmap_lock_buffer_for_read(img);

  i = nvgCreateImageRaw(canvas->vg, img->w, img->h, f, img->line_length, NVG_IMAGE_NEAREST,
                        img_data);

  if (i >= 0) {
    img->flags |= BITMAP_FLAG_TEXTURE;
    img->specific = img_data;
  }
  bitmap_unlock_buffer(img);

  return i;
}

static ret_t vgcanvas_nanovg_destroy(vgcanvas_t* vgcanvas) {
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vgcanvas;

  nvgDeleteInternal(canvas->vg);
  TKMEM_FREE(vgcanvas);

  return RET_OK;
}
