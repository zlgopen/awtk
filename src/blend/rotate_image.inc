static ret_t rotate_image(bitmap_t* fb, bitmap_t* img, rect_t* src, lcd_orientation_t o) {
  xy_t dx = 0;
  xy_t dy = 0;
  uint32_t i = 0;
  uint32_t k = 0;
  uint32_t w = 0;
  uint32_t h = 0;
  uint32_t fb_w = 0;
  uint32_t img_w = 0;
  pixel_dst_t* src_p = NULL;
  pixel_dst_t* dst_p = NULL;

  dx = src->y;
  dy = img->w - src->x - src->w;
  dst_p = ((pixel_dst_t*)(fb->data)) + (dy * fb->w + dx);
  src_p = ((pixel_dst_t*)(img->data)) + (src->y * img->w + src->x);

  w = src->w;
  h = src->h;
  fb_w = fb->w;
  img_w = img->w;

  for (i = 0; i < h; i++) {
    pixel_dst_t* s = src_p + w - 1;
    pixel_dst_t* d = dst_p;

    for (k = 0; k < w; k++) {
      *d = *s--;
      d += fb_w;
    }

    dst_p++;
    src_p += img_w;
  }

  return RET_OK;
}
