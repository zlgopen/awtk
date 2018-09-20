
#define rgb_to_565(r, g, b) ((((r) >> 3) << 11) | (((g) >> 2) << 5) | ((b) >> 3))
#define rgb_from_565(rr, gg, bb, pixel) \
  (rr) = (0xff & ((pixel >> 11) << 3)); \
  (gg) = (0xff & ((pixel >> 5) << 2));  \
  (bb) = (0xff & (pixel << 3));
#define BLEND_PIXEL()               \
  r = (dr * minus_a + sr * a) >> 8; \
  g = (dg * minus_a + sg * a) >> 8; \
  b = (db * minus_a + sb * a) >> 8;

#define rgb_to_rgba8888(r, g, b) ((r) | ((g) << 8) | ((b) << 16) | 0xff000000)
#define rgba_from_rgba8888(rr, gg, bb, aa, pixel) \
  aa = (pixel >> 24);                             \
  bb = (pixel >> 16);                             \
  gg = (pixel >> 8);                              \
  rr = pixel & 0xff;

#ifdef WITH_BITMAP_BGRA
#define rgba_to_image8888(r, g, b, a) (((a) << 24) | ((r) << 16) | ((g) << 8) | (b))
#define rgba_from_image8888(rr, gg, bb, aa, pixel) \
  aa = (pixel >> 24);                              \
  rr = (pixel >> 16);                              \
  gg = (pixel >> 8);                               \
  bb = pixel & 0xff;
#define a_from_image8888(pixel) (pixel >> 24)
#else
#define rgba_to_image8888(r, g, b, a) (((a) << 24) | ((b) << 16) | ((g) << 8) | (r))
#define rgba_from_image8888(rr, gg, bb, aa, pixel) \
  aa = (pixel >> 24);                              \
  bb = (pixel >> 16);                              \
  gg = (pixel >> 8);                               \
  rr = pixel & 0xff;
#define a_from_image8888(pixel) (pixel >> 24)
#endif /*WITH_BITMAP_BGRA*/
#define rgb_to_image8888(r, g, b) rgba_to_image8888(r, g, b, (uint8_t)0xff)

static inline uint32_t rgb565_to_8888(uint16_t pixel) {
  uint8_t r, g, b;
  rgb_from_565(r, g, b, pixel);

  return rgb_to_rgba8888(r, g, b);
}
