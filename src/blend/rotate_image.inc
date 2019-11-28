static ret_t rotate_image(bitmap_t* dst, bitmap_t* src, rect_t* src_r, lcd_orientation_t o) {
  xy_t dx = 0;
  xy_t dy = 0;
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
  uint32_t dst_line_length = bitmap_get_line_length(dst);
  uint32_t src_line_length = bitmap_get_line_length(src);

  switch (o) {
    case LCD_ORIENTATION_90: {
      dx = src_r->y;
      dy = src->w - src_r->x - src_r->w;
      break;
    }
    case LCD_ORIENTATION_180: {
      dy = src->h - src_r->y - 1;
      dx = src->w - src_r->x - 1;
      break;
    }
    case LCD_ORIENTATION_270: {
      dx = src->h - src_r->y;
      dy = src_r->x;
      break;
    }
    default:
      break;
  }

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
