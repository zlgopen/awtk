#include <math.h>
#include <stdio.h>
#include "agg_basics.h"
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_conv_curve.h"
#include "agg_conv_contour.h"
#include "agg_conv_stroke.h"
#include "agg_scanline_p.h"
#include "agg_renderer_scanline.h"
#include "agg_pixfmt_rgb.h"
#include "agg_pixfmt_rgba.h"
#include "agg_pixfmt_gray.h"
#include "agg_bounding_rect.h"
#include "agg_trans_perspective.h"
#include "agg_blur.h"
#include "ctrl/agg_slider_ctrl.h"
#include "ctrl/agg_rbox_ctrl.h"
#include "ctrl/agg_cbox_ctrl.h"
#include "ctrl/agg_polygon_ctrl.h"
#include "platform/agg_platform_support.h"

//#define AGG_GRAY8
//#define AGG_GRAY16
//#define AGG_GRAY32
#define AGG_BGR24
//#define AGG_RGB24
//#define AGG_BGR48 
//#define AGG_RGB48
//#define AGG_BGR96
//#define AGG_RGB96
//#define AGG_BGRA32
//#define AGG_RGBA32
//#define AGG_ARGB32
//#define AGG_ABGR32
//#define AGG_BGRA64
//#define AGG_RGBA64 
//#define AGG_ARGB64 
//#define AGG_ABGR64
//#define AGG_BGRA128
//#define AGG_RGBA128
//#define AGG_ARGB128
//#define AGG_ABGR128
//#define AGG_RGB565
//#define AGG_RGB555
#include "pixel_formats.h"


static agg::int8u g_gradient_colors[] = 
{
    255, 255, 255, 255,
    255, 255, 254, 255,
    255, 255, 254, 255,
    255, 255, 254, 255,
    255, 255, 253, 255,
    255, 255, 253, 255,
    255, 255, 252, 255,
    255, 255, 251, 255,
    255, 255, 250, 255,
    255, 255, 248, 255,
    255, 255, 246, 255,
    255, 255, 244, 255,
    255, 255, 241, 255,
    255, 255, 238, 255,
    255, 255, 235, 255,
    255, 255, 231, 255,
    255, 255, 227, 255,
    255, 255, 222, 255,
    255, 255, 217, 255,
    255, 255, 211, 255,
    255, 255, 206, 255,
    255, 255, 200, 255,
    255, 254, 194, 255,
    255, 253, 188, 255,
    255, 252, 182, 255,
    255, 250, 176, 255,
    255, 249, 170, 255,
    255, 247, 164, 255,
    255, 246, 158, 255,
    255, 244, 152, 255,
    254, 242, 146, 255,
    254, 240, 141, 255,
    254, 238, 136, 255,
    254, 236, 131, 255,
    253, 234, 126, 255,
    253, 232, 121, 255,
    253, 229, 116, 255,
    252, 227, 112, 255,
    252, 224, 108, 255,
    251, 222, 104, 255,
    251, 219, 100, 255,
    251, 216,  96, 255,
    250, 214,  93, 255,
    250, 211,  89, 255,
    249, 208,  86, 255,
    249, 205,  83, 255,
    248, 202,  80, 255,
    247, 199,  77, 255,
    247, 196,  74, 255,
    246, 193,  72, 255,
    246, 190,  69, 255,
    245, 187,  67, 255,
    244, 183,  64, 255,
    244, 180,  62, 255,
    243, 177,  60, 255,
    242, 174,  58, 255,
    242, 170,  56, 255,
    241, 167,  54, 255,
    240, 164,  52, 255,
    239, 161,  51, 255,
    239, 157,  49, 255,
    238, 154,  47, 255,
    237, 151,  46, 255,
    236, 147,  44, 255,
    235, 144,  43, 255,
    235, 141,  41, 255,
    234, 138,  40, 255,
    233, 134,  39, 255,
    232, 131,  37, 255,
    231, 128,  36, 255,
    230, 125,  35, 255,
    229, 122,  34, 255,
    228, 119,  33, 255,
    227, 116,  31, 255,
    226, 113,  30, 255,
    225, 110,  29, 255,
    224, 107,  28, 255,
    223, 104,  27, 255,
    222, 101,  26, 255,
    221,  99,  25, 255,
    220,  96,  24, 255,
    219,  93,  23, 255,
    218,  91,  22, 255,
    217,  88,  21, 255,
    216,  86,  20, 255,
    215,  83,  19, 255,
    214,  81,  18, 255,
    213,  79,  17, 255,
    212,  77,  17, 255,
    211,  74,  16, 255,
    210,  72,  15, 255,
    209,  70,  14, 255,
    207,  68,  13, 255,
    206,  66,  13, 255,
    205,  64,  12, 255,
    204,  62,  11, 255,
    203,  60,  10, 255,
    202,  58,  10, 255,
    201,  56,   9, 255,
    199,  55,   9, 255,
    198,  53,   8, 255,
    197,  51,   7, 255,
    196,  50,   7, 255,
    195,  48,   6, 255,
    193,  46,   6, 255,
    192,  45,   5, 255,
    191,  43,   5, 255,
    190,  42,   4, 255,
    188,  41,   4, 255,
    187,  39,   3, 255,
    186,  38,   3, 255,
    185,  37,   2, 255,
    183,  35,   2, 255,
    182,  34,   1, 255,
    181,  33,   1, 255,
    179,  32,   1, 255,
    178,  30,   0, 255,
    177,  29,   0, 255,
    175,  28,   0, 255,
    174,  27,   0, 255,
    173,  26,   0, 255,
    171,  25,   0, 255,
    170,  24,   0, 255,
    168,  23,   0, 255,
    167,  22,   0, 255,
    165,  21,   0, 255,
    164,  21,   0, 255,
    163,  20,   0, 255,
    161,  19,   0, 255,
    160,  18,   0, 255,
    158,  17,   0, 255,
    156,  17,   0, 255,
    155,  16,   0, 255,
    153,  15,   0, 255,
    152,  14,   0, 255,
    150,  14,   0, 255,
    149,  13,   0, 255,
    147,  12,   0, 255,
    145,  12,   0, 255,
    144,  11,   0, 255,
    142,  11,   0, 255,
    140,  10,   0, 255,
    139,  10,   0, 255,
    137,   9,   0, 255,
    135,   9,   0, 255,
    134,   8,   0, 255,
    132,   8,   0, 255,
    130,   7,   0, 255,
    128,   7,   0, 255,
    126,   6,   0, 255,
    125,   6,   0, 255,
    123,   5,   0, 255,
    121,   5,   0, 255,
    119,   4,   0, 255,
    117,   4,   0, 255,
    115,   4,   0, 255,
    113,   3,   0, 255,
    111,   3,   0, 255,
    109,   2,   0, 255,
    107,   2,   0, 255,
    105,   2,   0, 255,
    103,   1,   0, 255,
    101,   1,   0, 255,
     99,   1,   0, 255,
     97,   0,   0, 255,
     95,   0,   0, 255,
     93,   0,   0, 255,
     91,   0,   0, 255,
     90,   0,   0, 255,
     88,   0,   0, 255,
     86,   0,   0, 255,
     84,   0,   0, 255,
     82,   0,   0, 255,
     80,   0,   0, 255,
     78,   0,   0, 255,
     77,   0,   0, 255,
     75,   0,   0, 255,
     73,   0,   0, 255,
     72,   0,   0, 255,
     70,   0,   0, 255,
     68,   0,   0, 255,
     67,   0,   0, 255,
     65,   0,   0, 255,
     64,   0,   0, 255,
     63,   0,   0, 255,
     61,   0,   0, 255,
     60,   0,   0, 255,
     59,   0,   0, 255,
     58,   0,   0, 255,
     57,   0,   0, 255,
     56,   0,   0, 255,
     55,   0,   0, 255,
     54,   0,   0, 255,
     53,   0,   0, 255,
     53,   0,   0, 255,
     52,   0,   0, 255,
     52,   0,   0, 255,
     51,   0,   0, 255,
     51,   0,   0, 255,
     51,   0,   0, 255,
     50,   0,   0, 255,
     50,   0,   0, 255,
     51,   0,   0, 255,
     51,   0,   0, 255,
     51,   0,   0, 255,
     51,   0,   0, 255,
     52,   0,   0, 255,
     52,   0,   0, 255,
     53,   0,   0, 255,
     54,   1,   0, 255,
     55,   2,   0, 255,
     56,   3,   0, 255,
     57,   4,   0, 255,
     58,   5,   0, 255,
     59,   6,   0, 255,
     60,   7,   0, 255,
     62,   8,   0, 255,
     63,   9,   0, 255,
     64,  11,   0, 255,
     66,  12,   0, 255,
     68,  13,   0, 255,
     69,  14,   0, 255,
     71,  16,   0, 255,
     73,  17,   0, 255,
     75,  18,   0, 255,
     77,  20,   0, 255,
     79,  21,   0, 255,
     81,  23,   0, 255,
     83,  24,   0, 255,
     85,  26,   0, 255,
     87,  28,   0, 255,
     90,  29,   0, 255,
     92,  31,   0, 255,
     94,  33,   0, 255,
     97,  34,   0, 255,
     99,  36,   0, 255,
    102,  38,   0, 255,
    104,  40,   0, 255,
    107,  41,   0, 255,
    109,  43,   0, 255,
    112,  45,   0, 255,
    115,  47,   0, 255,
    117,  49,   0, 255,
    120,  51,   0, 255,
    123,  52,   0, 255,
    126,  54,   0, 255,
    128,  56,   0, 255,
    131,  58,   0, 255,
    134,  60,   0, 255,
    137,  62,   0, 255,
    140,  64,   0, 255,
    143,  66,   0, 255,
    145,  68,   0, 255,
    148,  70,   0, 255,
    151,  72,   0, 255,
    154,  74,   0, 255
};


enum flip_y_e { flip_y = true };


class the_application : public agg::platform_support
{
    agg::rbox_ctrl<color_type>    m_method;
    agg::slider_ctrl<color_type>  m_radius;
    agg::polygon_ctrl<color_type> m_shadow_ctrl;

    agg::path_storage             m_path;
    typedef agg::conv_curve<agg::path_storage> shape_type;
    shape_type                    m_shape;

    agg::rasterizer_scanline_aa<> m_ras;
    agg::scanline_p8              m_sl;

    agg::rect_d m_shape_bounds;

    agg::pod_array<agg::int8u> m_gray8_buf;
    agg::rendering_buffer      m_gray8_rbuf;
    agg::rendering_buffer      m_gray8_rbuf2;

    agg::pod_array<color_type> m_color_lut;
public:

    the_application(agg::pix_format_e format, bool flip_y) :
        agg::platform_support(format, flip_y),
        m_method     (10.0, 10.0, 130.0, 55.0, !flip_y),
        m_radius     (130 + 10.0, 10.0 + 4.0, 130 + 300.0, 10.0 + 8.0 + 4.0, !flip_y),
        m_shadow_ctrl(4),
        m_shape(m_path)
    {
        add_ctrl(m_method);
        m_method.text_size(8);
        m_method.add_item("Single Color");
        m_method.add_item("Color LUT");
        m_method.cur_item(1);

        add_ctrl(m_radius);
        m_radius.range(0.0, 40.0);
        m_radius.value(15.0);
        m_radius.label("Blur Radius=%1.2f");

        add_ctrl(m_shadow_ctrl);

        m_path.remove_all();
        m_path.move_to(28.47, 6.45);
        m_path.curve3(21.58, 1.12, 19.82, 0.29);
        m_path.curve3(17.19, -0.93, 14.21, -0.93);
        m_path.curve3(9.57, -0.93, 6.57, 2.25);
        m_path.curve3(3.56, 5.42, 3.56, 10.60);
        m_path.curve3(3.56, 13.87, 5.03, 16.26);
        m_path.curve3(7.03, 19.58, 11.99, 22.51);
        m_path.curve3(16.94, 25.44, 28.47, 29.64);
        m_path.line_to(28.47, 31.40);
        m_path.curve3(28.47, 38.09, 26.34, 40.58);
        m_path.curve3(24.22, 43.07, 20.17, 43.07);
        m_path.curve3(17.09, 43.07, 15.28, 41.41);
        m_path.curve3(13.43, 39.75, 13.43, 37.60);
        m_path.line_to(13.53, 34.77);
        m_path.curve3(13.53, 32.52, 12.38, 31.30);
        m_path.curve3(11.23, 30.08, 9.38, 30.08);
        m_path.curve3(7.57, 30.08, 6.42, 31.35);
        m_path.curve3(5.27, 32.62, 5.27, 34.81);
        m_path.curve3(5.27, 39.01, 9.57, 42.53);
        m_path.curve3(13.87, 46.04, 21.63, 46.04);
        m_path.curve3(27.59, 46.04, 31.40, 44.04);
        m_path.curve3(34.28, 42.53, 35.64, 39.31);
        m_path.curve3(36.52, 37.21, 36.52, 30.71);
        m_path.line_to(36.52, 15.53);
        m_path.curve3(36.52, 9.13, 36.77, 7.69);
        m_path.curve3(37.01, 6.25, 37.57, 5.76);
        m_path.curve3(38.13, 5.27, 38.87, 5.27);
        m_path.curve3(39.65, 5.27, 40.23, 5.62);
        m_path.curve3(41.26, 6.25, 44.19, 9.18);
        m_path.line_to(44.19, 6.45);
        m_path.curve3(38.72, -0.88, 33.74, -0.88);
        m_path.curve3(31.35, -0.88, 29.93, 0.78);
        m_path.curve3(28.52, 2.44, 28.47, 6.45);
        m_path.close_polygon();

        m_path.move_to(28.47, 9.62);
        m_path.line_to(28.47, 26.66);
        m_path.curve3(21.09, 23.73, 18.95, 22.51);
        m_path.curve3(15.09, 20.36, 13.43, 18.02);
        m_path.curve3(11.77, 15.67, 11.77, 12.89);
        m_path.curve3(11.77, 9.38, 13.87, 7.06);
        m_path.curve3(15.97, 4.74, 18.70, 4.74);
        m_path.curve3(22.41, 4.74, 28.47, 9.62);
        m_path.close_polygon();

        agg::trans_affine shape_mtx;
        shape_mtx *= agg::trans_affine_scaling(4.0);
        shape_mtx *= agg::trans_affine_translation(150, 100);
        m_path.transform(shape_mtx);

        agg::bounding_rect_single(m_shape, 0, 
                                  &m_shape_bounds.x1, &m_shape_bounds.y1, 
                                  &m_shape_bounds.x2, &m_shape_bounds.y2);

        m_shadow_ctrl.xn(0) = m_shape_bounds.x1;
        m_shadow_ctrl.yn(0) = m_shape_bounds.y1;
        m_shadow_ctrl.xn(1) = m_shape_bounds.x2;
        m_shadow_ctrl.yn(1) = m_shape_bounds.y1;
        m_shadow_ctrl.xn(2) = m_shape_bounds.x2;
        m_shadow_ctrl.yn(2) = m_shape_bounds.y2;
        m_shadow_ctrl.xn(3) = m_shape_bounds.x1;
        m_shadow_ctrl.yn(3) = m_shape_bounds.y2;
        m_shadow_ctrl.line_color(agg::rgba(0, 0.3, 0.5, 0.3));


        m_color_lut.resize(256);
        unsigned i;
        const agg::int8u* p = g_gradient_colors;
        for(i = 0; i < 256; i++)
        {
            m_color_lut[i] = agg::srgba8(p[0], p[1], p[2], (i > 63) ? 255 : i * 4);//p[3]);
            //m_color_lut[i].premultiply();
            p += 4;
        }
    }


    virtual void on_resize(int sx, int sy)
    {
        m_gray8_buf.resize(sx * sy);
        m_gray8_rbuf.attach(m_gray8_buf.data(), sx, sy, sx);
    }


    virtual void on_draw()
    {
        typedef agg::pixfmt_sgray8 pixfmt_gray8;
        typedef agg::renderer_base<pixfmt_gray8> ren_base_gray8;

        m_ras.clip_box(0,0, width(), height());

        pixfmt_gray8 pixf_gray8(m_gray8_rbuf);
        ren_base_gray8 renb_gray8(pixf_gray8);
        renb_gray8.clear(agg::sgray8(0));

        // Testing enhanced compositing operations. 
        // Uncomment and replace renb.blend_from_* to renb_blend.blend_from_*
        //----------------
        //typedef agg::comp_op_rgba_minus<color_type, component_order> blender_type;
        //typedef agg::comp_adaptor_rgba<blender_type> blend_adaptor_type;
        //typedef agg::pixfmt_custom_blend_rgba<blend_adaptor_type, agg::rendering_buffer> pixfmt_type;
        //typedef agg::renderer_base<pixfmt_type> ren_base;
        //pixfmt_type pixf_blend(rbuf_window());
        //agg::renderer_base<pixfmt_type> renb_blend(pixf_blend);

        pixfmt pixf(rbuf_window());
        agg::renderer_base<pixfmt> renb(pixf);
        renb.clear(agg::rgba(1, 0.95, 0.95));

        agg::trans_perspective shadow_persp(m_shape_bounds.x1, m_shape_bounds.y1, 
                                            m_shape_bounds.x2, m_shape_bounds.y2,
                                            m_shadow_ctrl.polygon());

        agg::conv_transform<shape_type, 
                            agg::trans_perspective> shadow_trans(m_shape, 
                                                                 shadow_persp);

        start_timer();

        // Render shadow
        m_ras.add_path(shadow_trans);
        agg::render_scanlines_aa_solid(m_ras, m_sl, renb_gray8, agg::sgray8(255));

        // Calculate the bounding box and extend it by the blur radius
        agg::rect_d bbox;
        agg::bounding_rect_single(shadow_trans, 0, &bbox.x1, &bbox.y1, &bbox.x2, &bbox.y2);

        bbox.x1 -= m_radius.value();
        bbox.y1 -= m_radius.value();
        bbox.x2 += m_radius.value();
        bbox.y2 += m_radius.value();

        if(bbox.clip(agg::rect_d(0, 0, width(), height())))
        {
            // Create a new pixel renderer and attach it to the main one as a child image. 
            // It returns true if the attachment suceeded. It fails if the rectangle 
            // (bbox) is fully clipped.
            //------------------
            pixfmt_gray8 pixf2(m_gray8_rbuf2);
            if(pixf2.attach(pixf_gray8, int(bbox.x1), int(bbox.y1), int(bbox.x2), int(bbox.y2)))
            {
                // Blur it
                agg::stack_blur_gray8(pixf2, agg::uround(m_radius.value()), 
                                             agg::uround(m_radius.value()));
            }
            if(m_method.cur_item() == 0)
            {
                renb.blend_from_color(pixf2, 
                                      agg::srgba8(0, 100, 0), 
                                      0, 
                                      int(bbox.x1), 
                                      int(bbox.y1));
            }
            else
            {
                renb.blend_from_lut(pixf2, 
                                    m_color_lut.data(),
                                    0, 
                                    int(bbox.x1), 
                                    int(bbox.y1));
            }
        }
        double tm = elapsed_time();

        char buf[64]; 
        agg::gsv_text t;
        t.size(10.0);

        agg::conv_stroke<agg::gsv_text> st(t);
        st.width(1.5);

        sprintf(buf, "%3.2f ms", tm);
        t.start_point(140.0, 30.0);
        t.text(buf);

        m_ras.add_path(st);
        agg::render_scanlines_aa_solid(m_ras, m_sl, renb, agg::rgba(0,0,0));

        agg::render_ctrl(m_ras, m_sl, renb, m_method);
        agg::render_ctrl(m_ras, m_sl, renb, m_radius);
        agg::render_ctrl(m_ras, m_sl, renb, m_shadow_ctrl);
    }

};



int agg_main(int argc, char* argv[])
{
    the_application app(pix_format, flip_y);
    app.caption("AGG Example. Gaussian and Stack Blur");

    if(app.init(440, 330, 0))
    {
        return app.run();
    }
    return 1;
}


