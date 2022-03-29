#include "tkc/utils.h"
#include "base/types_def.h"
#include "vgcanvas_nanovg.inc" 
#include "base/vgcanvas_asset_manager.h"

static ret_t vgcanvas_asset_manager_nanovg_font_destroy(void* vg, const char* font_name, void* specific) {
  int32_t id = tk_pointer_to_int(specific);
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vg;
  if (canvas != NULL && canvas->vg != NULL && id >= 0) {
    nvgDeleteFontByName(canvas->vg, font_name);
  }
  return RET_OK;
}

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

static enum NVGorientation vgcanvas_nanovg_bitmap_orientation_to_NVGorientation(lcd_orientation_t orientation) {
  switch (orientation) {
  case LCD_ORIENTATION_0:
    return NVG_ORIENTATION_0;
  case LCD_ORIENTATION_90:
    return NVG_ORIENTATION_90;
  case LCD_ORIENTATION_180:
    return NVG_ORIENTATION_180;
  case LCD_ORIENTATION_270:
    return NVG_ORIENTATION_270;
  default :
    assert(!"not find orientation");
    break;
  }
  return NVG_ORIENTATION_0;
}

static ret_t vgcanvas_nanovg_create_fbo(vgcanvas_t* vgcanvas, uint32_t w, uint32_t h, bool_t custom_draw_model, framebuffer_object_t* fbo) {
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

static ret_t vgcanvas_nanovg_fbo_to_bitmap(vgcanvas_t* vgcanvas, framebuffer_object_t* fbo,
                                           bitmap_t* img, const rect_t* r) {
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

static ret_t vgcanvas_nanovg_begin_frame(vgcanvas_t* vgcanvas, const dirty_rects_t* dirty_rects) {
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vgcanvas;
  NVGcontext* vg = canvas->vg;
  system_info_t* info = system_info();

#ifdef WITH_FAST_LCD_PORTRAIT
  nvgBeginFrame(vg, vgcanvas->w, vgcanvas->h, vgcanvas->ratio, vgcanvas_nanovg_bitmap_orientation_to_NVGorientation(info->lcd_orientation));
#else
  nvgBeginFrame(vg, vgcanvas->w, vgcanvas->h, vgcanvas->ratio, NVG_ORIENTATION_0);
#endif

  return RET_OK;
}

static ret_t vgcanvas_nanovg_end_frame(vgcanvas_t* vgcanvas) {
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vgcanvas;
  NVGcontext* vg = canvas->vg;

  nvgEndFrame(vg);

  return RET_OK;
}

typedef struct _vgcanvas_nanovg_soft_texture_t {
  int32_t id;
  void* data;
}vgcanvas_nanovg_soft_texture_t;

static int vgcanvas_nanovg_bitmap_flag_to_NVGimageFlags(bitmap_flag_t flags) {

  if(flags & BITMAP_FLAG_PREMULTI_ALPHA) {
    return NVG_IMAGE_PREMULTIPLIED;
  }
  return NVG_IMAGE_NEAREST;
}

static ret_t vgcanvas_asset_manager_nanovg_bitmap_destroy(void* vg, void* specific) {
  vgcanvas_nanovg_soft_texture_t* texture = (vgcanvas_nanovg_soft_texture_t*)specific;
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vg;
  if (canvas != NULL && canvas->vg != NULL) {
    nvgDeleteImage(canvas->vg, texture->id);
  }
  TKMEM_FREE(texture);
  return RET_OK;
}

static int vgcanvas_nanovg_ensure_image(vgcanvas_nanovg_t* canvas, bitmap_t* img) {
  int flag = 0;
  int32_t i = 0;
  int32_t f = 0;
  uint8_t* img_data = NULL;
  vgcanvas_nanovg_soft_texture_t* texture = NULL;
  enum NVGorientation image_orientation = NVG_ORIENTATION_0;

  if (img->flags & BITMAP_FLAG_TEXTURE) {
    if (img->specific != 0) {
      ret_t reuslt = RET_FAIL;
      const void* specific = vgcanvas_asset_manager_get_image_specific(vgcanvas_asset_manager(), (vgcanvas_t*)canvas, img, &reuslt);
      if (reuslt == RET_OK && specific != NULL) {
        const vgcanvas_nanovg_soft_texture_t* tmp = (const vgcanvas_nanovg_soft_texture_t*)specific;
        i = nvgFindTextureRaw(canvas->vg, tmp->data);
        if (i > 0) {
          return i;
        }
      }
    }
  }

#ifdef WITH_FAST_LCD_PORTRAIT
  assert(img->orientation == system_info()->lcd_orientation);
#endif

  flag = (int)vgcanvas_nanovg_bitmap_flag_to_NVGimageFlags((bitmap_flag_t)(img->flags));
  image_orientation = vgcanvas_nanovg_bitmap_orientation_to_NVGorientation(img->orientation);

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

  i = nvgCreateImageRaw(canvas->vg, img->w, img->h, f, bitmap_get_physical_line_length(img), flag, image_orientation,
                        img_data);

  if (i >= 0) {
    texture = TKMEM_ZALLOC(vgcanvas_nanovg_soft_texture_t);
    texture->id = i;
    texture->data = img_data;

    img->flags |= BITMAP_FLAG_TEXTURE;
    vgcanvas_asset_manager_add_image(vgcanvas_asset_manager(), (vgcanvas_t*)canvas, img, texture);
  }
  bitmap_unlock_buffer(img);

  return i;
}

static ret_t vgcanvas_nanovg_soft_set_asset_manager(vgcanvas_t* vg) {
  return_value_if_fail(vg != NULL, RET_BAD_PARAMS);
  vgcanvas_asset_manager_add_vg(vgcanvas_asset_manager(), vg, vgcanvas_asset_manager_nanovg_bitmap_destroy, vgcanvas_asset_manager_nanovg_font_destroy);
  return RET_OK;
}

static ret_t vgcanvas_nanovg_soft_close_asset_manager(vgcanvas_t* vg) {
  return_value_if_fail(vg != NULL, RET_BAD_PARAMS);
  vgcanvas_asset_manager_remove_vg(vgcanvas_asset_manager(), vg);
  return RET_OK;
}


static ret_t vgcanvas_nanovg_destroy(vgcanvas_t* vgcanvas) {
  vgcanvas_nanovg_t* canvas = (vgcanvas_nanovg_t*)vgcanvas;
  vgcanvas_nanovg_soft_close_asset_manager(vgcanvas);
  vgcanvas_nanovg_deinit(vgcanvas);
  nvgDeleteInternal(canvas->vg);
  TKMEM_FREE(vgcanvas);

  return RET_OK;
}
