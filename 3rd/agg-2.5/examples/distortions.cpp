#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_ellipse.h"
#include "agg_trans_affine.h"
#include "agg_conv_transform.h"
#include "agg_pixfmt_rgb.h"
#include "agg_span_allocator.h"
#include "agg_span_image_filter_rgb.h"
#include "agg_scanline_u.h"
#include "agg_renderer_scanline.h"
#include "agg_span_interpolator_linear.h"
#include "agg_span_interpolator_adaptor.h"
#include "agg_span_gradient.h"
#include "agg_image_accessors.h"
#include "ctrl/agg_slider_ctrl.h"
#include "ctrl/agg_rbox_ctrl.h"
#include "platform/agg_platform_support.h"


enum flip_y_e { flip_y = true };


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



class periodic_distortion
{
public:
    periodic_distortion() :
        m_cx(0.0),
        m_cy(0.0),
        m_period(0.5),
        m_amplitude(0.5),
        m_phase(0.0)
    {}

    void center(double x, double y) { m_cx = x; m_cy = y; }
    void period(double v)           { m_period = v; }
    void amplitude(double v)        { m_amplitude = 1.0 / v; }
    void phase(double v)            { m_phase = v; }

    virtual void calculate(int* x, int* y) const = 0;

protected:
    double m_cx;
    double m_cy;
    double m_period;
    double m_amplitude;
    double m_phase;
};



inline void calculate_wave(int* x, int* y, 
                           double cx, double cy, 
                           double period, double amplitude, double phase)
{
    double xd = double(*x) / agg::image_subpixel_scale - cx;
    double yd = double(*y) / agg::image_subpixel_scale - cy;
    double d = sqrt(xd*xd + yd*yd);
    if(d > 1)
    {
        double a = cos(d / (16.0 * period) - phase) * (1.0 / (amplitude * d)) + 1.0; 
        *x = int((xd * a + cx) * agg::image_subpixel_scale);
        *y = int((yd * a + cy) * agg::image_subpixel_scale);
    }
}



inline void calculate_swirl(int* x, int* y,
                            double cx, double cy, 
                            double amplitude, double phase)
{
    double xd = double(*x) / agg::image_subpixel_scale - cx;
    double yd = double(*y) / agg::image_subpixel_scale - cy;
    double a = double(100.0 - sqrt(xd * xd + yd * yd)) / 100.0 * (0.1 / -amplitude);
    double sa = sin(a - phase/25.0);
    double ca = cos(a - phase/25.0);
    *x = int((xd * ca - yd * sa + cx) * agg::image_subpixel_scale);
    *y = int((xd * sa + yd * ca + cy) * agg::image_subpixel_scale);
}





class distortion_wave : public periodic_distortion
{
    virtual void calculate(int* x, int* y) const
    {
        calculate_wave(x, y, m_cx, m_cy, m_period, m_amplitude, m_phase);
    }
};

class distortion_swirl : public periodic_distortion
{
    virtual void calculate(int* x, int* y) const
    {
        calculate_swirl(x, y, m_cx, m_cy, m_amplitude, m_phase);
    }
};

class distortion_swirl_wave : public periodic_distortion
{
    virtual void calculate(int* x, int* y) const
    {
        calculate_swirl(x, y, m_cx, m_cy, m_amplitude, m_phase);
        calculate_wave(x, y, m_cx, m_cy, m_period, m_amplitude, m_phase);
    }
};


class distortion_wave_swirl : public periodic_distortion
{
    virtual void calculate(int* x, int* y) const
    {
        calculate_wave(x, y, m_cx, m_cy, m_period, m_amplitude, m_phase);
        calculate_swirl(x, y, m_cx, m_cy, m_amplitude, m_phase);
    }
};










class the_application : public agg::platform_support
{
    agg::slider_ctrl<agg::rgba8> m_angle;
    agg::slider_ctrl<agg::rgba8> m_scale;
    agg::slider_ctrl<agg::rgba8> m_amplitude;
    agg::slider_ctrl<agg::rgba8> m_period;
    agg::rbox_ctrl<agg::rgba8>   m_distortion;

    double m_center_x;
    double m_center_y;
    double m_phase;

    typedef agg::pod_auto_array<agg::rgba8, 256> color_array_type;
    color_array_type m_gradient_colors;

public:
    the_application(agg::pix_format_e format, bool flip_y) :
        agg::platform_support(format, flip_y),
        m_angle     (5,      5,    150,     12,    !flip_y),
        m_scale     (5,      5+15, 150,     12+15, !flip_y),
        m_period    (5+170,  5,    150+170, 12,    !flip_y),
        m_amplitude (5+170,  5+15, 150+170, 12+15, !flip_y),
        m_distortion(480,    5,    600,     90,    !flip_y),

        m_center_x(0.0),
        m_center_y(0.0),
        m_phase(0.0)
    {
        add_ctrl(m_angle);
        add_ctrl(m_scale);
        add_ctrl(m_amplitude);
        add_ctrl(m_period);
        add_ctrl(m_distortion);
        m_angle.label("Angle=%3.2f");
        m_scale.label("Scale=%3.2f");
        m_angle.range(-180.0, 180.0);
        m_angle.value(20.0);
        m_scale.range(0.1, 5.0);
        m_scale.value(1.0);

        m_amplitude.label("Amplitude=%3.2f");
        m_period.label("Period=%3.2f");
        m_amplitude.range(0.1, 40.0);
        m_period.range(0.1, 2.0);
        m_amplitude.value(10.0);
        m_period.value(1.0);

        m_distortion.add_item("Wave");
        m_distortion.add_item("Swirl");
        m_distortion.add_item("Wave-Swirl");
        m_distortion.add_item("Swirl-Wave");
        m_distortion.cur_item(0);

        unsigned i;
        const agg::int8u* p = g_gradient_colors;
        for(i = 0; i < 256; i++)
        {
            m_gradient_colors[i] = agg::rgba8(p[0], p[1], p[2], p[3]);
            p += 4;
        }
    }

    virtual ~the_application()
    {
    }

    virtual void on_init()
    {
        m_center_x = rbuf_img(0).width() / 2.0 + 10;
        m_center_y = rbuf_img(0).height() / 2.0 + 10 + 40;
    }

    virtual void on_draw()
    {
        double img_width = rbuf_img(0).width();
        double img_height = rbuf_img(0).height();
    
        typedef agg::pixfmt_bgr24 pixfmt; 
        typedef agg::renderer_base<pixfmt> renderer_base;

        pixfmt pixf(rbuf_window());
        pixfmt img_pixf(rbuf_img(0));

        renderer_base rb(pixf);

        rb.clear(agg::rgba(1.0, 1.0, 1.0));

        agg::trans_affine src_mtx;
        src_mtx *= agg::trans_affine_translation(-img_width/2, -img_height/2);
        src_mtx *= agg::trans_affine_rotation(m_angle.value() * agg::pi / 180.0);
        src_mtx *= agg::trans_affine_translation(img_width/2 + 10, img_height/2 + 10 + 40);
        src_mtx *= trans_affine_resizing();

        agg::trans_affine img_mtx;
        img_mtx *= agg::trans_affine_translation(-img_width/2, -img_height/2);
        img_mtx *= agg::trans_affine_rotation(m_angle.value() * agg::pi / 180.0);
        img_mtx *= agg::trans_affine_scaling(m_scale.value());
        img_mtx *= agg::trans_affine_translation(img_width/2 + 10, img_height/2 + 10 + 40);
        img_mtx *= trans_affine_resizing();
        img_mtx.invert();


        typedef agg::span_allocator<agg::rgba8> span_alloc_type;

        span_alloc_type sa;
        
        typedef agg::span_interpolator_adaptor<agg::span_interpolator_linear<>, 
                                               periodic_distortion> interpolator_type;

        periodic_distortion*  dist = 0;
        distortion_wave       dist_wave;
        distortion_swirl      dist_swirl;
        distortion_wave_swirl dist_wave_swirl;
        distortion_swirl_wave dist_swirl_wave;

        switch(m_distortion.cur_item())
        {
            case 0: dist = &dist_wave;       break;
            case 1: dist = &dist_swirl;      break;
            case 2: dist = &dist_wave_swirl; break;
            case 3: dist = &dist_swirl_wave; break;
        }

        dist->period(m_period.value());
        dist->amplitude(m_amplitude.value());
        dist->phase(m_phase);
        double cx = m_center_x;
        double cy = m_center_y;
        img_mtx.transform(&cx, &cy);
        dist->center(cx, cy);

        interpolator_type interpolator(img_mtx, *dist);

        typedef agg::image_accessor_clip<pixfmt> img_source_type;
        img_source_type img_src(img_pixf, agg::rgba(1,1,1));

/*
        // Version without filtering (nearest neighbor)
        //------------------------------------------
        typedef agg::span_image_filter_rgb_nn<img_source_type,
                                              interpolator_type> span_gen_type;
        span_gen_type sg(img_src, interpolator);
        //------------------------------------------
*/

        // Version with "hardcoded" bilinear filter and without 
        // image_accessor (direct filter, the old variant)
        //------------------------------------------
        typedef agg::span_image_filter_rgb_bilinear_clip<pixfmt,
                                                         interpolator_type> span_gen_type;
        span_gen_type sg(img_pixf, agg::rgba(1,1,1), interpolator);
        //------------------------------------------

/*
        // Version with arbitrary 2x2 filter
        //------------------------------------------
        typedef agg::span_image_filter_rgb_2x2<img_source_type,
                                               interpolator_type> span_gen_type;
        agg::image_filter<agg::image_filter_kaiser> filter;
        span_gen_type sg(img_src, interpolator, filter);
        //------------------------------------------
*/
/*
        // Version with arbitrary filter
        //------------------------------------------
        typedef agg::span_image_filter_rgb<img_source_type,
                                           interpolator_type> span_gen_type;
        agg::image_filter<agg::image_filter_spline36> filter;
        span_gen_type sg(img_src, interpolator, filter);
        //------------------------------------------
*/


        agg::rasterizer_scanline_aa<> ras;
        agg::scanline_u8 sl;
        double r = img_width;
        if(img_height < r) r = img_height;
        agg::ellipse ell(img_width  / 2.0, 
                         img_height / 2.0, 
                         r / 2.0 - 20.0, 
                         r / 2.0 - 20.0, 200);


        agg::conv_transform<agg::ellipse> tr(ell, src_mtx);

        ras.add_path(tr);
        agg::render_scanlines_aa(ras, sl, rb, sa, sg);

        src_mtx *= ~trans_affine_resizing();
        src_mtx *= agg::trans_affine_translation(img_width - img_width/10, 0.0);
        src_mtx *= trans_affine_resizing();

        ras.add_path(tr);
        agg::render_scanlines_aa_solid(ras, sl, rb, agg::rgba8(0,0,0));

        typedef agg::span_gradient<agg::rgba8, 
                                   interpolator_type,
                                   agg::gradient_circle,
                                   color_array_type> gradient_span_gen;

        agg::gradient_circle gradient_function;

        color_array_type gradient_colors(m_gradient_colors);
        gradient_span_gen span_gradient(interpolator, 
                                        gradient_function, 
                                        gradient_colors, 
                                        0, 180);

        agg::trans_affine gr1_mtx;
        gr1_mtx *= agg::trans_affine_translation(-img_width/2, -img_height/2);
        gr1_mtx *= agg::trans_affine_scaling(0.8);
        gr1_mtx *= agg::trans_affine_rotation(m_angle.value() * agg::pi / 180.0);
        gr1_mtx *= agg::trans_affine_translation(img_width - img_width/10 + img_width/2 + 10, 
                                                 img_height/2 + 10 + 40);
        gr1_mtx *= trans_affine_resizing();

        agg::trans_affine gr2_mtx;
        gr2_mtx *= agg::trans_affine_rotation(m_angle.value() * agg::pi / 180.0);
        gr2_mtx *= agg::trans_affine_scaling(m_scale.value());
        gr2_mtx *= agg::trans_affine_translation(img_width - img_width/10 + img_width/2 + 10 + 50, 
                                                 img_height/2 + 10 + 40 + 50);
        gr2_mtx *= trans_affine_resizing();
        gr2_mtx.invert();

        cx = m_center_x + img_width - img_width/10;
        cy = m_center_y;
        gr2_mtx.transform(&cx, &cy);
        dist->center(cx, cy);

        interpolator.transformer(gr2_mtx);

        agg::conv_transform<agg::ellipse> tr2(ell, gr1_mtx);

        ras.add_path(tr2);
        agg::render_scanlines_aa(ras, sl, rb, sa, span_gradient);

        agg::render_ctrl(ras, sl, rb, m_angle);
        agg::render_ctrl(ras, sl, rb, m_scale);
        agg::render_ctrl(ras, sl, rb, m_amplitude);
        agg::render_ctrl(ras, sl, rb, m_period);
        agg::render_ctrl(ras, sl, rb, m_distortion);
    }


    
    virtual void on_mouse_button_down(int x, int y, unsigned flags)
    {
        if(flags)
        {
            m_center_x = x;
            m_center_y = y;
            force_redraw();
        }
    }



    virtual void on_mouse_move(int x, int y, unsigned flags)
    {
        if(flags & 1)
        {
            m_center_x = x;
            m_center_y = y;
            force_redraw();
        }
    }

    virtual void on_idle()
    {
        m_phase += 15.0 * agg::pi / 180.0;
        if(m_phase > agg::pi * 200.0) m_phase -= agg::pi * 200.0;
        force_redraw();
    }


};





int agg_main(int argc, char* argv[])
{
    the_application app(agg::pix_format_bgr24, flip_y);
    app.caption("Image and Gradient Distortions");

    const char* img_name = "spheres";
    if(argc >= 2) img_name = argv[1];
    if(!app.load_img(0, img_name)) 
    {
        char buf[256];
        if(strcmp(img_name, "spheres") == 0)
        {
            sprintf(buf, "File not found: %s%s. Download http://www.antigrain.com/%s%s\n"
                         "or copy it from another directory if available.",
                    img_name, app.img_ext(), img_name, app.img_ext());
        }
        else
        {
            sprintf(buf, "File not found: %s%s", img_name, app.img_ext());
        }
        app.message(buf);
        return 1;
    }

    if(app.init(app.rbuf_img(0).width() + 300, app.rbuf_img(0).height() + 40 + 20, agg::window_resize))
    {
        app.wait_mode(false);
        return app.run();
    }
    return 0;
}


