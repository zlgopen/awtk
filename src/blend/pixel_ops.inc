/**
 * File:   pixel_ops.inc
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  pixel operations
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2018-02-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

static inline uint8_t tk_pixel_ops_limit_uint8(int32_t tmp) {
  if(tmp > 0xff) {
    tmp = 0xff;
  } else if(tmp < 0) {
    tmp = 0;
  }
  return (uint8_t)tmp;
}

static inline pixel_t blend_color(color_t bg, color_t fg, uint8_t a) {
  if(a > 0xf4) { 
    uint8_t minus_a = 0xff - a;

    uint8_t r = (bg.rgba.r * minus_a + fg.rgba.r * a) >> 8;
    uint8_t g = (bg.rgba.g * minus_a + fg.rgba.g * a) >> 8;
    uint8_t b = (bg.rgba.b * minus_a + fg.rgba.b * a) >> 8;
    pixel_t p = pixel_from_rgb(r, g, b);

    return p;
  } else {
    uint8_t fg_a = (fg.rgba.a * a) >> 8;
    uint8_t out_a = tk_pixel_ops_limit_uint8(bg.rgba.a + fg_a - ((bg.rgba.a * fg_a) >> 8));
    if(out_a > 0) {
      uint8_t d_a = (bg.rgba.a * (0xff - fg_a)) >> 8;
      uint8_t r = (bg.rgba.r * d_a + fg.rgba.r * fg_a) / out_a;
      uint8_t g = (bg.rgba.g * d_a + fg.rgba.g * fg_a) / out_a;
      uint8_t b = (bg.rgba.b * d_a + fg.rgba.b * fg_a) / out_a;
      pixel_t p = pixel_from_rgba(r, g, b, out_a);
      return p;
    } else {
      pixel_t p = pixel_from_rgba(0x0, 0x0, 0x0, 0x0);
      return p;
    }
  }
}

static inline pixel_t blend_pixel(pixel_t pixel, color_t c) {
  uint8_t a = c.rgba.a;
  uint8_t minus_a = 0xff - a;
  rgba_t rgba = pixel_to_rgba(pixel);
  
  if(rgba.a > 0xf4) { 
    uint8_t r = (rgba.r * minus_a + c.rgba.r * a) >> 8;
    uint8_t g = (rgba.g * minus_a + c.rgba.g * a) >> 8;
    uint8_t b = (rgba.b * minus_a + c.rgba.b * a) >> 8;
    pixel_t p = pixel_from_rgb(r, g, b);

    return p;
  } else {
    uint8_t out_a = tk_pixel_ops_limit_uint8(c.rgba.a + rgba.a - ((c.rgba.a * rgba.a) >> 8));
    if(out_a > 0) {
      uint8_t d_a = (rgba.a * (0xff - c.rgba.a)) >> 8;
      uint8_t r = (rgba.r * d_a + c.rgba.r * c.rgba.a) / out_a;
      uint8_t g = (rgba.g * d_a + c.rgba.g * c.rgba.a) / out_a;
      uint8_t b = (rgba.b * d_a + c.rgba.b * c.rgba.a) / out_a;
      pixel_t p = pixel_from_rgba(r, g, b, out_a);
      return p;
    } else {
      pixel_t p = pixel_from_rgba(0x0, 0x0, 0x0, 0x0);
      return p;
    }
  }
}

static inline pixel_t blend_alpha(color_t fg, uint8_t a) {
  if(fg.rgba.a > 0xf4) {
    uint8_t r = (fg.rgba.r * a) >> 8;
    uint8_t g = (fg.rgba.g * a) >> 8;
    uint8_t b = (fg.rgba.b * a) >> 8;
    pixel_t p = pixel_from_rgb(r, g, b);

    return p;
  } else {

    uint8_t out_a =tk_pixel_ops_limit_uint8(a + fg.rgba.a - ((a * fg.rgba.a) >> 8));
    if(out_a > 0) {
      uint8_t d_a = (fg.rgba.a * (0xff - a)) >> 8;
      uint8_t r = (fg.rgba.r * d_a) / out_a;
      uint8_t g = (fg.rgba.g * d_a) / out_a;
      uint8_t b = (fg.rgba.b * d_a) / out_a;
      pixel_t p = pixel_from_rgba(r, g, b, out_a);
      return p;
    } else {
      pixel_t p = pixel_from_rgba(0x0, 0x0, 0x0, 0x0);
      return p;
    }
  }
}

static inline pixel_t blend_rgba(rgba_t d, rgba_t s, uint8_t a) {
  if(d.a > 0xf4) {
    uint8_t minus_a = 0xff - a;
    uint8_t r = (d.r * minus_a + s.r * a) >> 8;
    uint8_t g = (d.g * minus_a + s.g * a) >> 8;
    uint8_t b = (d.b * minus_a + s.b * a) >> 8;

    pixel_t p = pixel_from_rgb(r, g, b);
    return p;
  } else {
    uint8_t out_a = tk_pixel_ops_limit_uint8(s.a + d.a - ((s.a * d.a) >> 8));
    if(out_a > 0) {
      uint8_t d_a = (d.a * (0xff - s.a)) >> 8;
      uint8_t r = (d.r * d_a + s.r * s.a) / out_a;
      uint8_t g = (d.g * d_a + s.g * s.a) / out_a;
      uint8_t b = (d.b * d_a + s.b * s.a) / out_a;
      pixel_t p = pixel_from_rgba(r, g, b, out_a);
      return p;
    } else {
      pixel_t p = pixel_from_rgba(0x0, 0x0, 0x0, 0x0);
      return p;
    }
  }
}

static inline pixel_t blend_rgba_premulti(rgba_t d, rgba_t s, uint8_t a) {
  if(d.a > 0xf4) {
    uint8_t r = ((d.r * a) >> 8) + s.r;
    uint8_t g = ((d.g * a) >> 8) + s.g;
    uint8_t b = ((d.b * a) >> 8) + s.b;

    pixel_t p = pixel_from_rgb(r, g, b);
    return p;
  } else {
    uint8_t out_a = tk_pixel_ops_limit_uint8(s.a + d.a - ((s.a * d.a) >> 8));
    if(out_a > 0) {
      uint8_t d_a = (d.a * (0xff - s.a)) >> 8;
      uint8_t r = (d.r * d_a + s.r) / out_a;
      uint8_t g = (d.g * d_a + s.g) / out_a;
      uint8_t b = (d.b * d_a + s.b) / out_a;
      pixel_t p = pixel_from_rgba(r, g, b, out_a);
      return p;
    } else {
      pixel_t p = pixel_from_rgba(0x0, 0x0, 0x0, 0x0);
      return p;
    }
  }
}

static inline pixel_t blend_rgba_dark(rgba_t d, uint8_t a) {
  if(d.a > 0xf4) {
    uint8_t r = ((d.r * a) >> 8);
    uint8_t g = ((d.g * a) >> 8);
    uint8_t b = ((d.b * a) >> 8);

    pixel_t p = pixel_from_rgb(r, g, b);

    return p;
  } else {

    uint8_t out_a =tk_pixel_ops_limit_uint8(a + d.a - ((a * d.a) >> 8));
    if(out_a > 0) {
      uint8_t d_a = (d.a * (0xff - a)) >> 8;
      uint8_t r  = (d.r * d_a) / out_a;
      uint8_t g = (d.g * d_a) / out_a;
      uint8_t b = (d.b * d_a) / out_a;
      pixel_t p = pixel_from_rgba(r, g, b, out_a);
      return p;
    } else {
      pixel_t p = pixel_from_rgba(0x0, 0x0, 0x0, 0x0);
      return p;
    }
  }
}
