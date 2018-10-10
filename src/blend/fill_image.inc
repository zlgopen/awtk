static ret_t fill_image(bitmap_t* fb, rect_t* dst, color_t c) {
  int x = 0;
  int y = 0;
  int w = dst->w;
  int h = dst->h;
  uint32_t a = c.rgba.a;
  uint32_t bpp = bitmap_get_bpp(fb);
  uint32_t offset = (fb->w - dst->w);
  pixel_dst_t data = pixel_dst_from_rgb(c.rgba.r, c.rgba.g, c.rgba.b);
  pixel_dst_t* p = (pixel_dst_t*)((fb->data) + (dst->y * fb->w + dst->x) * bpp);

  if (a > 0xf8) {
    for (y = 0; y < h; y++) {
      for (x = 0; x < w; x++) {
        /*FIXME: optimization*/
        *p++ = data;
      }
      p += offset;
    }
  } else {
    for (y = 0; y < h; y++) {
      for (x = 0; x < w; x++) {
        rgba_t drgba = pixel_dst_to_rgba((*p));

        *p++ = blend_rgba(drgba, c.rgba, a);
      }
      p += offset;
    }
  }

  return RET_OK;
}
