/* Picasso - a vector graphics library
 * 
 * Copyright (C) 2014 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#ifndef _GFX_PIXFMT_RGBA_H_
#define _GFX_PIXFMT_RGBA_H_

#include "common.h"
#include "gfx_rendering_buffer.h"

namespace gfx {

// composite_op_rgba_clear
template <typename ColorType, typename Order>
struct composite_op_rgba_clear
{
    typedef ColorType color_type;
    typedef typename color_type::value_type value_type;

    static void blend_pix(value_type* p, unsigned int, unsigned int,
                          unsigned int, unsigned int, unsigned int cover)
    {
        if (cover < 255) {
            cover = 255 - cover;
            p[Order::R] = (value_type)((p[Order::R] * cover + 255) >> 8);
            p[Order::G] = (value_type)((p[Order::G] * cover + 255) >> 8);
            p[Order::B] = (value_type)((p[Order::B] * cover + 255) >> 8);
            p[Order::A] = (value_type)((p[Order::A] * cover + 255) >> 8);
        } else {
            p[0] = p[1] = p[2] = p[3] = 0; 
        }
    }
};

// composite_op_rgba_src
template <typename ColorType, typename Order>
struct composite_op_rgba_src
{
    typedef ColorType color_type;
    typedef typename color_type::value_type value_type;

    static void blend_pix(value_type* p, unsigned int sr, unsigned int sg,
                          unsigned int sb, unsigned int sa, unsigned int cover)
    {
        if (cover < 255) {
            unsigned int alpha = 255 - cover;
            p[Order::R] = (value_type)(((p[Order::R] * alpha + 255) >> 8) + ((sr * cover + 255) >> 8));
            p[Order::G] = (value_type)(((p[Order::G] * alpha + 255) >> 8) + ((sg * cover + 255) >> 8));
            p[Order::B] = (value_type)(((p[Order::B] * alpha + 255) >> 8) + ((sb * cover + 255) >> 8));
            p[Order::A] = (value_type)(((p[Order::A] * alpha + 255) >> 8) + ((sa * cover + 255) >> 8));
        } else {
            p[Order::R] = sr;
            p[Order::G] = sg;
            p[Order::B] = sb;
            p[Order::A] = sa;
        }
    }
};

// composite_op_rgba_dst
template <typename ColorType, typename Order>
struct composite_op_rgba_dst
{
    typedef ColorType color_type;
    typedef typename color_type::value_type value_type;

    static void blend_pix(value_type*, unsigned int, unsigned int,
                          unsigned int, unsigned int, unsigned int)
    {
    }
};

// composite_op_rgba_src_over
template <typename ColorType, typename Order>
struct composite_op_rgba_src_over
{
    typedef ColorType color_type;
    typedef typename color_type::value_type value_type;
    typedef typename color_type::calc_type calc_type;

    enum { 
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };

    // Dca' = Sca + Dca.(1 - Sa)
    // Da'  = Sa + Da - Sa.Da 
    static void blend_pix(value_type* p, unsigned int sr, unsigned int sg,
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
            p[Order::R] = (value_type)(sr + ((p[Order::R] * s1a + base_mask) >> base_shift));
            p[Order::G] = (value_type)(sg + ((p[Order::G] * s1a + base_mask) >> base_shift));
            p[Order::B] = (value_type)(sb + ((p[Order::B] * s1a + base_mask) >> base_shift));
            p[Order::A] = (value_type)(sa + p[Order::A] - ((sa * p[Order::A] + base_mask) >> base_shift));
        } else { // fast for opaque
            p[Order::R] = (value_type)sr;
            p[Order::G] = (value_type)sg;
            p[Order::B] = (value_type)sb;
            p[Order::A] = (value_type)sa;
        }
    }
};

// composite_op_rgba_dst_over
template <typename ColorType, typename Order>
struct composite_op_rgba_dst_over
{
    typedef ColorType color_type;
    typedef typename color_type::value_type value_type;
    typedef typename color_type::calc_type calc_type;

    enum { 
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };

    // Dca' = Dca + Sca.(1 - Da)
    // Da'  = Sa + Da - Sa.Da 
    static void blend_pix(value_type* p, unsigned int sr, unsigned int sg,
                          unsigned int sb, unsigned int sa, unsigned int cover)
    {
        if (cover < 255) {
            sr = (sr * cover + 255) >> 8;
            sg = (sg * cover + 255) >> 8;
            sb = (sb * cover + 255) >> 8;
            sa = (sa * cover + 255) >> 8;
        }

        calc_type d1a = base_mask - p[Order::A];
        p[Order::R] = (value_type)(p[Order::R] + ((sr * d1a + base_mask) >> base_shift));
        p[Order::G] = (value_type)(p[Order::G] + ((sg * d1a + base_mask) >> base_shift));
        p[Order::B] = (value_type)(p[Order::B] + ((sb * d1a + base_mask) >> base_shift));
        p[Order::A] = (value_type)(sa + p[Order::A] - ((sa * p[Order::A] + base_mask) >> base_shift));
    }
};

// composite_op_rgba_src_in
template <typename ColorType, typename Order>
struct composite_op_rgba_src_in
{
    typedef ColorType color_type;
    typedef typename color_type::value_type value_type;
    typedef typename color_type::calc_type calc_type;

    enum { 
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };

    // Dca' = Sca.Da
    // Da'  = Sa.Da 
    static void blend_pix(value_type* p, unsigned int sr, unsigned int sg,
                          unsigned int sb, unsigned int sa, unsigned int cover)
    {
        calc_type da = p[Order::A];
        if (cover < 255) {
            unsigned int alpha = 255 - cover;
            p[Order::R] = (value_type)(((p[Order::R] * alpha + 255) >> 8) +
                          ((((sr * da + base_mask) >> base_shift) * cover + 255) >> 8));
            p[Order::G] = (value_type)(((p[Order::G] * alpha + 255) >> 8) +
                          ((((sg * da + base_mask) >> base_shift) * cover + 255) >> 8));
            p[Order::B] = (value_type)(((p[Order::B] * alpha + 255) >> 8) +
                          ((((sb * da + base_mask) >> base_shift) * cover + 255) >> 8));
            p[Order::A] = (value_type)(((p[Order::A] * alpha + 255) >> 8) +
                          ((((sa * da + base_mask) >> base_shift) * cover + 255) >> 8));
        } else {
            p[Order::R] = (value_type)((sr * da + base_mask) >> base_shift);
            p[Order::G] = (value_type)((sg * da + base_mask) >> base_shift);
            p[Order::B] = (value_type)((sb * da + base_mask) >> base_shift);
            p[Order::A] = (value_type)((sa * da + base_mask) >> base_shift);
        }
    }
};

// composite_op_rgba_dst_in
template <typename ColorType, typename Order>
struct composite_op_rgba_dst_in
{
    typedef ColorType color_type;
    typedef typename color_type::value_type value_type;

    enum { 
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };

    // Dca' = Dca.Sa
    // Da'  = Sa.Da 
    static void blend_pix(value_type* p, unsigned int, unsigned int, unsigned int, 
                                         unsigned int sa, unsigned int cover)
    {
        if (cover < 255) {
            sa = base_mask - ((cover * (base_mask - sa) + 255) >> 8);
        }

        p[Order::R] = (value_type)((p[Order::R] * sa + base_mask) >> base_shift);
        p[Order::G] = (value_type)((p[Order::G] * sa + base_mask) >> base_shift);
        p[Order::B] = (value_type)((p[Order::B] * sa + base_mask) >> base_shift);
        p[Order::A] = (value_type)((p[Order::A] * sa + base_mask) >> base_shift);
    }
};

// composite_op_rgba_src_out
template <typename ColorType, typename Order>
struct composite_op_rgba_src_out
{
    typedef ColorType color_type;
    typedef typename color_type::value_type value_type;
    typedef typename color_type::calc_type calc_type;

    enum { 
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };

    // Dca' = Sca.(1 - Da)
    // Da'  = Sa.(1 - Da) 
    static void blend_pix(value_type* p, unsigned int sr, unsigned int sg,
                                         unsigned int sb, unsigned int sa, unsigned int cover)
    {
        calc_type da = base_mask - p[Order::A];
        if (cover < 255) {
            unsigned int alpha = 255 - cover;
            p[Order::R] = (value_type)(((p[Order::R] * alpha + 255) >> 8) +
                          ((((sr * da + base_mask) >> base_shift) * cover + 255) >> 8));
            p[Order::G] = (value_type)(((p[Order::G] * alpha + 255) >> 8) +
                          ((((sg * da + base_mask) >> base_shift) * cover + 255) >> 8));
            p[Order::B] = (value_type)(((p[Order::B] * alpha + 255) >> 8) +
                          ((((sb * da + base_mask) >> base_shift) * cover + 255) >> 8));
            p[Order::A] = (value_type)(((p[Order::A] * alpha + 255) >> 8) +
                          ((((sa * da + base_mask) >> base_shift) * cover + 255) >> 8));
        } else {
            p[Order::R] = (value_type)((sr * da + base_mask) >> base_shift);
            p[Order::G] = (value_type)((sg * da + base_mask) >> base_shift);
            p[Order::B] = (value_type)((sb * da + base_mask) >> base_shift);
            p[Order::A] = (value_type)((sa * da + base_mask) >> base_shift);
        }
    }
};

// composite_op_rgba_dst_out
template <typename ColorType, typename Order>
struct composite_op_rgba_dst_out
{
    typedef ColorType color_type;
    typedef typename color_type::value_type value_type;

    enum { 
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };

    // Dca' = Dca.(1 - Sa) 
    // Da'  = Da.(1 - Sa) 
    static void blend_pix(value_type* p, unsigned int, unsigned int, unsigned int, 
                                         unsigned int sa, unsigned int cover)
    {
        if (cover < 255) {
            sa = (sa * cover + 255) >> 8;
        }

        sa = base_mask - sa;
        p[Order::R] = (value_type)((p[Order::R] * sa + base_shift) >> base_shift);
        p[Order::G] = (value_type)((p[Order::G] * sa + base_shift) >> base_shift);
        p[Order::B] = (value_type)((p[Order::B] * sa + base_shift) >> base_shift);
        p[Order::A] = (value_type)((p[Order::A] * sa + base_shift) >> base_shift);
    }
};

// composite_op_rgba_src_atop
template <typename ColorType, typename Order>
struct composite_op_rgba_src_atop
{
    typedef ColorType color_type;
    typedef typename color_type::value_type value_type;
    typedef typename color_type::calc_type calc_type;

    enum { 
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };

    // Dca' = Sca.Da + Dca.(1 - Sa)
    // Da'  = Da
    static void blend_pix(value_type* p, unsigned int sr, unsigned int sg,
                          unsigned int sb, unsigned int sa, unsigned int cover)
    {
        if (cover < 255) {
            sr = (sr * cover + 255) >> 8;
            sg = (sg * cover + 255) >> 8;
            sb = (sb * cover + 255) >> 8;
            sa = (sa * cover + 255) >> 8;
        }

        calc_type da = p[Order::A];
        sa = base_mask - sa;
        p[Order::R] = (value_type)((sr * da + p[Order::R] * sa + base_mask) >> base_shift);
        p[Order::G] = (value_type)((sg * da + p[Order::G] * sa + base_mask) >> base_shift);
        p[Order::B] = (value_type)((sb * da + p[Order::B] * sa + base_mask) >> base_shift);
    }
};

// composite_op_rgba_dst_atop
template <typename ColorType, typename Order>
struct composite_op_rgba_dst_atop
{
    typedef ColorType color_type;
    typedef typename color_type::value_type value_type;
    typedef typename color_type::calc_type calc_type;

    enum { 
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };

    // Dca' = Dca.Sa + Sca.(1 - Da)
    // Da'  = Sa 
    static void blend_pix(value_type* p, unsigned int sr, unsigned int sg,
                          unsigned int sb, unsigned int sa, unsigned int cover)
    {
        calc_type da = base_mask - p[Order::A];
        if (cover < 255) {
            unsigned int alpha = 255 - cover;
            sr = (p[Order::R] * sa + sr * da + base_mask) >> base_shift;
            sg = (p[Order::G] * sa + sg * da + base_mask) >> base_shift;
            sb = (p[Order::B] * sa + sb * da + base_mask) >> base_shift;
            p[Order::R] = (value_type)(((p[Order::R] * alpha + 255) >> 8) + ((sr * cover + 255) >> 8));
            p[Order::G] = (value_type)(((p[Order::G] * alpha + 255) >> 8) + ((sg * cover + 255) >> 8));
            p[Order::B] = (value_type)(((p[Order::B] * alpha + 255) >> 8) + ((sb * cover + 255) >> 8));
            p[Order::A] = (value_type)(((p[Order::A] * alpha + 255) >> 8) + ((sa * cover + 255) >> 8));

        } else {
            p[Order::R] = (value_type)((p[Order::R] * sa + sr * da + base_mask) >> base_shift);
            p[Order::G] = (value_type)((p[Order::G] * sa + sg * da + base_mask) >> base_shift);
            p[Order::B] = (value_type)((p[Order::B] * sa + sb * da + base_mask) >> base_shift);
            p[Order::A] = (value_type)sa;
        }
    }
};

// composite_op_rgba_xor
template <typename ColorType, typename Order>
struct composite_op_rgba_xor
{
    typedef ColorType color_type;
    typedef typename color_type::value_type value_type;
    typedef typename color_type::calc_type calc_type;

    enum { 
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };

    // Dca' = Sca.(1 - Da) + Dca.(1 - Sa)
    // Da'  = Sa + Da - 2.Sa.Da 
    static void blend_pix(value_type* p, unsigned int sr, unsigned int sg,
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
            calc_type d1a = base_mask - p[Order::A];
            p[Order::R] = (value_type)((p[Order::R] * s1a + sr * d1a + base_mask) >> base_shift);
            p[Order::G] = (value_type)((p[Order::G] * s1a + sg * d1a + base_mask) >> base_shift);
            p[Order::B] = (value_type)((p[Order::B] * s1a + sb * d1a + base_mask) >> base_shift);
            p[Order::A] = (value_type)(sa + p[Order::A] - ((sa * p[Order::A] + base_mask/2) >> (base_shift - 1)));
        }
    }
};

// composite_op_rgba_plus
template <typename ColorType, typename Order>
struct composite_op_rgba_plus
{
    typedef ColorType color_type;
    typedef typename color_type::value_type value_type;
    typedef typename color_type::calc_type calc_type;

    enum { 
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };

    // Dca' = Sca + Dca
    // Da'  = Sa + Da 
    static void blend_pix(value_type* p, unsigned int sr, unsigned int sg,
                          unsigned int sb, unsigned int sa, unsigned int cover)
    {
        if (cover < 255) {
            sr = (sr * cover + 255) >> 8;
            sg = (sg * cover + 255) >> 8;
            sb = (sb * cover + 255) >> 8;
            sa = (sa * cover + 255) >> 8;
        }

        if (sa) {
            calc_type dr = p[Order::R] + sr;
            calc_type dg = p[Order::G] + sg;
            calc_type db = p[Order::B] + sb;
            calc_type da = p[Order::A] + sa;
            p[Order::R] = (dr > base_mask) ? (value_type)base_mask : dr;
            p[Order::G] = (dg > base_mask) ? (value_type)base_mask : dg;
            p[Order::B] = (db > base_mask) ? (value_type)base_mask : db;
            p[Order::A] = (da > base_mask) ? (value_type)base_mask : da;
        }
    }
};

// composite_op_rgba_minus
template <typename ColorType, typename Order>
struct composite_op_rgba_minus
{
    typedef ColorType color_type;
    typedef typename color_type::value_type value_type;
    typedef typename color_type::calc_type calc_type;

    enum { 
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };

    // Dca' = Dca - Sca
    // Da' = 1 - (1 - Sa).(1 - Da)
    static void blend_pix(value_type* p, unsigned int sr, unsigned int sg,
                          unsigned int sb, unsigned int sa, unsigned int cover)
    {
        if (cover < 255) {
            sr = (sr * cover + 255) >> 8;
            sg = (sg * cover + 255) >> 8;
            sb = (sb * cover + 255) >> 8;
            sa = (sa * cover + 255) >> 8;
        }

        if (sa) {
            calc_type dr = p[Order::R] - sr;
            calc_type dg = p[Order::G] - sg;
            calc_type db = p[Order::B] - sb;
            p[Order::R] = (dr > base_mask) ? 0 : dr;
            p[Order::G] = (dg > base_mask) ? 0 : dg;
            p[Order::B] = (db > base_mask) ? 0 : db;
            p[Order::A] = (value_type)(sa + p[Order::A] - ((sa * p[Order::A] + base_mask) >> base_shift));
        }
    }
};

// composite_op_rgba_multiply
template <typename ColorType, typename Order>
struct composite_op_rgba_multiply
{
    typedef ColorType color_type;
    typedef typename color_type::value_type value_type;
    typedef typename color_type::calc_type calc_type;

    enum { 
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };

    // Dca' = Sca.Dca + Sca.(1 - Da) + Dca.(1 - Sa)
    // Da'  = Sa + Da - Sa.Da 
    static void blend_pix(value_type* p, unsigned int sr, unsigned int sg,
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
            calc_type d1a = base_mask - p[Order::A];
            calc_type dr = p[Order::R];
            calc_type dg = p[Order::G];
            calc_type db = p[Order::B];
            p[Order::R] = (value_type)((sr * dr + sr * d1a + dr * s1a + base_mask) >> base_shift);
            p[Order::G] = (value_type)((sg * dg + sg * d1a + dg * s1a + base_mask) >> base_shift);
            p[Order::B] = (value_type)((sb * db + sb * d1a + db * s1a + base_mask) >> base_shift);
            p[Order::A] = (value_type)(sa + p[Order::A] - ((sa * p[Order::A] + base_mask) >> base_shift));
        }
    }
};

// composite_op_rgba_screen
template <typename ColorType, typename Order>
struct composite_op_rgba_screen
{
    typedef ColorType color_type;
    typedef typename color_type::value_type value_type;
    typedef typename color_type::calc_type calc_type;

    enum {
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };

    // Dca' = Sca + Dca - Sca.Dca
    // Da'  = Sa + Da - Sa.Da 
    static void blend_pix(value_type* p, unsigned int sr, unsigned int sg,
                          unsigned int sb, unsigned int sa, unsigned int cover)
    {
        if (cover < 255) {
            sr = (sr * cover + 255) >> 8;
            sg = (sg * cover + 255) >> 8;
            sb = (sb * cover + 255) >> 8;
            sa = (sa * cover + 255) >> 8;
        }

        if (sa) {
            calc_type dr = p[Order::R];
            calc_type dg = p[Order::G];
            calc_type db = p[Order::B];
            calc_type da = p[Order::A];
            p[Order::R] = (value_type)(sr + dr - ((sr * dr + base_mask) >> base_shift));
            p[Order::G] = (value_type)(sg + dg - ((sg * dg + base_mask) >> base_shift));
            p[Order::B] = (value_type)(sb + db - ((sb * db + base_mask) >> base_shift));
            p[Order::A] = (value_type)(sa + da - ((sa * da + base_mask) >> base_shift));
        }
    }
};

// composite_op_rgba_overlay
template <typename ColorType, typename Order>
struct composite_op_rgba_overlay
{
    typedef ColorType color_type;
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
    static void blend_pix(value_type* p, unsigned int sr, unsigned int sg,
                          unsigned int sb, unsigned int sa, unsigned int cover)
    {
        if (cover < 255) {
            sr = (sr * cover + 255) >> 8;
            sg = (sg * cover + 255) >> 8;
            sb = (sb * cover + 255) >> 8;
            sa = (sa * cover + 255) >> 8;
        }

        if (sa) {
            calc_type d1a  = base_mask - p[Order::A];
            calc_type s1a  = base_mask - sa;
            calc_type dr   = p[Order::R];
            calc_type dg   = p[Order::G];
            calc_type db   = p[Order::B];
            calc_type da   = p[Order::A];
            calc_type sada = sa * p[Order::A];

            p[Order::R] = (value_type)(((2*dr < da) ? 
                2*sr*dr + sr*d1a + dr*s1a : 
                sada - 2*(da - dr)*(sa - sr) + sr*d1a + dr*s1a + base_mask) >> base_shift);

            p[Order::G] = (value_type)(((2*dg < da) ? 
                2*sg*dg + sg*d1a + dg*s1a : 
                sada - 2*(da - dg)*(sa - sg) + sg*d1a + dg*s1a + base_mask) >> base_shift);

            p[Order::B] = (value_type)(((2*db < da) ? 
                2*sb*db + sb*d1a + db*s1a : 
                sada - 2*(da - db)*(sa - sb) + sb*d1a + db*s1a + base_mask) >> base_shift);

            p[Order::A] = (value_type)(sa + da - ((sa * da + base_mask) >> base_shift));
        }
    }
};


// composite_op_rgba_darken
template <typename ColorType, typename Order>
struct composite_op_rgba_darken
{
    typedef ColorType color_type;
    typedef typename color_type::value_type value_type;
    typedef typename color_type::calc_type calc_type;

    enum {
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };

    // Dca' = min(Sca.Da, Dca.Sa) + Sca.(1 - Da) + Dca.(1 - Sa)
    // Da'  = Sa + Da - Sa.Da 
    static void blend_pix(value_type* p, unsigned int sr, unsigned int sg,
                          unsigned int sb, unsigned int sa, unsigned int cover)
    {
        if (cover < 255) {
            sr = (sr * cover + 255) >> 8;
            sg = (sg * cover + 255) >> 8;
            sb = (sb * cover + 255) >> 8;
            sa = (sa * cover + 255) >> 8;
        }

        if (sa) {
            calc_type d1a = base_mask - p[Order::A];
            calc_type s1a = base_mask - sa;
            calc_type dr  = p[Order::R];
            calc_type dg  = p[Order::G];
            calc_type db  = p[Order::B];
            calc_type da  = p[Order::A];

            p[Order::R] = (value_type)((Min(sr * da, dr * sa) + sr * d1a + dr * s1a + base_mask) >> base_shift);
            p[Order::G] = (value_type)((Min(sg * da, dg * sa) + sg * d1a + dg * s1a + base_mask) >> base_shift);
            p[Order::B] = (value_type)((Min(sb * da, db * sa) + sb * d1a + db * s1a + base_mask) >> base_shift);
            p[Order::A] = (value_type)(sa + da - ((sa * da + base_mask) >> base_shift));
        }
    }
};

// composite_op_rgba_lighten
template <typename ColorType, typename Order>
struct composite_op_rgba_lighten
{
    typedef ColorType color_type;
    typedef typename color_type::value_type value_type;
    typedef typename color_type::calc_type calc_type;

    enum {
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };

    // Dca' = max(Sca.Da, Dca.Sa) + Sca.(1 - Da) + Dca.(1 - Sa)
    // Da'  = Sa + Da - Sa.Da 
    static void blend_pix(value_type* p, unsigned int sr, unsigned int sg,
                          unsigned int sb, unsigned int sa, unsigned int cover)
    {
        if (cover < 255) {
            sr = (sr * cover + 255) >> 8;
            sg = (sg * cover + 255) >> 8;
            sb = (sb * cover + 255) >> 8;
            sa = (sa * cover + 255) >> 8;
        }

        if (sa) {
            calc_type d1a = base_mask - p[Order::A];
            calc_type s1a = base_mask - sa;
            calc_type dr  = p[Order::R];
            calc_type dg  = p[Order::G];
            calc_type db  = p[Order::B];
            calc_type da  = p[Order::A];

            p[Order::R] = (value_type)((Max(sr * da, dr * sa) + sr * d1a + dr * s1a + base_mask) >> base_shift);
            p[Order::G] = (value_type)((Max(sg * da, dg * sa) + sg * d1a + dg * s1a + base_mask) >> base_shift);
            p[Order::B] = (value_type)((Max(sb * da, db * sa) + sb * d1a + db * s1a + base_mask) >> base_shift);
            p[Order::A] = (value_type)(sa + da - ((sa * da + base_mask) >> base_shift));
        }
    }
};

// composite_op_rgba_color_dodge
template <typename ColorType, typename Order>
struct composite_op_rgba_color_dodge
{
    typedef ColorType color_type;
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
    static void blend_pix(value_type* p, unsigned int sr, unsigned int sg,
                          unsigned int sb, unsigned int sa, unsigned int cover)
    {
        if (cover < 255) {
            sr = (sr * cover + 255) >> 8;
            sg = (sg * cover + 255) >> 8;
            sb = (sb * cover + 255) >> 8;
            sa = (sa * cover + 255) >> 8;
        }

        if (sa) {
            calc_type d1a  = base_mask - p[Order::A];
            calc_type s1a  = base_mask - sa;
            calc_type dr   = p[Order::R];
            calc_type dg   = p[Order::G];
            calc_type db   = p[Order::B];
            calc_type da   = p[Order::A];
            long_type drsa = dr * sa;
            long_type dgsa = dg * sa;
            long_type dbsa = db * sa;
            long_type srda = sr * da;
            long_type sgda = sg * da;
            long_type sbda = sb * da;
            long_type sada = sa * da;

            p[Order::R] = (value_type)((srda + drsa >= sada) ? 
                (sada + sr * d1a + dr * s1a + base_mask) >> base_shift :
                drsa / (base_mask - (sr << base_shift) / sa) + ((sr * d1a + dr * s1a + base_mask) >> base_shift));

            p[Order::G] = (value_type)((sgda + dgsa >= sada) ? 
                (sada + sg * d1a + dg * s1a + base_mask) >> base_shift :
                dgsa / (base_mask - (sg << base_shift) / sa) + ((sg * d1a + dg * s1a + base_mask) >> base_shift));

            p[Order::B] = (value_type)((sbda + dbsa >= sada) ? 
                (sada + sb * d1a + db * s1a + base_mask) >> base_shift :
                dbsa / (base_mask - (sb << base_shift) / sa) + ((sb * d1a + db * s1a + base_mask) >> base_shift));

            p[Order::A] = (value_type)(sa + da - ((sa * da + base_mask) >> base_shift));
        }
    }
};

// composite_op_rgba_color_burn
template <typename ColorType, typename Order>
struct composite_op_rgba_color_burn
{
    typedef ColorType color_type;
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
    static void blend_pix(value_type* p, unsigned int sr, unsigned int sg,
                          unsigned int sb, unsigned int sa, unsigned int cover)
    {
        if (cover < 255) {
            sr = (sr * cover + 255) >> 8;
            sg = (sg * cover + 255) >> 8;
            sb = (sb * cover + 255) >> 8;
            sa = (sa * cover + 255) >> 8;
        }

        if (sa) {
            calc_type d1a  = base_mask - p[Order::A];
            calc_type s1a  = base_mask - sa;
            calc_type dr   = p[Order::R];
            calc_type dg   = p[Order::G];
            calc_type db   = p[Order::B];
            calc_type da   = p[Order::A];
            long_type drsa = dr * sa;
            long_type dgsa = dg * sa;
            long_type dbsa = db * sa;
            long_type srda = sr * da;
            long_type sgda = sg * da;
            long_type sbda = sb * da;
            long_type sada = sa * da;

            p[Order::R] = (value_type)(((srda + drsa <= sada) ? 
                sr * d1a + dr * s1a :
                sa * (srda + drsa - sada) / sr + sr * d1a + dr * s1a + base_mask) >> base_shift);

            p[Order::G] = (value_type)(((sgda + dgsa <= sada) ? 
                sg * d1a + dg * s1a :
                sa * (sgda + dgsa - sada) / sg + sg * d1a + dg * s1a + base_mask) >> base_shift);

            p[Order::B] = (value_type)(((sbda + dbsa <= sada) ? 
                sb * d1a + db * s1a :
                sa * (sbda + dbsa - sada) / sb + sb * d1a + db * s1a + base_mask) >> base_shift);

            p[Order::A] = (value_type)(sa + da - ((sa * da + base_mask) >> base_shift));
        }
    }
};

// composite_op_rgba_hard_light
template <typename ColorType, typename Order>
struct composite_op_rgba_hard_light
{
    typedef ColorType color_type;
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
    static void blend_pix(value_type* p, unsigned int sr, unsigned int sg,
                          unsigned int sb, unsigned int sa, unsigned int cover)
    {
        if (cover < 255) {
            sr = (sr * cover + 255) >> 8;
            sg = (sg * cover + 255) >> 8;
            sb = (sb * cover + 255) >> 8;
            sa = (sa * cover + 255) >> 8;
        }

        if (sa) {
            calc_type d1a  = base_mask - p[Order::A];
            calc_type s1a  = base_mask - sa;
            calc_type dr   = p[Order::R];
            calc_type dg   = p[Order::G];
            calc_type db   = p[Order::B];
            calc_type da   = p[Order::A];
            calc_type sada = sa * da;

            p[Order::R] = (value_type)(((2*sr < sa) ? 
                2*sr*dr + sr*d1a + dr*s1a : 
                sada - 2*(da - dr)*(sa - sr) + sr*d1a + dr*s1a + base_mask) >> base_shift);

            p[Order::G] = (value_type)(((2*sg < sa) ? 
                2*sg*dg + sg*d1a + dg*s1a : 
                sada - 2*(da - dg)*(sa - sg) + sg*d1a + dg*s1a + base_mask) >> base_shift);

            p[Order::B] = (value_type)(((2*sb < sa) ? 
                2*sb*db + sb*d1a + db*s1a : 
                sada - 2*(da - db)*(sa - sb) + sb*d1a + db*s1a + base_mask) >> base_shift);

            p[Order::A] = (value_type)(sa + da - ((sa * da + base_mask) >> base_shift));
        }
    }
};

// composite_op_rgba_soft_light
template <typename ColorType, typename Order>
struct composite_op_rgba_soft_light
{
    typedef ColorType color_type;
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

    static void blend_pix(value_type* p, unsigned int r, unsigned int g,
                          unsigned int b, unsigned int a, unsigned int cover)
    {
        scalar sr = INT_TO_SCALAR(r * cover) / (base_mask * 255);
        scalar sg = INT_TO_SCALAR(g * cover) / (base_mask * 255);
        scalar sb = INT_TO_SCALAR(b * cover) / (base_mask * 255);
        scalar sa = INT_TO_SCALAR(a * cover) / (base_mask * 255);
        if (sa > 0) {
            scalar dr = INT_TO_SCALAR(p[Order::R]) / base_mask;
            scalar dg = INT_TO_SCALAR(p[Order::G]) / base_mask;
            scalar db = INT_TO_SCALAR(p[Order::B]) / base_mask;
            scalar da = INT_TO_SCALAR(p[Order::A] ? p[Order::A] : 1) / base_mask;

            if (cover < 255) {
                a = (a * cover + 255) >> 8;
            }

            if (2*sr < sa)       dr = dr*(sa + (1 - dr/da)*(2*sr - sa)) + sr*(1 - da) + dr*(1 - sa);
            else if (8*dr <= da) dr = dr*(sa + (1 - dr/da)*(2*sr - sa)*(3 - 8*dr/da)) + sr*(1 - da) + dr*(1 - sa);
            else                 dr = (dr*sa + (Sqrt(dr/da)*da - dr)*(2*sr - sa)) + sr*(1 - da) + dr*(1 - sa);

            if (2*sg < sa)       dg = dg*(sa + (1 - dg/da)*(2*sg - sa)) + sg*(1 - da) + dg*(1 - sa);
            else if (8*dg <= da) dg = dg*(sa + (1 - dg/da)*(2*sg - sa)*(3 - 8*dg/da)) + sg*(1 - da) + dg*(1 - sa);
            else                 dg = (dg*sa + (Sqrt(dg/da)*da - dg)*(2*sg - sa)) + sg*(1 - da) + dg*(1 - sa);

            if (2*sb < sa)       db = db*(sa + (1 - db/da)*(2*sb - sa)) + sb*(1 - da) + db*(1 - sa);
            else if (8*db <= da) db = db*(sa + (1 - db/da)*(2*sb - sa)*(3 - 8*db/da)) + sb*(1 - da) + db*(1 - sa);
            else                 db = (db*sa + (Sqrt(db/da)*da - db)*(2*sb - sa)) + sb*(1 - da) + db*(1 - sa);

            p[Order::R] = (value_type)uround(dr * base_mask);
            p[Order::G] = (value_type)uround(dg * base_mask);
            p[Order::B] = (value_type)uround(db * base_mask);
            p[Order::A] = (value_type)(a + p[Order::A] - ((a * p[Order::A] + base_mask) >> base_shift));
        }
    }
};

// composite_op_rgba_difference
template <typename ColorType, typename Order>
struct composite_op_rgba_difference
{
    typedef ColorType color_type;
    typedef typename color_type::value_type value_type;
    typedef typename color_type::calc_type calc_type;
    typedef typename color_type::long_type long_type;

    enum { 
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };

    // Dca' = Sca + Dca - 2.min(Sca.Da, Dca.Sa)
    // Da'  = Sa + Da - Sa.Da 
    static void blend_pix(value_type* p, unsigned int sr, unsigned int sg,
                          unsigned int sb, unsigned int sa, unsigned int cover)
    {
        if (cover < 255) {
            sr = (sr * cover + 255) >> 8;
            sg = (sg * cover + 255) >> 8;
            sb = (sb * cover + 255) >> 8;
            sa = (sa * cover + 255) >> 8;
        }

        if (sa) {
            calc_type dr = p[Order::R];
            calc_type dg = p[Order::G];
            calc_type db = p[Order::B];
            calc_type da = p[Order::A];
            p[Order::R] = (value_type)(sr + dr - ((2 * Min(sr*da, dr*sa) + base_mask) >> base_shift));
            p[Order::G] = (value_type)(sg + dg - ((2 * Min(sg*da, dg*sa) + base_mask) >> base_shift));
            p[Order::B] = (value_type)(sb + db - ((2 * Min(sb*da, db*sa) + base_mask) >> base_shift));
            p[Order::A] = (value_type)(sa + da - ((sa * da + base_mask) >> base_shift));
        }
    }
};

// composite_op_rgba_exclusion
template <typename ColorType, typename Order>
struct composite_op_rgba_exclusion
{
    typedef ColorType color_type;
    typedef typename color_type::value_type value_type;
    typedef typename color_type::calc_type calc_type;
    typedef typename color_type::long_type long_type;

    enum { 
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };

    // Dca' = (Sca.Da + Dca.Sa - 2.Sca.Dca) + Sca.(1 - Da) + Dca.(1 - Sa)
    // Da'  = Sa + Da - Sa.Da 
    static void blend_pix(value_type* p, unsigned int sr, unsigned int sg,
                          unsigned int sb, unsigned int sa, unsigned int cover)
    {
        if (cover < 255) {
            sr = (sr * cover + 255) >> 8;
            sg = (sg * cover + 255) >> 8;
            sb = (sb * cover + 255) >> 8;
            sa = (sa * cover + 255) >> 8;
        }

        if (sa) {
            calc_type d1a = base_mask - p[Order::A];
            calc_type s1a = base_mask - sa;
            calc_type dr = p[Order::R];
            calc_type dg = p[Order::G];
            calc_type db = p[Order::B];
            calc_type da = p[Order::A];
            p[Order::R] = (value_type)((sr*da + dr*sa - 2*sr*dr + sr*d1a + dr*s1a + base_mask) >> base_shift);
            p[Order::G] = (value_type)((sg*da + dg*sa - 2*sg*dg + sg*d1a + dg*s1a + base_mask) >> base_shift);
            p[Order::B] = (value_type)((sb*da + db*sa - 2*sb*db + sb*d1a + db*s1a + base_mask) >> base_shift);
            p[Order::A] = (value_type)(sa + da - ((sa * da + base_mask) >> base_shift));
        }
    }
};

// composite_op_rgba_contrast
template <typename ColorType, typename Order>
struct composite_op_rgba_contrast
{
    typedef ColorType color_type;
    typedef typename color_type::value_type value_type;
    typedef typename color_type::calc_type calc_type;
    typedef typename color_type::long_type long_type;

    enum { 
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };


    static void blend_pix(value_type* p, unsigned int sr, unsigned int sg,
                          unsigned int sb, unsigned int sa, unsigned int cover)
    {
        if (cover < 255) {
            sr = (sr * cover + 255) >> 8;
            sg = (sg * cover + 255) >> 8;
            sb = (sb * cover + 255) >> 8;
            sa = (sa * cover + 255) >> 8;
        }

        long_type dr = p[Order::R];
        long_type dg = p[Order::G];
        long_type db = p[Order::B];
        int       da = p[Order::A];
        long_type d2a = da >> 1;
        unsigned int s2a = sa >> 1;

        int r = (int)((((dr - d2a) * int((sr - s2a)*2 + base_mask)) >> base_shift) + d2a); 
        int g = (int)((((dg - d2a) * int((sg - s2a)*2 + base_mask)) >> base_shift) + d2a); 
        int b = (int)((((db - d2a) * int((sb - s2a)*2 + base_mask)) >> base_shift) + d2a); 

        r = (r < 0) ? 0 : r;
        g = (g < 0) ? 0 : g;
        b = (b < 0) ? 0 : b;

        p[Order::R] = (value_type)((r > da) ? da : r);
        p[Order::G] = (value_type)((g > da) ? da : g);
        p[Order::B] = (value_type)((b > da) ? da : b);
    }
};

// composite_op_rgba_invert
template <typename ColorType, typename Order>
struct composite_op_rgba_invert
{
    typedef ColorType color_type;
    typedef typename color_type::value_type value_type;
    typedef typename color_type::calc_type calc_type;
    typedef typename color_type::long_type long_type;

    enum { 
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };

    // Dca' = (Da - Dca) * Sa + Dca.(1 - Sa)
    // Da'  = Sa + Da - Sa.Da 
    static void blend_pix(value_type* p, unsigned int sr, unsigned int sg,
                          unsigned int sb, unsigned int sa, unsigned int cover)
    {
        sa = (sa * cover + 255) >> 8;
        if (sa) {
            calc_type da = p[Order::A];
            calc_type dr = ((da - p[Order::R]) * sa + base_mask) >> base_shift;
            calc_type dg = ((da - p[Order::G]) * sa + base_mask) >> base_shift;
            calc_type db = ((da - p[Order::B]) * sa + base_mask) >> base_shift;
            calc_type s1a = base_mask - sa;
            p[Order::R] = (value_type)(dr + ((p[Order::R] * s1a + base_mask) >> base_shift));
            p[Order::G] = (value_type)(dg + ((p[Order::G] * s1a + base_mask) >> base_shift));
            p[Order::B] = (value_type)(db + ((p[Order::B] * s1a + base_mask) >> base_shift));
            p[Order::A] = (value_type)(sa + da - ((sa * da + base_mask) >> base_shift));
        }
    }
};

// composite_op_rgba_invert_rgb
template <typename ColorType, typename Order>
struct composite_op_rgba_invert_rgb
{
    typedef ColorType color_type;
    typedef typename color_type::value_type value_type;
    typedef typename color_type::calc_type calc_type;
    typedef typename color_type::long_type long_type;

    enum { 
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };

    // Dca' = (Da - Dca) * Sca + Dca.(1 - Sa)
    // Da'  = Sa + Da - Sa.Da 
    static void blend_pix(value_type* p, unsigned int sr, unsigned int sg,
                          unsigned int sb, unsigned int sa, unsigned int cover)
    {
        if (cover < 255) {
            sr = (sr * cover + 255) >> 8;
            sg = (sg * cover + 255) >> 8;
            sb = (sb * cover + 255) >> 8;
            sa = (sa * cover + 255) >> 8;
        }

        if (sa) {
            calc_type da = p[Order::A];
            calc_type dr = ((da - p[Order::R]) * sr + base_mask) >> base_shift;
            calc_type dg = ((da - p[Order::G]) * sg + base_mask) >> base_shift;
            calc_type db = ((da - p[Order::B]) * sb + base_mask) >> base_shift;
            calc_type s1a = base_mask - sa;
            p[Order::R] = (value_type)(dr + ((p[Order::R] * s1a + base_mask) >> base_shift));
            p[Order::G] = (value_type)(dg + ((p[Order::G] * s1a + base_mask) >> base_shift));
            p[Order::B] = (value_type)(db + ((p[Order::B] * s1a + base_mask) >> base_shift));
            p[Order::A] = (value_type)(sa + da - ((sa * da + base_mask) >> base_shift));
        }
    }
};


// composite operate table for blend rgba pixel format.
template <typename ColorType, typename Order>
struct blend_op_table_rgba
{
    typedef typename ColorType::value_type value_type;
    typedef void (*composite_op_func_type)(value_type* p, 
                                      unsigned int cr, 
                                      unsigned int cg, 
                                      unsigned int cb,
                                      unsigned int ca,
                                      unsigned int cover);

    static composite_op_func_type g_rgba_blend_op_func[];
};

// g_rgba_comp_op_func
template <typename ColorType, typename Order> 
typename blend_op_table_rgba<ColorType, Order>::composite_op_func_type
blend_op_table_rgba<ColorType, Order>::g_rgba_blend_op_func[] = 
{
    composite_op_rgba_clear      <ColorType,Order>::blend_pix,
    composite_op_rgba_src        <ColorType,Order>::blend_pix,
    composite_op_rgba_src_over   <ColorType,Order>::blend_pix,
    composite_op_rgba_src_in     <ColorType,Order>::blend_pix,
    composite_op_rgba_src_out    <ColorType,Order>::blend_pix,
    composite_op_rgba_src_atop   <ColorType,Order>::blend_pix,
    composite_op_rgba_dst        <ColorType,Order>::blend_pix,
    composite_op_rgba_dst_over   <ColorType,Order>::blend_pix,
    composite_op_rgba_dst_in     <ColorType,Order>::blend_pix,
    composite_op_rgba_dst_out    <ColorType,Order>::blend_pix,
    composite_op_rgba_dst_atop   <ColorType,Order>::blend_pix,
    composite_op_rgba_xor        <ColorType,Order>::blend_pix,
    composite_op_rgba_darken     <ColorType,Order>::blend_pix,
    composite_op_rgba_lighten    <ColorType,Order>::blend_pix,
    composite_op_rgba_overlay    <ColorType,Order>::blend_pix,
    composite_op_rgba_screen     <ColorType,Order>::blend_pix,
    composite_op_rgba_multiply   <ColorType,Order>::blend_pix,
    composite_op_rgba_plus       <ColorType,Order>::blend_pix,
    composite_op_rgba_minus      <ColorType,Order>::blend_pix,
    composite_op_rgba_exclusion  <ColorType,Order>::blend_pix,
    composite_op_rgba_difference <ColorType,Order>::blend_pix,
    composite_op_rgba_soft_light <ColorType,Order>::blend_pix,
    composite_op_rgba_hard_light <ColorType,Order>::blend_pix,
    composite_op_rgba_color_burn <ColorType,Order>::blend_pix,
    composite_op_rgba_color_dodge<ColorType,Order>::blend_pix,
    composite_op_rgba_contrast   <ColorType,Order>::blend_pix,
    composite_op_rgba_invert     <ColorType,Order>::blend_pix,
    composite_op_rgba_invert_rgb <ColorType,Order>::blend_pix,
    0
};

// blend operate adaptor for rgba 
template <typename ColorType, typename Order>
class blend_op_adaptor_rgba
{
public:
    typedef ColorType color_type;
    typedef typename color_type::value_type value_type;
    typedef Order order_type;

    enum {  
        base_shift = color_type::base_shift,
        base_mask  = color_type::base_mask,
    };

    static void blend_pix(unsigned int op, value_type* p, 
                          unsigned int cr, unsigned int cg,
                          unsigned int cb, unsigned int ca, unsigned int cover)
    {
        blend_op_table_rgba<ColorType, Order>::g_rgba_blend_op_func[op]
            (p, (cr * ca + base_mask) >> base_shift,
                (cg * ca + base_mask) >> base_shift,
                (cb * ca + base_mask) >> base_shift,
                 ca, cover);
    }
};


// pixfmt blender rgba
template <typename Blender, typename RenBuffer> 
class pixfmt_blender_rgba
{
public:
    typedef RenBuffer buffer_type;
    typedef typename buffer_type::row_data row_data;
    typedef uint32_t pixel_type;
    typedef Blender blender_type;
    typedef typename blender_type::color_type color_type;
    typedef typename blender_type::order_type order_type;
    typedef typename color_type::value_type value_type;

    enum {
        base_shift = color_type::base_shift,
        base_scale = color_type::base_scale,
        base_mask  = color_type::base_mask,
        pix_width  = sizeof(pixel_type)
    };

    pixfmt_blender_rgba()
        : m_buffer(0)
        , m_blend_op(comp_op_src_over)
        , m_alpha_factor(base_mask) 
    {
    }

    explicit pixfmt_blender_rgba(buffer_type& rb, unsigned int op = comp_op_src_over, unsigned int alpha = base_mask) 
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
        const value_type* p = (value_type*)m_buffer->row_ptr(y) + (x << 2);
        return color_type(p[order_type::R], 
                          p[order_type::G], 
                          p[order_type::B], 
                          p[order_type::A]);
    }

    void copy_pixel(int x, int y, const color_type& c)
    {
        value_type* p = (value_type*)m_buffer->row_ptr(x, y, 1) + (x << 2);
        p[order_type::R] = c.r;
        p[order_type::G] = c.g;
        p[order_type::B] = c.b;
        p[order_type::A] = c.a;
    }

    void blend_pixel(int x, int y, const color_type& c, uint8_t cover)
    {
        blender_type::blend_pix(m_blend_op, 
            (value_type*)m_buffer->row_ptr(x, y, 1) + (x << 2),
             c.r, c.g, c.b, (value_type)alpha_mul(c.a, m_alpha_factor), cover);
    }

    void copy_hline(int x, int y, unsigned int len, const color_type& c)
    {
        pixel_type v;
        ((value_type*)&v)[order_type::R] = c.r;
        ((value_type*)&v)[order_type::G] = c.g;
        ((value_type*)&v)[order_type::B] = c.b;
        ((value_type*)&v)[order_type::A] = c.a;

        value_type* p = (value_type*)m_buffer->row_ptr(x, y, len) + (x << 2);
        do {
            *(pixel_type*)p = v;
            p += 4;
        } while(--len);
    }

    void copy_vline(int x, int y, unsigned int len, const color_type& c)
    {
        pixel_type v;
        ((value_type*)&v)[order_type::R] = c.r;
        ((value_type*)&v)[order_type::G] = c.g;
        ((value_type*)&v)[order_type::B] = c.b;
        ((value_type*)&v)[order_type::A] = c.a;

        do {
            value_type* p = (value_type*)m_buffer->row_ptr(x, y++, 1) + (x << 2);
            *(pixel_type*)p = v;
        } while(--len);
    }

    void blend_hline(int x, int y, unsigned int len, const color_type& c, uint8_t cover)
    {

        value_type* p = (value_type*)m_buffer->row_ptr(x, y, len) + (x << 2);
        do {
            blender_type::blend_pix(m_blend_op, p, c.r, c.g, c.b, (value_type)alpha_mul(c.a, m_alpha_factor), cover);
            p += 4;
        } while(--len);
    }

    void blend_vline(int x, int y, unsigned int len, const color_type& c, uint8_t cover)
    {
        do {
            blender_type::blend_pix(m_blend_op,
                    (value_type*)m_buffer->row_ptr(x, y++, 1) + (x << 2), 
                     c.r, c.g, c.b, (value_type)alpha_mul(c.a, m_alpha_factor), cover);
        } while(--len);
    }

    void blend_solid_hspan(int x, int y, unsigned int len, const color_type& c, const uint8_t* covers)
    {
        value_type* p = (value_type*)m_buffer->row_ptr(x, y, len) + (x << 2);
        do {
            blender_type::blend_pix(m_blend_op, 
                          p, c.r, c.g, c.b, (value_type)alpha_mul(c.a, m_alpha_factor), *covers++);
            p += 4;
        } while(--len);
    }

    void blend_solid_vspan(int x, int y, unsigned int len, const color_type& c, const uint8_t* covers)
    {
        do {
            blender_type::blend_pix(m_blend_op, 
                    (value_type*)m_buffer->row_ptr(x, y++, 1) + (x << 2), 
                     c.r, c.g, c.b, (value_type)alpha_mul(c.a, m_alpha_factor), *covers++);
        } while(--len);
    }

    void copy_color_hspan(int x, int y, unsigned int len, const color_type* colors)
    {
        value_type* p = (value_type*)m_buffer->row_ptr(x, y, len) + (x << 2);
        do {
            p[order_type::R] = colors->r;
            p[order_type::G] = colors->g;
            p[order_type::B] = colors->b;
            p[order_type::A] = colors->a;
            ++colors;
            p += 4;
        } while(--len);
    }

    void copy_color_vspan(int x, int y, unsigned int len, const color_type* colors)
    {
        do {
            value_type* p = (value_type*)m_buffer->row_ptr(x, y++, 1) + (x << 2);
            p[order_type::R] = colors->r;
            p[order_type::G] = colors->g;
            p[order_type::B] = colors->b;
            p[order_type::A] = colors->a;
            ++colors;
        } while(--len);
    }

    void blend_color_hspan(int x, int y, unsigned int len,
                           const color_type* colors, const uint8_t* covers, uint8_t cover)
    {
        value_type* p = (value_type*)m_buffer->row_ptr(x, y, len) + (x << 2);
        do {
            blender_type::blend_pix(m_blend_op, p, 
                    colors->r, 
                    colors->g, 
                    colors->b, 
                    (value_type)alpha_mul(colors->a, m_alpha_factor), 
                    covers ? *covers++ : cover);
            p += 4;
            ++colors;
        } while(--len);
    }

    void blend_color_vspan(int x, int y, unsigned int len,
                           const color_type* colors, const uint8_t* covers, uint8_t cover)
    {
        do {
            blender_type::blend_pix(m_blend_op,
                (value_type*)m_buffer->row_ptr(x, y++, 1) + (x << 2),
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
        ((value_type*)p)[order_type::R] = c.r;
        ((value_type*)p)[order_type::G] = c.g;
        ((value_type*)p)[order_type::B] = c.b;
        ((value_type*)p)[order_type::A] = c.a;
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
            value_type* pdst = (value_type*)m_buffer->row_ptr(xdst, ydst, len) + (xdst << 2);

            int incp = 4;
            if (xdst > xsrc) {
                psrc += (len-1) << 2;
                pdst += (len-1) << 2;
                incp = -4;
            }

            do {
                blender_type::blend_pix(m_blend_op, pdst, 
                                        psrc[src_order::R],
                                        psrc[src_order::G],
                                        psrc[src_order::B],
                                        (value_type)alpha_mul(psrc[src_order::A], m_alpha_factor), cover);
                psrc += incp;
                pdst += incp;
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
            value_type* pdst = (value_type*)m_buffer->row_ptr(xdst, ydst, 1) + (xdst << 2);

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
            value_type* pdst = (value_type*)m_buffer->row_ptr(xdst, ydst, len) + (xdst << 2);

            do {
                blender_type::blend_pix(m_blend_op, pdst,
                                        color.r, color.g, color.b, alpha_mul(color.a, m_alpha_factor),
                                        (*psrc * cover + base_mask) >> base_shift);
                ++psrc;
                pdst += 4;
            } while(--len);
        }
    }

    template <typename SrcPixelFormatRenderer>
    void blend_from_lut(const SrcPixelFormatRenderer& from, const color_type* color_lut,
                        int xdst, int ydst, int xsrc, int ysrc, unsigned int len, uint8_t cover)
    {
        typedef typename SrcPixelFormatRenderer::value_type src_value_type;
        const src_value_type* psrc = (src_value_type*)from.row_ptr(ysrc);
        if (psrc) {
            value_type* pdst = (value_type*)m_buffer->row_ptr(xdst, ydst, len) + (xdst << 2);

            do {
                const color_type& color = color_lut[*psrc];
                blender_type::blend_pix(m_blend_op, pdst,
                                        color.r, color.g, color.b, alpha_mul(color.a, m_alpha_factor), cover);
                ++psrc;
                pdst += 4;
            } while(--len);
        }
    }

private:
    unsigned int alpha_mul(unsigned int a, unsigned int s)
    {
        return (s == 255) ? a : ((a * s + base_mask) >> base_shift);
    }

    pixfmt_blender_rgba(const pixfmt_blender_rgba&);
    pixfmt_blender_rgba& operator=(const pixfmt_blender_rgba&);
private:
    buffer_type* m_buffer;
    unsigned int m_blend_op;
    unsigned int m_alpha_factor;
};


typedef blend_op_adaptor_rgba<rgba8, order_rgba> blender_rgba32; // blender_rgba32
typedef blend_op_adaptor_rgba<rgba8, order_argb> blender_argb32; // blender_argb32
typedef blend_op_adaptor_rgba<rgba8, order_abgr> blender_abgr32; // blender_abgr32
typedef blend_op_adaptor_rgba<rgba8, order_bgra> blender_bgra32; // blender_bgra32

typedef pixfmt_blender_rgba<blender_rgba32, gfx_rendering_buffer> pixfmt_rgba32; // pixfmt_rgba32
typedef pixfmt_blender_rgba<blender_argb32, gfx_rendering_buffer> pixfmt_argb32; // pixfmt_argb32
typedef pixfmt_blender_rgba<blender_abgr32, gfx_rendering_buffer> pixfmt_abgr32; // pixfmt_abgr32
typedef pixfmt_blender_rgba<blender_bgra32, gfx_rendering_buffer> pixfmt_bgra32; // pixfmt_bgra32

}
#endif /*_GFX_PIXFMT_RGBA_H_*/
