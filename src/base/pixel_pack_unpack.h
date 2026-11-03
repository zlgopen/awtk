#ifndef TK_PIXEL_PACK_UNPACK_H
#define TK_PIXEL_PACK_UNPACK_H

#ifdef WITH_BITMAP_BGRA
#define rgba_to_image8888(r, g, b, a) (((a) << 24) | ((r) << 16) | ((g) << 8) | (b))
#else
#define rgba_to_image8888(r, g, b, a) (((a) << 24) | ((b) << 16) | ((g) << 8) | (r))
#endif /*WITH_BITMAP_BGRA*/

#define rgb_to_bgr565(r, g, b) ((((r) >> 3) << 11) | (((g) >> 2) << 5) | ((b) >> 3))
#define rgb_to_rgb565(r, g, b) ((((b) >> 3) << 11) | (((g) >> 2) << 5) | ((r) >> 3))
#define rgb_to_image8888(r, g, b) rgba_to_image8888(r, g, b, (uint8_t)0xff)
#define rgb_to_gray(r, g, b) (((r) * 30 + (g) * 59 + (b) * 11) / 100)

#define bgr565_to_r(p) ((uint8_t)((((p) >> 11) & 0x1f) << 3))
#define bgr565_to_g(p) ((uint8_t)((((p) >> 5) & 0x3f) << 2))
#define bgr565_to_b(p) ((uint8_t)(((p) & 0x1f) << 3))

#define rgb565_to_r(p) ((uint8_t)(((p) & 0x1f) << 3))
#define rgb565_to_g(p) ((uint8_t)((((p) >> 5) & 0x3f) << 2))
#define rgb565_to_b(p) ((uint8_t)((((p) >> 11) & 0x1f) << 3))

/* 三个"8位刻度值×alpha"的加权和打包成bgr565像素值：r/b右移11位、g右移10位(归一化) */
#define rgb_sum_to_bgr565(r_sum, g_sum, b_sum) \
  ((((r_sum) >> 11) << 11) | (((g_sum) >> 10) << 5) | ((b_sum) >> 11))

/* 同rgb_sum_to_bgr565，rgb565的蓝在高位，打包顺序是 b g r */
#define rgb_sum_to_rgb565(r_sum, g_sum, b_sum) \
  ((((b_sum) >> 11) << 11) | (((g_sum) >> 10) << 5) | ((r_sum) >> 11))

#endif /*TK_PIXEL_PACK_UNPACK_H*/
