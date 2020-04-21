

#ifdef WITH_BITMAP_BGRA
#define rgba_to_image8888(r, g, b, a) (((a) << 24) | ((r) << 16) | ((g) << 8) | (b))
#else
#define rgba_to_image8888(r, g, b, a) (((a) << 24) | ((b) << 16) | ((g) << 8) | (r))
#endif /*WITH_BITMAP_BGRA*/

#define rgb_to_bgr565(r, g, b) ((((r) >> 3) << 11) | (((g) >> 2) << 5) | ((b) >> 3))
#define rgb_to_rgb565(r, g, b) ((((b) >> 3) << 11) | (((g) >> 2) << 5) | ((r) >> 3))
#define rgb_to_image8888(r, g, b) rgba_to_image8888(r, g, b, (uint8_t)0xff)
#define rgb_to_gray(r, g, b) (((r)*30 + (g)*59 + (b)*11) / 100)
