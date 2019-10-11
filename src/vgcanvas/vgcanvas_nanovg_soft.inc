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

static ret_t vgcanvas_nanovg_unbind_fbo(vgcanvas_t* vgcanvas, framebuffer_object_t* fbo) {
  return RET_NOT_IMPL;
}

static ret_t vgcanvas_nanovg_reinit(vgcanvas_t* vgcanvas, uint32_t w, uint32_t h, uint32_t stride,
                                    bitmap_format_t format, void* data) {
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vgcanvas;
  NVGcontext* vg = canvas->vg;

  vgcanvas->w = w;
  vgcanvas->h = h;
  vgcanvas->format = format;
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
  uint8_t* data = NULL;
  uint32_t bpp = bitmap_get_bpp(img);

  if (img->flags & BITMAP_FLAG_TEXTURE) {
    i = tk_pointer_to_int(img->specific);

    return i;
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

  if (bpp * img->w == img->line_length) {
    data = img->data;
  } else {
    size = bpp * img->w * img->h;
    size = TK_ROUND_TO(size, BITMAP_ALIGN_SIZE) + BITMAP_ALIGN_SIZE;
    data = (uint8_t*)TKMEM_ALLOC(size);
    memset(data, 0x00, size);
    for (j = 0; j < img->h; j++) {
      memcpy(data + j * img->w * bpp, img->data + j * img->line_length, img->w * bpp);
    }
  }

  i = nvgCreateImageRaw(canvas->vg, img->w, img->h, f, NVG_IMAGE_NEAREST, img->data);

  if (data != img->data) {
    TKMEM_FREE(data);
  }

  if (i >= 0) {
    img->flags |= BITMAP_FLAG_TEXTURE;
    img->specific = tk_pointer_from_int(i);
    image_manager_update_specific(image_manager(), img);
  }

  return i;
}

static ret_t vgcanvas_nanovg_destroy(vgcanvas_t* vgcanvas) {
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vgcanvas;

  nvgDeleteInternal(canvas->vg);
  TKMEM_FREE(vgcanvas);

  return RET_OK;
}
