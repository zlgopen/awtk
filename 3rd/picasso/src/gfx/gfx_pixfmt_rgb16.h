/* Picasso - a vector graphics library
 * 
 * Copyright (C) 2014 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#ifndef _GFX_PIXFMT_RGB16_H_
#define _GFX_PIXFMT_RGB16_H_

#include "common.h"
#include "gfx_rendering_buffer.h"

namespace gfx {

// composite_op_rgb_16_clear
template <typename ColorType, typename Order, typename Blender>
struct composite_op_rgb_16_clear
{
    typedef ColorType color_type;
    typedef Blender blender_type;
    typedef uint16_t pixel_type;
    typedef typename color_type::value_type value_type;

    static void blend_pix(pixel_type* p, unsigned int, unsigned int,
                          unsigned int, unsigned int, unsigned int cover)
    {
        if (cover < 255) {
            cover = 255 - cover;
            color_type c = blender_type::make_color(*p);
            c.r = (value_type)((c.r * cover + 255) >> 8);
            c.g = (value_type)((c.g * cover + 255) >> 8);
            c.b = (value_type)((c.b * cover + 255) >> 8);
            *p = blender_type::make_pix(c.r, c.g, c.b);
        } else {
            *p = blender_type::make_pix(0, 0, 0); 
        }
    }
};

// composite_op_rgb_16_src
template <typename ColorType, typename Order, typename Blender>
struct composite_op_rgb_16_src
{
    typedef ColorType color_type;
    typedef Blender blender_type;
    typedef uint16_t pixel_type;
    typedef typename color_type::value_type value_type;

    static void blend_pix(pixel_type* p, unsigned int sr, unsigned int sg,
                          unsigned int sb, unsigned int sa, unsigned int cover)
    {
        if (cover < 255) {
            unsigned int alpha = 255 - cover;
            color_type c = blender_type::make_color(*p);
            c.r = (value_type)(((c.r * alpha + 255) >> 8) + ((sr * cover + 255) >> 8));
            c.g = (value_type)(((c.g * alpha + 255) >> 8) + ((sg * cover + 255) >> 8));
            c.b = (value_type)(((c.b * alpha + 255) >> 8) + ((sb * cover + 255) >> 8));
            *p = blender_type::make_pix(c.r, c.g, c.b);
        } else {
            *p = blender_type::make_pix(sr, sg, sb); 
        }
    }
};

// composite_op_rgb_16_dst
template <typename ColorType, typename Order, typename Blender>
struct composite_op_rgb_16_dst
{
    typedef uint16_t pixel_type;

    static void blend_pix(pixel_type*, unsigned int, unsigned int,
                          unsigned int, unsigned int, unsigned int)
    {
    }
};

// comp_op_rgb_src_over
template <typename ColorType, typename Order, typename Blender>
struct composite_op_rgb_16_src_over
{
    typedef ColorType color_type;
    typedef Blender blender_type;
    typedef uint16_t pixel_type;
    typedef typename color_type::value_type value_type;
    typedef typename color_type::calc_type calc_type;

    enum {
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };

    //   Dca' = Sca + Dca.(1 - Sa)
    //   Da'  = Sa + Da - Sa.Da 
    static void blend_pix(pixel_type* p, unsigned int sr, unsigned int sg,
                          unsigned int sb, unsigned int sa, unsigned int cover)
    {
        if (cover < 255) {
            sr = (sr * cover + 255) >> 8;
            sg = (sg * cover + 255) >> 8;
            sb = (sb * cover + 255) >> 8;
            sa = (sa * cover + 255) >> 8;
        }

        if (sa < 255) {
            calc_type s1a = base_mask - sa;
            color_type c = blender_type::make_color(*p);
            c.r = (value_type)(sr + ((c.r * s1a + base_mask) >> base_shift));
            c.g = (value_type)(sg + ((c.g * s1a + base_mask) >> base_shift));
            c.b = (value_type)(sb + ((c.b * s1a + base_mask) >> base_shift));
            *p = blender_type::make_pix(c.r, c.g, c.b);
        } else { // fast for opaque
            *p = blender_type::make_pix(sr, sg, sb);
        }
    }
};

// comp_op_rgb_dst_over
template <typename ColorType, typename Order, typename Blender>
struct composite_op_rgb_16_dst_over
{
    typedef ColorType color_type;
    typedef Blender blender_type;
    typedef uint16_t pixel_type;
    typedef typename color_type::value_type value_type;
    typedef typename color_type::calc_type calc_type;

    enum {
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };

    // Dca' = Dca + Sca.(1 - Da)
    // Da'  = Sa + Da - Sa.Da 
    static void blend_pix(pixel_type* p, unsigned int sr, unsigned int sg,
                          unsigned int sb, unsigned int sa, unsigned int cover)
    {
        if (cover < 255) {
            sr = (sr * cover + 255) >> 8;
            sg = (sg * cover + 255) >> 8;
            sb = (sb * cover + 255) >> 8;
            sa = (sa * cover + 255) >> 8;
        }

        color_type c = blender_type::make_color(*p);
        c.r = (value_type)(c.r + (base_mask >> base_shift));
        c.g = (value_type)(c.g + (base_mask >> base_shift));
        c.b = (value_type)(c.b + (base_mask >> base_shift));
        *p = blender_type::make_pix(c.r, c.g, c.b);
    }
};

// composite_op_rgb_16_src_in
template <typename ColorType, typename Order, typename Blender>
struct composite_op_rgb_16_src_in
{
    typedef ColorType color_type;
    typedef Blender blender_type;
    typedef uint16_t pixel_type;
    typedef typename color_type::value_type value_type;
    typedef typename color_type::calc_type calc_type;

    enum { 
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };

    // Dca' = Sca.Da
    // Da'  = Sa.Da 
    static void blend_pix(pixel_type* p, unsigned int sr, unsigned int sg,
                          unsigned int sb, unsigned int sa, unsigned int cover)
    {
        calc_type da = base_mask;
        color_type c = blender_type::make_color(*p);
        if (cover < 255) {
            unsigned int alpha = 255 - cover;
            c.r = (value_type)(((c.r * alpha + 255) >> 8) +
                  ((((sr * da + base_mask) >> base_shift) * cover + 255) >> 8));
            c.g = (value_type)(((c.g * alpha + 255) >> 8) +
                  ((((sg * da + base_mask) >> base_shift) * cover + 255) >> 8));
            c.b = (value_type)(((c.b * alpha + 255) >> 8) +
                  ((((sb * da + base_mask) >> base_shift) * cover + 255) >> 8));
        } else {
            c.r = (value_type)((sr * da + base_mask) >> base_shift);
            c.g = (value_type)((sg * da + base_mask) >> base_shift);
            c.b = (value_type)((sb * da + base_mask) >> base_shift);
        }
        *p = blender_type::make_pix(c.r, c.g, c.b);
    }
};

// composite_op_rgb_16_dst_in
template <typename ColorType, typename Order, typename Blender>
struct composite_op_rgb_16_dst_in
{
    typedef ColorType color_type;
    typedef Blender blender_type;
    typedef uint16_t pixel_type;
    typedef typename color_type::value_type value_type;
    typedef typename color_type::calc_type calc_type;

    enum { 
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };

    // Dca' = Dca.Sa
    // Da'  = Sa.Da 
    static void blend_pix(pixel_type* p, unsigned int, unsigned int, unsigned int, 
                                         unsigned int sa, unsigned int cover)
    {
        if (cover < 255) {
            sa = base_mask - ((cover * (base_mask - sa) + 255) >> 8);
        }

        color_type c = blender_type::make_color(*p);
        c.r = (value_type)((c.r * sa + base_mask) >> base_shift);
        c.g = (value_type)((c.g * sa + base_mask) >> base_shift);
        c.b = (value_type)((c.b * sa + base_mask) >> base_shift);
        *p = blender_type::make_pix(c.r, c.g, c.b);
    }
};

// composite_op_rgb_16_src_out
template <typename ColorType, typename Order, typename Blender>
struct composite_op_rgb_16_src_out
{
    typedef ColorType color_type;
    typedef Blender blender_type;
    typedef uint16_t pixel_type;
    typedef typename color_type::value_type value_type;
    typedef typename color_type::calc_type calc_type;

    enum {
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };

    // Dca' = Sca.(1 - Da)
    // Da'  = Sa.(1 - Da) 
    static void blend_pix(pixel_type* p, unsigned int sr, unsigned int sg,
                                         unsigned int sb, unsigned int sa, unsigned int cover)
    {
        calc_type da = 0;
        color_type c = blender_type::make_color(*p);
        if (cover < 255) {
            unsigned int alpha = 255 - cover;
            c.r = (value_type)(((c.r * alpha + 255) >> 8) +
                              ((((sr * da + base_mask) >> base_shift) * cover + 255) >> 8));
            c.g = (value_type)(((c.g * alpha + 255) >> 8) +
                              ((((sg * da + base_mask) >> base_shift) * cover + 255) >> 8));
            c.b = (value_type)(((c.b * alpha + 255) >> 8) +
                              ((((sb * da + base_mask) >> base_shift) * cover + 255) >> 8));
        } else {
            c.r = (value_type)((sr * da + base_mask) >> base_shift);
            c.g = (value_type)((sg * da + base_mask) >> base_shift);
            c.b = (value_type)((sb * da + base_mask) >> base_shift);
        }
        *p = blender_type::make_pix(c.r, c.g, c.b);
    }
};

// composite_op_rgb_16_dst_out
template <typename ColorType, typename Order, typename Blender>
struct composite_op_rgb_16_dst_out
{
    typedef ColorType color_type;
    typedef Blender blender_type;
    typedef uint16_t pixel_type;
    typedef typename color_type::value_type value_type;
    typedef typename color_type::calc_type calc_type;

    enum {
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };

    // Dca' = Dca.(1 - Sa) 
    // Da'  = Da.(1 - Sa) 
    static void blend_pix(pixel_type* p, unsigned int, unsigned int, unsigned int,
                                          unsigned int sa, unsigned int cover)
    {
        if (cover < 255) {
            sa = (sa * cover + 255) >> 8;
        }

        sa = base_mask - sa;
        color_type c = blender_type::make_color(*p);
        c.r = (value_type)((c.r * sa + base_shift) >> base_shift);
        c.g = (value_type)((c.g * sa + base_shift) >> base_shift);
        c.b = (value_type)((c.b * sa + base_shift) >> base_shift);
        *p = blender_type::make_pix(c.r, c.g, c.b);
    }
};

// composite_op_rgb_16_src_atop
template <typename ColorType, typename Order, typename Blender>
struct composite_op_rgb_16_src_atop
{
    typedef ColorType color_type;
    typedef Blender blender_type;
    typedef uint16_t pixel_type;
    typedef typename color_type::value_type value_type;
    typedef typename color_type::calc_type calc_type;

    enum { 
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };

    // Dca' = Sca.Da + Dca.(1 - Sa)
    // Da'  = Da
    static void blend_pix(pixel_type* p, unsigned int sr, unsigned int sg,
                                         unsigned int sb, unsigned int sa, unsigned int cover)
    {
        if (cover < 255) {
            sr = (sr * cover + 255) >> 8;
            sg = (sg * cover + 255) >> 8;
            sb = (sb * cover + 255) >> 8;
            sa = (sa * cover + 255) >> 8;
        }

        calc_type da = base_mask;
        sa = base_mask - sa;
        color_type c = blender_type::make_color(*p);
        c.r = (value_type)((sr * da + c.r * sa + base_mask) >> base_shift);
        c.g = (value_type)((sg * da + c.g * sa + base_mask) >> base_shift);
        c.b = (value_type)((sb * da + c.b * sa + base_mask) >> base_shift);
        *p = blender_type::make_pix(c.r, c.g, c.b);
    }
};

// composite_op_rgb_16_dst_atop
template <typename ColorType, typename Order, typename Blender>
struct composite_op_rgb_16_dst_atop
{
    typedef ColorType color_type;
    typedef Blender blender_type;
    typedef uint16_t pixel_type;
    typedef typename color_type::value_type value_type;
    typedef typename color_type::calc_type calc_type;

    enum {
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };

    // Dca' = Dca.Sa + Sca.(1 - Da)
    // Da'  = Sa 
    static void blend_pix(pixel_type* p, unsigned int sr, unsigned int sg,
                                         unsigned int sb, unsigned int sa, unsigned int cover)
    {
        calc_type da = 0;
        color_type c = blender_type::make_color(*p);

        if (cover < 255) {
            unsigned int alpha = 255 - cover;
            sr = (c.r * sa + sr * da + base_mask) >> base_shift;
            sg = (c.g * sa + sg * da + base_mask) >> base_shift;
            sb = (c.b * sa + sb * da + base_mask) >> base_shift;
            c.r = (value_type)(((c.r * alpha + 255) >> 8) + ((sr * cover + 255) >> 8));
            c.g = (value_type)(((c.g * alpha + 255) >> 8) + ((sg * cover + 255) >> 8));
            c.b = (value_type)(((c.b * alpha + 255) >> 8) + ((sb * cover + 255) >> 8));
        } else {
            c.r = (value_type)((c.r * sa + sr * da + base_mask) >> base_shift);
            c.g = (value_type)((c.g * sa + sg * da + base_mask) >> base_shift);
            c.b = (value_type)((c.b * sa + sb * da + base_mask) >> base_shift);
        }
        *p = blender_type::make_pix(c.r, c.g, c.b);
    }
};

// composite_op_rgb_16_xor
template <typename ColorType, typename Order, typename Blender>
struct composite_op_rgb_16_xor
{
    typedef ColorType color_type;
    typedef Blender blender_type;
    typedef uint16_t pixel_type;
    typedef typename color_type::value_type value_type;
    typedef typename color_type::calc_type calc_type;

    enum {
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };

    // Dca' = Sca.(1 - Da) + Dca.(1 - Sa)
    // Da'  = Sa + Da - 2.Sa.Da 
    static void blend_pix(pixel_type* p, unsigned int sr, unsigned int sg,
                                         unsigned int sb, unsigned int sa, unsigned int cover)
    {
        if (cover < 255) {
            sr = (sr * cover + 255) >> 8;
            sg = (sg * cover + 255) >> 8;
            sb = (sb * cover + 255) >> 8;
            sa = (sa * cover + 255) >> 8;
        }

        if (sa) {
            calc_type s1a = base_mask - sa;
            calc_type d1a = 0;
            color_type c = blender_type::make_color(*p);
            c.r = (value_type)((c.r * s1a + sr * d1a + base_mask) >> base_shift);
            c.g = (value_type)((c.g * s1a + sg * d1a + base_mask) >> base_shift);
            c.b = (value_type)((c.b * s1a + sb * d1a + base_mask) >> base_shift);
            *p = blender_type::make_pix(c.r, c.g, c.b);
        }
    }
};

// composite_op_rgb_16_plus
template <typename ColorType, typename Order, typename Blender>
struct composite_op_rgb_16_plus
{
    typedef ColorType color_type;
    typedef Blender blender_type;
    typedef uint16_t pixel_type;
    typedef typename color_type::value_type value_type;
    typedef typename color_type::calc_type calc_type;

    enum {
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };

    // Dca' = Sca + Dca
    // Da'  = Sa + Da 
    static void blend_pix(pixel_type* p, unsigned int sr, unsigned int sg,
                                         unsigned int sb, unsigned int sa, unsigned int cover)
    {
        if (cover < 255) {
            sr = (sr * cover + 255) >> 8;
            sg = (sg * cover + 255) >> 8;
            sb = (sb * cover + 255) >> 8;
            sa = (sa * cover + 255) >> 8;
        }

        if (sa) {
            color_type c = blender_type::make_color(*p);
            calc_type dr = c.r + sr;
            calc_type dg = c.g + sg;
            calc_type db = c.b + sb;
            c.r = (dr > base_mask) ? (value_type)base_mask : dr;
            c.g = (dg > base_mask) ? (value_type)base_mask : dg;
            c.b = (db > base_mask) ? (value_type)base_mask : db;
            *p = blender_type::make_pix(c.r, c.g, c.b);
        }
    }
};

// composite_op_rgb_16_minus
template <typename ColorType, typename Order, typename Blender>
struct composite_op_rgb_16_minus
{
    typedef ColorType color_type;
    typedef Blender blender_type;
    typedef uint16_t pixel_type;
    typedef typename color_type::value_type value_type;
    typedef typename color_type::calc_type calc_type;

    enum {
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };

    // Dca' = Dca - Sca
    // Da' = 1 - (1 - Sa).(1 - Da)
    static void blend_pix(pixel_type* p, unsigned int sr, unsigned int sg,
                                         unsigned int sb, unsigned int sa, unsigned int cover)
    {
        if (cover < 255) {
            sr = (sr * cover + 255) >> 8;
            sg = (sg * cover + 255) >> 8;
            sb = (sb * cover + 255) >> 8;
            sa = (sa * cover + 255) >> 8;
        }

        if (sa) {
            color_type c = blender_type::make_color(*p);
            calc_type dr = c.r - sr;
            calc_type dg = c.g - sg;
            calc_type db = c.b - sb;
            c.r = (dr > base_mask) ? 0 : dr;
            c.g = (dg > base_mask) ? 0 : dg;
            c.b = (db > base_mask) ? 0 : db;
            *p = blender_type::make_pix(c.r, c.g, c.b);
        }
    }
};

// composite_op_rgb_16_multiply
template <typename ColorType, typename Order, typename Blender>
struct composite_op_rgb_16_multiply
{
    typedef ColorType color_type;
    typedef Blender blender_type;
    typedef uint16_t pixel_type;
    typedef typename color_type::value_type value_type;
    typedef typename color_type::calc_type calc_type;

    enum {
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };

    // Dca' = Sca.Dca + Sca.(1 - Da) + Dca.(1 - Sa)
    // Da'  = Sa + Da - Sa.Da 
    static void blend_pix(pixel_type* p, unsigned int sr, unsigned int sg,
                                         unsigned int sb, unsigned int sa, unsigned int cover)
    {
        if (cover < 255) {
            sr = (sr * cover + 255) >> 8;
            sg = (sg * cover + 255) >> 8;
            sb = (sb * cover + 255) >> 8;
            sa = (sa * cover + 255) >> 8;
        }

        if (sa) {
            color_type c = blender_type::make_color(*p);
            calc_type s1a = base_mask - sa;
            calc_type d1a = 0;
            calc_type dr = c.r;
            calc_type dg = c.g;
            calc_type db = c.b;
            c.r = (value_type)((sr * dr + sr * d1a + dr * s1a + base_mask) >> base_shift);
            c.g = (value_type)((sg * dg + sg * d1a + dg * s1a + base_mask) >> base_shift);
            c.b = (value_type)((sb * db + sb * d1a + db * s1a + base_mask) >> base_shift);
            *p = blender_type::make_pix(c.r, c.g, c.b);
        }
    }
};

// composite_op_rgb_16_screen
template <typename ColorType, typename Order, typename Blender>
struct composite_op_rgb_16_screen
{
    typedef ColorType color_type;
    typedef Blender blender_type;
    typedef uint16_t pixel_type;
    typedef typename color_type::value_type value_type;
    typedef typename color_type::calc_type calc_type;

    enum {
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };

    // Dca' = Sca + Dca - Sca.Dca
    // Da'  = Sa + Da - Sa.Da 
    static void blend_pix(pixel_type* p, unsigned int sr, unsigned int sg,
                                         unsigned int sb, unsigned int sa, unsigned int cover)
    {
        if (cover < 255) {
            sr = (sr * cover + 255) >> 8;
            sg = (sg * cover + 255) >> 8;
            sb = (sb * cover + 255) >> 8;
            sa = (sa * cover + 255) >> 8;
        }

        if (sa) {
            color_type c = blender_type::make_color(*p);
            calc_type dr = c.r;
            calc_type dg = c.g;
            calc_type db = c.b;
            c.r = (value_type)(sr + dr - ((sr * dr + base_mask) >> base_shift));
            c.g = (value_type)(sg + dg - ((sg * dg + base_mask) >> base_shift));
            c.b = (value_type)(sb + db - ((sb * db + base_mask) >> base_shift));
            *p = blender_type::make_pix(c.r, c.g, c.b);
        }
    }
};

// composite_op_rgb_16_overlay
template <typename ColorType, typename Order, typename Blender>
struct composite_op_rgb_16_overlay
{
    typedef ColorType color_type;
    typedef Blender blender_type;
    typedef uint16_t pixel_type;
    typedef typename color_type::value_type value_type;
    typedef typename color_type::calc_type calc_type;

    enum {
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };

    // if 2.Dca < Da
    //   Dca' = 2.Sca.Dca + Sca.(1 - Da) + Dca.(1 - Sa)
    // otherwise
    //   Dca' = Sa.Da - 2.(Da - Dca).(Sa - Sca) + Sca.(1 - Da) + Dca.(1 - Sa)
    // 
    // Da' = Sa + Da - Sa.Da
    static void blend_pix(pixel_type* p, unsigned int sr, unsigned int sg,
                                         unsigned int sb, unsigned int sa, unsigned int cover)
    {
        if (cover < 255) {
            sr = (sr * cover + 255) >> 8;
            sg = (sg * cover + 255) >> 8;
            sb = (sb * cover + 255) >> 8;
            sa = (sa * cover + 255) >> 8;
        }

        if (sa) {
            color_type c = blender_type::make_color(*p);
            calc_type d1a  = 0;
            calc_type s1a  = base_mask - sa;
            calc_type dr   = c.r;
            calc_type dg   = c.g;
            calc_type db   = c.b;
            calc_type da   = base_mask;
            calc_type sada = sa * base_mask;

            c.r = (value_type)(((2*dr < da) ? 
                2*sr*dr + sr*d1a + dr*s1a : 
                sada - 2*(da - dr)*(sa - sr) + sr*d1a + dr*s1a + base_mask) >> base_shift);

            c.g = (value_type)(((2*dg < da) ? 
                2*sg*dg + sg*d1a + dg*s1a : 
                sada - 2*(da - dg)*(sa - sg) + sg*d1a + dg*s1a + base_mask) >> base_shift);

            c.b = (value_type)(((2*db < da) ? 
                2*sb*db + sb*d1a + db*s1a : 
                sada - 2*(da - db)*(sa - sb) + sb*d1a + db*s1a + base_mask) >> base_shift);
            *p = blender_type::make_pix(c.r, c.g, c.b);
        }
    }
};

// composite_op_rgb_16_darken
template <typename ColorType, typename Order, typename Blender>
struct composite_op_rgb_16_darken
{
    typedef ColorType color_type;
    typedef Blender blender_type;
    typedef uint16_t pixel_type;
    typedef typename color_type::value_type value_type;
    typedef typename color_type::calc_type calc_type;

    enum {
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };

    // Dca' = min(Sca.Da, Dca.Sa) + Sca.(1 - Da) + Dca.(1 - Sa)
    // Da'  = Sa + Da - Sa.Da 
    static void blend_pix(pixel_type* p, unsigned int sr, unsigned int sg,
                                         unsigned int sb, unsigned int sa, unsigned int cover)
    {
        if (cover < 255) {
            sr = (sr * cover + 255) >> 8;
            sg = (sg * cover + 255) >> 8;
            sb = (sb * cover + 255) >> 8;
            sa = (sa * cover + 255) >> 8;
        }

        if (sa) {
            color_type c = blender_type::make_color(*p);
            calc_type d1a = 0;
            calc_type s1a = base_mask - sa;
            calc_type dr  = c.r;
            calc_type dg  = c.g;
            calc_type db  = c.b;
            calc_type da  = base_mask;

            c.r = (value_type)((Min(sr * da, dr * sa) + sr * d1a + dr * s1a + base_mask) >> base_shift);
            c.g = (value_type)((Min(sg * da, dg * sa) + sg * d1a + dg * s1a + base_mask) >> base_shift);
            c.b = (value_type)((Min(sb * da, db * sa) + sb * d1a + db * s1a + base_mask) >> base_shift);
            *p = blender_type::make_pix(c.r, c.g, c.b);
        }
    }
};

// composite_op_rgb_16_lighten
template <typename ColorType, typename Order, typename Blender>
struct composite_op_rgb_16_lighten
{
    typedef ColorType color_type;
    typedef Blender blender_type;
    typedef uint16_t pixel_type;
    typedef typename color_type::value_type value_type;
    typedef typename color_type::calc_type calc_type;

    enum {
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };

    // Dca' = max(Sca.Da, Dca.Sa) + Sca.(1 - Da) + Dca.(1 - Sa)
    // Da'  = Sa + Da - Sa.Da 
    static void blend_pix(pixel_type* p, unsigned int sr, unsigned int sg,
                                         unsigned int sb, unsigned int sa, unsigned int cover)
    {
        if (cover < 255) {
            sr = (sr * cover + 255) >> 8;
            sg = (sg * cover + 255) >> 8;
            sb = (sb * cover + 255) >> 8;
            sa = (sa * cover + 255) >> 8;
        }

        if (sa) {
            color_type c = blender_type::make_color(*p);
            calc_type d1a = 0;
            calc_type s1a = base_mask - sa;
            calc_type dr  = c.r;
            calc_type dg  = c.g;
            calc_type db  = c.b;
            calc_type da  = base_mask;

            c.r = (value_type)((Max(sr * da, dr * sa) + sr * d1a + dr * s1a + base_mask) >> base_shift);
            c.g = (value_type)((Max(sg * da, dg * sa) + sg * d1a + dg * s1a + base_mask) >> base_shift);
            c.b = (value_type)((Max(sb * da, db * sa) + sb * d1a + db * s1a + base_mask) >> base_shift);
            *p = blender_type::make_pix(c.r, c.g, c.b);
        }
    }
};

// composite_op_rgb_16_color_dodge
template <typename ColorType, typename Order, typename Blender>
struct composite_op_rgb_16_color_dodge
{
    typedef ColorType color_type;
    typedef Blender blender_type;
    typedef uint16_t pixel_type;
    typedef typename color_type::value_type value_type;
    typedef typename color_type::calc_type calc_type;
    typedef typename color_type::long_type long_type;

    enum {
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };

    // if Sca.Da + Dca.Sa >= Sa.Da
    //   Dca' = Sa.Da + Sca.(1 - Da) + Dca.(1 - Sa)
    // otherwise
    //   Dca' = Dca.Sa/(1-Sca/Sa) + Sca.(1 - Da) + Dca.(1 - Sa)
    //
    // Da'  = Sa + Da - Sa.Da 
    static void blend_pix(pixel_type* p, unsigned int sr, unsigned int sg,
                                         unsigned int sb, unsigned int sa, unsigned int cover)
    {
        if (cover < 255) {
            sr = (sr * cover + 255) >> 8;
            sg = (sg * cover + 255) >> 8;
            sb = (sb * cover + 255) >> 8;
            sa = (sa * cover + 255) >> 8;
        }

        if (sa) {
            color_type c = blender_type::make_color(*p);
            calc_type d1a  = 0;
            calc_type s1a  = base_mask - sa;
            calc_type dr   = c.r;
            calc_type dg   = c.g;
            calc_type db   = c.b;
            calc_type da   = base_mask;
            long_type drsa = dr * sa;
            long_type dgsa = dg * sa;
            long_type dbsa = db * sa;
            long_type srda = sr * da;
            long_type sgda = sg * da;
            long_type sbda = sb * da;
            long_type sada = sa * da;

            c.r = (value_type)((srda + drsa >= sada) ? 
                (sada + sr * d1a + dr * s1a + base_mask) >> base_shift :
                drsa / (base_mask - (sr << base_shift) / sa) + ((sr * d1a + dr * s1a + base_mask) >> base_shift));

            c.g = (value_type)((sgda + dgsa >= sada) ? 
                (sada + sg * d1a + dg * s1a + base_mask) >> base_shift :
                dgsa / (base_mask - (sg << base_shift) / sa) + ((sg * d1a + dg * s1a + base_mask) >> base_shift));

            c.b = (value_type)((sbda + dbsa >= sada) ? 
                (sada + sb * d1a + db * s1a + base_mask) >> base_shift :
                dbsa / (base_mask - (sb << base_shift) / sa) + ((sb * d1a + db * s1a + base_mask) >> base_shift));
            *p = blender_type::make_pix(c.r, c.g, c.b);
        }
    }
};

// composite_op_rgb_16_color_burn
template <typename ColorType, typename Order, typename Blender>
struct composite_op_rgb_16_color_burn
{
    typedef ColorType color_type;
    typedef Blender blender_type;
    typedef uint16_t pixel_type;
    typedef typename color_type::value_type value_type;
    typedef typename color_type::calc_type calc_type;
    typedef typename color_type::long_type long_type;

    enum {
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };

    // if Sca.Da + Dca.Sa <= Sa.Da
    //   Dca' = Sca.(1 - Da) + Dca.(1 - Sa)
    // otherwise
    //   Dca' = Sa.(Sca.Da + Dca.Sa - Sa.Da)/Sca + Sca.(1 - Da) + Dca.(1 - Sa)
    // 
    // Da'  = Sa + Da - Sa.Da 
    static void blend_pix(pixel_type* p, unsigned int sr, unsigned int sg,
                                         unsigned int sb, unsigned int sa, unsigned int cover)
    {
        if (cover < 255) {
            sr = (sr * cover + 255) >> 8;
            sg = (sg * cover + 255) >> 8;
            sb = (sb * cover + 255) >> 8;
            sa = (sa * cover + 255) >> 8;
        }

        if (sa) {
            color_type c = blender_type::make_color(*p);
            calc_type d1a  = 0;
            calc_type s1a  = base_mask - sa;
            calc_type dr   = c.r;
            calc_type dg   = c.g;
            calc_type db   = c.b;
            calc_type da   = base_mask;
            long_type drsa = dr * sa;
            long_type dgsa = dg * sa;
            long_type dbsa = db * sa;
            long_type srda = sr * da;
            long_type sgda = sg * da;
            long_type sbda = sb * da;
            long_type sada = sa * da;

            c.r = (value_type)(((srda + drsa <= sada) ? 
                sr * d1a + dr * s1a :
                sa * (srda + drsa - sada) / sr + sr * d1a + dr * s1a + base_mask) >> base_shift);

            c.g = (value_type)(((sgda + dgsa <= sada) ? 
                sg * d1a + dg * s1a :
                sa * (sgda + dgsa - sada) / sg + sg * d1a + dg * s1a + base_mask) >> base_shift);

            c.b = (value_type)(((sbda + dbsa <= sada) ? 
                sb * d1a + db * s1a :
                sa * (sbda + dbsa - sada) / sb + sb * d1a + db * s1a + base_mask) >> base_shift);
            *p = blender_type::make_pix(c.r, c.g, c.b);
        }
    }
};

// composite_op_rgb_16_hard_light
template <typename ColorType, typename Order, typename Blender>
struct composite_op_rgb_16_hard_light
{
    typedef ColorType color_type;
    typedef Blender blender_type;
    typedef uint16_t pixel_type;
    typedef typename color_type::value_type value_type;
    typedef typename color_type::calc_type calc_type;
    typedef typename color_type::long_type long_type;

    enum {
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };

    // if 2.Sca < Sa
    //    Dca' = 2.Sca.Dca + Sca.(1 - Da) + Dca.(1 - Sa)
    // otherwise
    //    Dca' = Sa.Da - 2.(Da - Dca).(Sa - Sca) + Sca.(1 - Da) + Dca.(1 - Sa)
    // 
    // Da'  = Sa + Da - Sa.Da
    static void blend_pix(pixel_type* p, unsigned int sr, unsigned int sg,
                                         unsigned int sb, unsigned int sa, unsigned int cover)
    {
        if (cover < 255) {
            sr = (sr * cover + 255) >> 8;
            sg = (sg * cover + 255) >> 8;
            sb = (sb * cover + 255) >> 8;
            sa = (sa * cover + 255) >> 8;
        }

        if (sa) {
            color_type c = blender_type::make_color(*p);
            calc_type d1a  = 0;
            calc_type s1a  = base_mask - sa;
            calc_type dr   = c.r;
            calc_type dg   = c.g;
            calc_type db   = c.b;
            calc_type da   = base_mask;
            calc_type sada = sa * da;

            c.r = (value_type)(((2*sr < sa) ? 
                2*sr*dr + sr*d1a + dr*s1a : 
                sada - 2*(da - dr)*(sa - sr) + sr*d1a + dr*s1a + base_mask) >> base_shift);

            c.g = (value_type)(((2*sg < sa) ? 
                2*sg*dg + sg*d1a + dg*s1a : 
                sada - 2*(da - dg)*(sa - sg) + sg*d1a + dg*s1a + base_mask) >> base_shift);

            c.b = (value_type)(((2*sb < sa) ? 
                2*sb*db + sb*d1a + db*s1a : 
                sada - 2*(da - db)*(sa - sb) + sb*d1a + db*s1a + base_mask) >> base_shift);
            *p = blender_type::make_pix(c.r, c.g, c.b);
        }
    }
};

// composite_op_rgb_16_soft_light
template <typename ColorType, typename Order, typename Blender>
struct composite_op_rgb_16_soft_light
{
    typedef ColorType color_type;
    typedef Blender blender_type;
    typedef uint16_t pixel_type;
    typedef typename color_type::value_type value_type;
    typedef typename color_type::calc_type calc_type;
    typedef typename color_type::long_type long_type;

    enum {
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };

    // if 2.Sca < Sa
    //   Dca' = Dca.(Sa + (1 - Dca/Da).(2.Sca - Sa)) + Sca.(1 - Da) + Dca.(1 - Sa)
    // otherwise if 8.Dca <= Da
    //   Dca' = Dca.(Sa + (1 - Dca/Da).(2.Sca - Sa).(3 - 8.Dca/Da)) + Sca.(1 - Da) + Dca.(1 - Sa)
    // otherwise
    //   Dca' = (Dca.Sa + ((Dca/Da)^(0.5).Da - Dca).(2.Sca - Sa)) + Sca.(1 - Da) + Dca.(1 - Sa)
    // 
    // Da'  = Sa + Da - Sa.Da 

    static void blend_pix(pixel_type* p, unsigned int r, unsigned int g,
                                         unsigned int b, unsigned int a, unsigned int cover)
    {
        scalar sr = INT_TO_SCALAR(r * cover) / (base_mask * 255);
        scalar sg = INT_TO_SCALAR(g * cover) / (base_mask * 255);
        scalar sb = INT_TO_SCALAR(b * cover) / (base_mask * 255);
        scalar sa = INT_TO_SCALAR(a * cover) / (base_mask * 255);
        if (sa > 0) {
            color_type c = blender_type::make_color(*p);
            scalar dr = INT_TO_SCALAR(c.r) / base_mask;
            scalar dg = INT_TO_SCALAR(c.g) / base_mask;
            scalar db = INT_TO_SCALAR(c.b) / base_mask;
            scalar da = INT_TO_SCALAR(base_mask) / base_mask;
            if (cover < 255) {
                a = (a * cover + 255) >> 8;
            }

            if (2*sr < sa)       dr = dr*(sa + (1 - dr/da)*(2*sr - sa)) + sr*(1 - da) + dr*(1 - sa);
            else if (8*dr <= da) dr = dr*(sa + (1 - dr/da)*(2*sr - sa)*(3 - 8*dr/da)) + sr*(1 - da) + dr*(1 - sa);
            else                 dr = (dr*sa + (Sqrt(dr/da)*da - dr)*(2*sr - sa)) + sr*(1 - da) + dr*(1 - sa);

            if (2*sg < sa)       dg = dg*(sa + (1 - dg/da)*(2*sg - sa)) + sg*(1 - da) + dg*(1 - sa);
            else if (8*dg <= da) dg = dg*(sa + (1 - dg/da)*(2*sg - sa)*(3 - 8*dg/da)) + sg*(1 - da) + dg*(1 - sa);
            else                 dg = (dg*sa + (Sqrt(dg/da)*da - dg)*(2*sg - sa)) + sg*(1 - da) + dg*(1 - sa);

            if(2*sb < sa)       db = db*(sa + (1 - db/da)*(2*sb - sa)) + sb*(1 - da) + db*(1 - sa);
            else if(8*db <= da) db = db*(sa + (1 - db/da)*(2*sb - sa)*(3 - 8*db/da)) + sb*(1 - da) + db*(1 - sa);
            else                db = (db*sa + (Sqrt(db/da)*da - db)*(2*sb - sa)) + sb*(1 - da) + db*(1 - sa);

            c.r = (value_type)uround(dr * base_mask);
            c.g = (value_type)uround(dg * base_mask);
            c.b = (value_type)uround(db * base_mask);
            *p = blender_type::make_pix(c.r, c.g, c.b);
        }
    }
};

// composite_op_rgb_16_difference
template <typename ColorType, typename Order, typename Blender>
struct composite_op_rgb_16_difference
{
    typedef ColorType color_type;
    typedef Blender blender_type;
    typedef uint16_t pixel_type;
    typedef typename color_type::value_type value_type;
    typedef typename color_type::calc_type calc_type;
    typedef typename color_type::long_type long_type;

    enum {
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };

    // Dca' = Sca + Dca - 2.min(Sca.Da, Dca.Sa)
    // Da'  = Sa + Da - Sa.Da 
    static void blend_pix(pixel_type* p, unsigned int sr, unsigned int sg,
                                         unsigned int sb, unsigned int sa, unsigned int cover)
    {
        if (cover < 255) {
            sr = (sr * cover + 255) >> 8;
            sg = (sg * cover + 255) >> 8;
            sb = (sb * cover + 255) >> 8;
            sa = (sa * cover + 255) >> 8;
        }

        if (sa) {
            color_type c = blender_type::make_color(*p);
            calc_type dr = c.r;
            calc_type dg = c.g;
            calc_type db = c.b;
            calc_type da = base_mask;
            c.r = (value_type)(sr + dr - ((2 * Min(sr*da, dr*sa) + base_mask) >> base_shift));
            c.g = (value_type)(sg + dg - ((2 * Min(sg*da, dg*sa) + base_mask) >> base_shift));
            c.b = (value_type)(sb + db - ((2 * Min(sb*da, db*sa) + base_mask) >> base_shift));
            *p = blender_type::make_pix(c.r, c.g, c.b);
        }
    }
};

// composite_op_rgb_16_exclusion
template <typename ColorType, typename Order, typename Blender>
struct composite_op_rgb_16_exclusion
{
    typedef ColorType color_type;
    typedef Blender blender_type;
    typedef uint16_t pixel_type;
    typedef typename color_type::value_type value_type;
    typedef typename color_type::calc_type calc_type;
    typedef typename color_type::long_type long_type;

    enum {
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };

    // Dca' = (Sca.Da + Dca.Sa - 2.Sca.Dca) + Sca.(1 - Da) + Dca.(1 - Sa)
    // Da'  = Sa + Da - Sa.Da 
    static void blend_pix(pixel_type* p, unsigned int sr, unsigned int sg,
                                         unsigned int sb, unsigned int sa, unsigned int cover)
    {
        if (cover < 255) {
            sr = (sr * cover + 255) >> 8;
            sg = (sg * cover + 255) >> 8;
            sb = (sb * cover + 255) >> 8;
            sa = (sa * cover + 255) >> 8;
        }

        if (sa) {
            color_type c = blender_type::make_color(*p);
            calc_type d1a = 0;
            calc_type s1a = base_mask - sa;
            calc_type dr = c.r;
            calc_type dg = c.g;
            calc_type db = c.b;
            calc_type da = base_mask;
            c.r = (value_type)((sr*da + dr*sa - 2*sr*dr + sr*d1a + dr*s1a + base_mask) >> base_shift);
            c.g = (value_type)((sg*da + dg*sa - 2*sg*dg + sg*d1a + dg*s1a + base_mask) >> base_shift);
            c.b = (value_type)((sb*da + db*sa - 2*sb*db + sb*d1a + db*s1a + base_mask) >> base_shift);
            *p = blender_type::make_pix(c.r, c.g, c.b);
        }
    }
};

// composite_op_rgb_16_contrast
template <typename ColorType, typename Order, typename Blender>
struct composite_op_rgb_16_contrast
{
    typedef ColorType color_type;
    typedef Blender blender_type;
    typedef uint16_t pixel_type;
    typedef typename color_type::value_type value_type;
    typedef typename color_type::calc_type calc_type;
    typedef typename color_type::long_type long_type;

    enum { 
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };


    static void blend_pix(pixel_type* p, unsigned int sr, unsigned int sg,
                                         unsigned int sb, unsigned int sa, unsigned int cover)
    {
        if (cover < 255) {
            sr = (sr * cover + 255) >> 8;
            sg = (sg * cover + 255) >> 8;
            sb = (sb * cover + 255) >> 8;
            sa = (sa * cover + 255) >> 8;
        }
        color_type c = blender_type::make_color(*p);
        long_type dr = c.r;
        long_type dg = c.g;
        long_type db = c.b;
        int       da = base_mask;
        long_type d2a = da >> 1;
        unsigned int s2a = sa >> 1;

        int r = (int)((((dr - d2a) * int((sr - s2a)*2 + base_mask)) >> base_shift) + d2a); 
        int g = (int)((((dg - d2a) * int((sg - s2a)*2 + base_mask)) >> base_shift) + d2a); 
        int b = (int)((((db - d2a) * int((sb - s2a)*2 + base_mask)) >> base_shift) + d2a); 

        r = (r < 0) ? 0 : r;
        g = (g < 0) ? 0 : g;
        b = (b < 0) ? 0 : b;

        c.r = (value_type)((r > da) ? da : r);
        c.g = (value_type)((g > da) ? da : g);
        c.b = (value_type)((b > da) ? da : b);
        *p = blender_type::make_pix(c.r, c.g, c.b);
    }
};

// composite_op_rgb_16_invert
template <typename ColorType, typename Order, typename Blender>
struct composite_op_rgb_16_invert
{
    typedef ColorType color_type;
    typedef Blender blender_type;
    typedef uint16_t pixel_type;
    typedef typename color_type::value_type value_type;
    typedef typename color_type::calc_type calc_type;
    typedef typename color_type::long_type long_type;

    enum {
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };

    // Dca' = (Da - Dca) * Sa + Dca.(1 - Sa)
    // Da'  = Sa + Da - Sa.Da 
    static void blend_pix(pixel_type* p, unsigned int sr, unsigned int sg,
                                         unsigned int sb, unsigned int sa, unsigned int cover)
    {
        sa = (sa * cover + 255) >> 8;
        if (sa) {
            color_type c = blender_type::make_color(*p);
            calc_type da = base_mask;
            calc_type dr = ((da - c.r) * sa + base_mask) >> base_shift;
            calc_type dg = ((da - c.g) * sa + base_mask) >> base_shift;
            calc_type db = ((da - c.b) * sa + base_mask) >> base_shift;
            calc_type s1a = base_mask - sa;
            c.r = (value_type)(dr + ((c.r * s1a + base_mask) >> base_shift));
            c.g = (value_type)(dg + ((c.g * s1a + base_mask) >> base_shift));
            c.b = (value_type)(db + ((c.b * s1a + base_mask) >> base_shift));
            *p = blender_type::make_pix(c.r, c.g, c.b);
        }
    }
};

// composite_op_rgb_16_invert_rgb
template <typename ColorType, typename Order, typename Blender>
struct composite_op_rgb_16_invert_rgb
{
    typedef ColorType color_type;
    typedef Blender blender_type;
    typedef uint16_t pixel_type;
    typedef typename color_type::value_type value_type;
    typedef typename color_type::calc_type calc_type;
    typedef typename color_type::long_type long_type;

    enum {
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };

    // Dca' = (Da - Dca) * Sca + Dca.(1 - Sa)
    // Da'  = Sa + Da - Sa.Da 
    static void blend_pix(pixel_type* p, unsigned int sr, unsigned int sg,
                                         unsigned int sb, unsigned int sa, unsigned int cover)
    {
        if (cover < 255) {
            sr = (sr * cover + 255) >> 8;
            sg = (sg * cover + 255) >> 8;
            sb = (sb * cover + 255) >> 8;
            sa = (sa * cover + 255) >> 8;
        }

        if (sa) {
            color_type c = blender_type::make_color(*p);
            calc_type da = base_mask;
            calc_type dr = ((da - c.r) * sr + base_mask) >> base_shift;
            calc_type dg = ((da - c.g) * sg + base_mask) >> base_shift;
            calc_type db = ((da - c.b) * sb + base_mask) >> base_shift;
            calc_type s1a = base_mask - sa;
            c.r = (value_type)(dr + ((c.r * s1a + base_mask) >> base_shift));
            c.g = (value_type)(dg + ((c.g * s1a + base_mask) >> base_shift));
            c.b = (value_type)(db + ((c.b * s1a + base_mask) >> base_shift));
            *p = blender_type::make_pix(c.r, c.g, c.b);
        }
    }
};


// composite operate table for blend rgb pixel format.
template <typename ColorType, typename Order, typename Blender>
struct blend_op_table_rgb_16
{
    typedef uint16_t pixel_type;
    typedef typename ColorType::value_type value_type;
    typedef void (*composite_op_func_type)(pixel_type* p, 
                                      unsigned int cr, 
                                      unsigned int cg, 
                                      unsigned int cb,
                                      unsigned int ca,
                                      unsigned int cover);

    static composite_op_func_type g_rgb_16_blend_op_func[];
};


// g_rgb_16_comp_op_func
template <typename ColorType, typename Order, typename Blender> 
typename blend_op_table_rgb_16<ColorType, Order, Blender>::composite_op_func_type
blend_op_table_rgb_16<ColorType, Order, Blender>::g_rgb_16_blend_op_func[] = 
{
    composite_op_rgb_16_clear      <ColorType,Order,Blender>::blend_pix,
    composite_op_rgb_16_src        <ColorType,Order,Blender>::blend_pix,
    composite_op_rgb_16_src_over   <ColorType,Order,Blender>::blend_pix,
    composite_op_rgb_16_src_in     <ColorType,Order,Blender>::blend_pix,
    composite_op_rgb_16_src_out    <ColorType,Order,Blender>::blend_pix,
    composite_op_rgb_16_src_atop   <ColorType,Order,Blender>::blend_pix,
    composite_op_rgb_16_dst        <ColorType,Order,Blender>::blend_pix,
    composite_op_rgb_16_dst_over   <ColorType,Order,Blender>::blend_pix,
    composite_op_rgb_16_dst_in     <ColorType,Order,Blender>::blend_pix,
    composite_op_rgb_16_dst_out    <ColorType,Order,Blender>::blend_pix,
    composite_op_rgb_16_dst_atop   <ColorType,Order,Blender>::blend_pix,
    composite_op_rgb_16_xor        <ColorType,Order,Blender>::blend_pix,
    composite_op_rgb_16_darken     <ColorType,Order,Blender>::blend_pix,
    composite_op_rgb_16_lighten    <ColorType,Order,Blender>::blend_pix,
    composite_op_rgb_16_overlay    <ColorType,Order,Blender>::blend_pix,
    composite_op_rgb_16_screen     <ColorType,Order,Blender>::blend_pix,
    composite_op_rgb_16_multiply   <ColorType,Order,Blender>::blend_pix,
    composite_op_rgb_16_plus       <ColorType,Order,Blender>::blend_pix,
    composite_op_rgb_16_minus      <ColorType,Order,Blender>::blend_pix,
    composite_op_rgb_16_exclusion  <ColorType,Order,Blender>::blend_pix,
    composite_op_rgb_16_difference <ColorType,Order,Blender>::blend_pix,
    composite_op_rgb_16_soft_light <ColorType,Order,Blender>::blend_pix,
    composite_op_rgb_16_hard_light <ColorType,Order,Blender>::blend_pix,
    composite_op_rgb_16_color_burn <ColorType,Order,Blender>::blend_pix,
    composite_op_rgb_16_color_dodge<ColorType,Order,Blender>::blend_pix,
    composite_op_rgb_16_contrast   <ColorType,Order,Blender>::blend_pix,
    composite_op_rgb_16_invert     <ColorType,Order,Blender>::blend_pix,
    composite_op_rgb_16_invert_rgb <ColorType,Order,Blender>::blend_pix,
    0
};

// blender_rgb555
class blender_rgb555
{
public:
    typedef rgba8 color_type;
    typedef order_rgb555 order_type;
    typedef color_type::value_type value_type;
    typedef color_type::calc_type calc_type;
    typedef uint16_t pixel_type;

    enum {
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };

    static void blend_pix(unsigned int op, pixel_type* p, 
                          unsigned int cr, unsigned int cg,
                          unsigned int cb, unsigned int ca, unsigned int cover)
    {
        blend_op_table_rgb_16<color_type, order_type, blender_rgb555>::g_rgb_16_blend_op_func[op]
            (p, (cr * ca + base_mask) >> base_shift, 
                (cg * ca + base_mask) >> base_shift,
                (cb * ca + base_mask) >> base_shift,
                 ca, cover);
    }

    static void blend_pix(pixel_type* p, unsigned int cr, unsigned int cg,
                                         unsigned int cb, unsigned int ca, unsigned int)
    {
        register pixel_type rgb = *p;
        calc_type r = (rgb >> 7) & 0xF8;
        calc_type g = (rgb >> 2) & 0xF8;
        calc_type b = (rgb << 3) & 0xF8;
        *p = (pixel_type)(((((cr - r) * ca + (r << 8)) >> 1)  & 0x7C00) |
                         ((((cg - g) * ca + (g << 8)) >> 6)  & 0x03E0) |
                         (((cb - b) * ca + (b << 8)) >> 11) | 0x8000);
    }

    static pixel_type make_pix(unsigned int r, unsigned int g, unsigned int b)
    {
        return (pixel_type)(((r & 0xF8) << 7) | ((g & 0xF8) << 2) | (b >> 3) | 0x8000);
    }

    static color_type make_color(pixel_type p)
    {
        return color_type((p >> 7) & 0xF8, (p >> 2) & 0xF8, (p << 3) & 0xF8);
    }
};

// blender_rgb565
class blender_rgb565
{
public:
    typedef rgba8 color_type;
    typedef order_rgb565 order_type;
    typedef color_type::value_type value_type;
    typedef color_type::calc_type calc_type;
    typedef uint16_t pixel_type;

    enum {
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };

    static void blend_pix(unsigned int op, pixel_type* p, 
                          unsigned int cr, unsigned int cg,
                          unsigned int cb, unsigned int ca, unsigned int cover)
    {
        blend_op_table_rgb_16<color_type, order_type, blender_rgb565>::g_rgb_16_blend_op_func[op]
            (p, (cr * ca + base_mask) >> base_shift, 
                (cg * ca + base_mask) >> base_shift,
                (cb * ca + base_mask) >> base_shift,
                 ca, cover);
    }

    static void blend_pix(pixel_type* p, unsigned int cr, unsigned int cg,
                                         unsigned int cb, unsigned int ca, unsigned int)
    {
        register pixel_type rgb = *p;
        calc_type r = (rgb >> 8) & 0xF8;
        calc_type g = (rgb >> 3) & 0xFC;
        calc_type b = (rgb << 3) & 0xF8;
        *p = (pixel_type)(((((cr - r) * ca + (r << 8))) & 0xF800) |
                         ((((cg - g) * ca + (g << 8)) >> 5) & 0x07E0) |
                         (((cb - b) * ca + (b << 8)) >> 11));
    }

    static pixel_type make_pix(unsigned int r, unsigned int g, unsigned int b)
    {
        return (pixel_type)(((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3));
    }

    static color_type make_color(pixel_type p)
    {
        return color_type((p >> 8) & 0xF8, (p >> 3) & 0xFC, (p << 3) & 0xF8);
    }
};


// pixfmt blender rgb16
template <typename Blender, typename RenBuffer>
class pixfmt_blender_rgb16
{
public:
    typedef RenBuffer buffer_type;
    typedef typename buffer_type::row_data row_data;
    typedef Blender  blender_type;
    typedef typename blender_type::pixel_type pixel_type;
    typedef typename blender_type::color_type color_type;
    typedef typename blender_type::order_type   order_type;
    typedef typename color_type::value_type   value_type;

    enum {
        base_shift = color_type::base_shift,
        base_scale = color_type::base_scale,
        base_mask  = color_type::base_mask,
        pix_width  = sizeof(pixel_type)
    };

    pixfmt_blender_rgb16()
        : m_buffer(0)
        , m_blend_op(comp_op_src_over)
        , m_alpha_factor(base_mask)
    {
    }

    explicit pixfmt_blender_rgb16(buffer_type& rb, unsigned int op = comp_op_src_over, unsigned int alpha = base_mask)
        : m_buffer(&rb)
        , m_blend_op(op)
        , m_alpha_factor(alpha) 
    {
    }

    void attach(buffer_type& rb) { m_buffer = &rb; }

    unsigned int width(void) const { return m_buffer->internal_width(); }
    unsigned int height(void) const { return m_buffer->internal_height(); }
    int stride(void) const { return m_buffer->internal_stride(); }

    byte* row_ptr(int y) { return m_buffer->row_ptr(y); }
    const byte* row_ptr(int y) const { return m_buffer->row_ptr(y); }
    row_data row(int y) const { return m_buffer->row(y); }

    void alpha(scalar a) { m_alpha_factor = uround(a * base_mask); }
    scalar alpha(void) const { return INT_TO_SCALAR(m_alpha_factor) / FLT_TO_SCALAR(255.0f); }

    byte* pix_ptr(int x, int y) const
    {
        return m_buffer->row_ptr(y) + x * pix_width;
    }

    void blend_op(unsigned int op) { m_blend_op = op; }
    unsigned int blend_op(void) const { return m_blend_op; }

    color_type pixel(int x, int y) const
    {
        return blender_type::make_color(((pixel_type*)m_buffer->row_ptr(y))[x]);
    }

    void copy_pixel(int x, int y, const color_type& c)
    {
        ((pixel_type*)m_buffer->row_ptr(x, y, 1))[x] = blender_type::make_pix(c.r, c.g, c.b);
    }

    void blend_pixel(int x, int y, const color_type& c, uint8_t cover)
    {
        blender_type::blend_pix(m_blend_op,
            (pixel_type*)m_buffer->row_ptr(x, y, 1) + x,
             c.r, c.g, c.b, (value_type)alpha_mul(c.a, m_alpha_factor), cover);
    }

    void copy_hline(int x, int y, unsigned int len, const color_type& c)
    {
        pixel_type* p = (pixel_type*)m_buffer->row_ptr(x, y, len) + x;
        pixel_type v = blender_type::make_pix(c.r, c.g, c.b);
        do {
            *p++ = v;
        } while(--len);
    }

    void copy_vline(int x, int y, unsigned int len, const color_type& c)
    {
        pixel_type v = blender_type::make_pix(c.r, c.g, c.b);
        do {
            pixel_type* p = (pixel_type*)m_buffer->row_ptr(x, y++, 1) + x;
            *p = v;
        } while(--len);
    }

    void blend_hline(int x, int y, unsigned int len, const color_type& c, uint8_t cover)
    {
        pixel_type* p = (pixel_type*)m_buffer->row_ptr(x, y, len) + x;
        do {
            blender_type::blend_pix(m_blend_op, p, c.r, c.g, c.b, (value_type)alpha_mul(c.a, m_alpha_factor), cover);
            p++;
        } while(--len);
    }

    void blend_vline(int x, int y, unsigned int len, const color_type& c, uint8_t cover)
    {
        do {
            blender_type::blend_pix(m_blend_op, 
                (pixel_type*)m_buffer->row_ptr(x, y++, 1) + x, 
                 c.r, c.g, c.b, (value_type)alpha_mul(c.a, m_alpha_factor), cover);
        } while(--len);
    }

    void blend_solid_hspan(int x, int y, unsigned int len, const color_type& c, const uint8_t* covers)
    {
        pixel_type* p = (pixel_type*)m_buffer->row_ptr(x, y, len) + x;
        do {
            blender_type::blend_pix(m_blend_op, 
                          p, c.r, c.g, c.b, (value_type)alpha_mul(c.a, m_alpha_factor), *covers++);
            p++;
        } while(--len);
    }

    void blend_solid_vspan(int x, int y, unsigned int len, const color_type& c, const uint8_t* covers)
    {
        do {
            blender_type::blend_pix(m_blend_op, 
                (pixel_type*)m_buffer->row_ptr(x, y++, 1) + x, 
                 c.r, c.g, c.b, (value_type)alpha_mul(c.a, m_alpha_factor), *covers++);
        } while(--len);
    }

    void copy_color_hspan(int x, int y, unsigned int len, const color_type* colors)
    {
        pixel_type* p = (pixel_type*)m_buffer->row_ptr(x, y, len) + x;
        do {
            *p++ = blender_type::make_pix(colors->r, colors->g, colors->b);
            ++colors;
        } while(--len);
    }

    void copy_color_vspan(int x, int y, unsigned int len, const color_type* colors)
    {
        do {
            pixel_type* p = (pixel_type*)m_buffer->row_ptr(x, y++, 1) + x;
            *p = blender_type::make_pix(colors->r, colors->g, colors->b);
            ++colors;
        } while(--len);
    }

    void blend_color_hspan(int x, int y, unsigned int len,
                           const color_type* colors, const uint8_t* covers, uint8_t cover)
    {
        pixel_type* p = (pixel_type*)m_buffer->row_ptr(x, y, len) + x;
        do {
            blender_type::blend_pix(m_blend_op, p, 
                    colors->r, 
                    colors->g, 
                    colors->b, 
                    (value_type)alpha_mul(colors->a, m_alpha_factor), 
                    covers ? *covers++ : cover);
            p++;
            ++colors;
        } while(--len);
    }

    void blend_color_vspan(int x, int y, unsigned int len,
                           const color_type* colors, const uint8_t* covers, uint8_t cover)
    {
        do {
            blender_type::blend_pix(m_blend_op, 
                (pixel_type*)m_buffer->row_ptr(x, y++, 1) + x, 
                colors->r,
                colors->g,
                colors->b,
                (value_type)alpha_mul(colors->a, m_alpha_factor),
                covers ? *covers++ : cover);
            ++colors;
        } while(--len);
    }
    
    static void make_pix(byte* p, const color_type& c)
    {
        *(pixel_type*)p = blender_type::make_pix(c.r, c.g, c.b);
    }

    template <typename RenBuffer2>
    void copy_point_from(const RenBuffer2& from, int xdst, int ydst, int xsrc, int ysrc)
    {
        const byte* p = from.row_ptr(ysrc);
        if (p) {
            mem_deep_copy(m_buffer->row_ptr(xdst, ydst, 1) + xdst * pix_width, 
                          p + xsrc * pix_width, pix_width);
        }
    }

    template <typename RenBuffer2>
    void copy_from(const RenBuffer2& from, int xdst, int ydst, int xsrc, int ysrc, unsigned int len)
    {
        const byte* p = from.row_ptr(ysrc);
        if (p) {
            mem_deep_copy(m_buffer->row_ptr(xdst, ydst, len) + xdst * pix_width,
                          p + xsrc * pix_width, len * pix_width);
        }
    }

    template <typename SrcPixelFormatRenderer>
    void blend_from(const SrcPixelFormatRenderer& from, int xdst, int ydst,
                                    int xsrc, int ysrc, unsigned int len, uint8_t cover)
    {
        typedef typename SrcPixelFormatRenderer::order_type src_order;
        const value_type* psrc = (const value_type*)from.row_ptr(ysrc);
        if (psrc) {
            psrc += xsrc << 2;
            pixel_type* pdst = 
                (pixel_type*)m_buffer->row_ptr(xdst, ydst, len) + xdst;

            do {
                blender_type::blend_pix(m_blend_op, pdst, 
                                        psrc[src_order::R],
                                        psrc[src_order::G],
                                        psrc[src_order::B],
                                        (value_type)alpha_mul(psrc[src_order::A], m_alpha_factor), cover);
                psrc += 4;
                ++pdst;
            } while(--len);
        }
    }

    template <typename SrcPixelFormatRenderer>
    void blend_point_from(const SrcPixelFormatRenderer& from, int xdst, int ydst,
                                            int xsrc, int ysrc, uint8_t cover)
    {
        typedef typename SrcPixelFormatRenderer::order_type src_order;
        const value_type* psrc = (const value_type*)from.row_ptr(ysrc);
        if (psrc) {
            psrc += xsrc << 2;
            pixel_type* pdst = (pixel_type*)m_buffer->row_ptr(xdst, ydst, 1) + xdst;

            blender_type::blend_pix(m_blend_op, pdst,
                    psrc[src_order::R],
                    psrc[src_order::G],
                    psrc[src_order::B],
                    (value_type)alpha_mul(psrc[src_order::A], m_alpha_factor), cover);
        }
    }

    template <typename SrcPixelFormatRenderer>
    void blend_from_color(const SrcPixelFormatRenderer& from, const color_type& color,
                          int xdst, int ydst, int xsrc, int ysrc, unsigned int len, uint8_t cover)
    {
        typedef typename SrcPixelFormatRenderer::value_type src_value_type;
        const src_value_type* psrc = (src_value_type*)from.row_ptr(ysrc);
        if (psrc) {
            pixel_type* pdst = (pixel_type*)m_buffer->row_ptr(xdst, ydst, len) + xdst;

            do {
                blender_type::blend_pix(m_blend_op, pdst, 
                                        color.r, color.g, color.b, (value_type)alpha_mul(color.a, m_alpha_factor),
                                        (*psrc * cover + base_mask) >> base_shift);
                ++psrc;
                ++pdst;
            } while(--len);
        }
    }

    template <class SrcPixelFormatRenderer>
    void blend_from_lut(const SrcPixelFormatRenderer& from, const color_type* color_lut,
                        int xdst, int ydst, int xsrc, int ysrc, unsigned int len, uint8_t cover)
    {
        typedef typename SrcPixelFormatRenderer::value_type src_value_type;
        const src_value_type* psrc = (src_value_type*)from.row_ptr(ysrc);
        if (psrc) {
            pixel_type* pdst = (pixel_type*)m_buffer->row_ptr(xdst, ydst, len) + xdst;

            do {
                const color_type& color = color_lut[*psrc];
                blender_type::blend_pix(m_blend_op, pdst, 
                                        color.r, color.g, color.b, (value_type)alpha_mul(color.a, m_alpha_factor),
                                        cover);
                ++psrc;
                ++pdst;
            } while(--len);
        }
    }

private:
    unsigned int alpha_mul(unsigned int a, unsigned int s)
    {
        return (s == 255) ? a : ((a * s + base_mask) >> base_shift);
    }

    pixfmt_blender_rgb16(const pixfmt_blender_rgb16&);
    pixfmt_blender_rgb16& operator=(const pixfmt_blender_rgb16&);
private:
    buffer_type* m_buffer;
    unsigned int m_blend_op;
    unsigned int m_alpha_factor;
};


typedef pixfmt_blender_rgb16<blender_rgb555, gfx_rendering_buffer> pixfmt_rgb555; // pixfmt_rgb555
typedef pixfmt_blender_rgb16<blender_rgb565, gfx_rendering_buffer> pixfmt_rgb565; // pixfmt_rgb565

}
#endif /*_GFX_PIXFMT_RGB16_H_*/
