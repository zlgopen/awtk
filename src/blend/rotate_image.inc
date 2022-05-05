#include "tkc/rect.h"
#include "base/lcd_orientation_helper.h"

static ret_t rotate_image_impl(bitmap_t* dst, bitmap_t* src, const rect_t* src_r, xy_t dx, xy_t dy, lcd_orientation_t o) {
  uint32_t i = 0;
  uint32_t k = 0;
  uint32_t w = 0;
  uint32_t h = 0;
  uint8_t* src_data = NULL;
  uint8_t* dst_data = NULL;
  pixel_dst_t* src_p = NULL;
  pixel_dst_t* dst_p = NULL;
  uint32_t fb_bpp = bitmap_get_bpp(dst);
  uint32_t img_bpp = bitmap_get_bpp(src);
  uint32_t dst_line_length = bitmap_get_physical_line_length(dst);
  uint32_t src_line_length = bitmap_get_physical_line_length(src);

  assert(o != LCD_ORIENTATION_0);

  dst_data = bitmap_lock_buffer_for_write(dst);
  src_data = bitmap_lock_buffer_for_read(src);
  return_value_if_fail(src_data != NULL && dst_data != NULL, RET_BAD_PARAMS);

  dst_p = (pixel_dst_t*)(dst_data + dy * dst_line_length + dx * fb_bpp);
  src_p = (pixel_dst_t*)(src_data + src_r->y * src_line_length + src_r->x * img_bpp);

  w = src_r->w;
  h = src_r->h;

  switch (o) {
    case LCD_ORIENTATION_90: {
      for (i = 0; i < h; i++) {
        pixel_dst_t* s = src_p + w - 1;
        pixel_dst_t* d = dst_p;

        for (k = 0; k < w; k++) {
          *d = *s--;
          d = (pixel_dst_t*)(((char*)d) + dst_line_length);
        }

        dst_p++;
        src_p = (pixel_dst_t*)(((char*)src_p) + src_line_length);
      }
      break;
    }
    case LCD_ORIENTATION_180: {
      for (i = 0; i < h; i++) {
        pixel_dst_t* s = src_p;
        pixel_dst_t* d = dst_p;

        for (k = 0; k < w; k++) {
          *d = *s++;
          d--;
        }
        dst_p = (pixel_dst_t*)(((char*)dst_p) - dst_line_length);
        src_p = (pixel_dst_t*)(((char*)src_p) + src_line_length);
      }
      break;
    }
    case LCD_ORIENTATION_270: {
      for (i = 0; i < h; i++) {
        pixel_dst_t* s = src_p;
        pixel_dst_t* d = dst_p;

        for (k = 0; k < w; k++) {
          *d = *s++;
          d = (pixel_dst_t*)(((char*)d) + dst_line_length);
        }

        dst_p--;
        src_p = (pixel_dst_t*)(((char*)src_p) + src_line_length);
      }
      break;
    }
    default:
      break;
  }

  bitmap_unlock_buffer(dst);
  bitmap_unlock_buffer(src);

  return RET_OK;
}

static ret_t rotate_image_ex(bitmap_t* dst, bitmap_t* src, const rect_t* src_r, xy_t dx, xy_t dy, lcd_orientation_t o) {
  /* 
  * 因为 rotate_image_impl 函数在处理不同的旋转的度数的时候，计算逻辑不太一样。（因为 dx 和 dy 我们是认为绘图的起始坐标）
   * 180 度：是按照 dx 和 dy 参数作为 dst 的左下角的点往右上角绘图的，所以在 180 度的时候需要再偏移一个 src_r 的宽高来解决。
   * 270 度：是按照 dx 和 dy 参数作为 dst 的左上角的点往右下角绘图的，所以在 270 度的时候需要偏移一个 src_r 的高度来解决。
   */
  if (o == LCD_ORIENTATION_180) {
    dx += src_r->w;
    dy += src_r->h;
  } else if (o == LCD_ORIENTATION_270) {
    dx += src_r->h;
  }
  return rotate_image_impl(dst, src, src_r, dx, dy, o);
}

static ret_t rotate_image(bitmap_t* dst, bitmap_t* src, const rect_t* src_r, lcd_orientation_t o) {
  xy_t dx = 0;
  xy_t dy = 0;
  uint32_t src_w = bitmap_get_physical_width(src);
  uint32_t src_h = bitmap_get_physical_height(src);
  switch (o) {
    case LCD_ORIENTATION_90: {
      dx = src_r->y;
      dy = src_w - src_r->x - src_r->w;
      break;
    }
    case LCD_ORIENTATION_180: {
      dy = src_h - src_r->y - 1;
      dx = src_w - src_r->x - 1;
      break;
    }
    case LCD_ORIENTATION_270: {
      dx = src_h - src_r->y - 1;
      dy = src_r->x;
      break;
    }
    default:
      break;
  }
  return rotate_image_impl(dst, src, src_r, dx, dy, o);
}

