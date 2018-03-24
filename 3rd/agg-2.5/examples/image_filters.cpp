#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_ellipse.h"
#include "agg_trans_affine.h"
#include "agg_conv_transform.h"
#include "agg_scanline_u.h"
#include "agg_scanline_p.h"
#include "agg_image_accessors.h"
#include "agg_renderer_scanline.h"
#include "agg_span_allocator.h"
#include "agg_span_interpolator_linear.h"
#include "ctrl/agg_slider_ctrl.h"
#include "ctrl/agg_rbox_ctrl.h"
#include "ctrl/agg_cbox_ctrl.h"
#include "platform/agg_platform_support.h"


#include "agg_pixfmt_rgb.h"
#include "agg_span_image_filter_rgb.h"
#define span_image_filter          span_image_filter_rgb
#define span_image_filter_nn       span_image_filter_rgb_nn
#define span_image_filter_bilinear span_image_filter_rgb_bilinear_clip
#define span_image_filter_2x2      span_image_filter_rgb_2x2
#define pix_format agg::pix_format_bgr24
typedef agg::pixfmt_bgr24 pixfmt;
typedef agg::pixfmt_bgr24_pre pixfmt_pre;
typedef pixfmt::color_type color_type;

enum flip_y_e { flip_y = true };

// Uncomment this for more accurate timings. 
// Well, it currently works well only on Windows, where there's
// high accuracy QueryPerformanceCounter is available. On Unix
// it uses regular clock() anyway, so, it won't work for short
// time periods and will give you totally wrong result (probably #INF).
#define AGG_ACCURATE_TIME  


class the_application : public agg::platform_support
{
    typedef agg::renderer_base<pixfmt> renderer_base;
    typedef agg::renderer_base<pixfmt_pre> renderer_base_pre;

    agg::slider_ctrl<agg::rgba> m_radius;
    agg::slider_ctrl<agg::rgba> m_step;
    agg::rbox_ctrl<agg::rgba>   m_filters;
    agg::cbox_ctrl<agg::rgba>   m_normalize;
    agg::cbox_ctrl<agg::rgba>   m_run;
    agg::cbox_ctrl<agg::rgba>   m_single_step;
    agg::cbox_ctrl<agg::rgba>   m_refresh;

    double  m_cur_angle;
    int     m_cur_filter;
    int     m_num_steps;
    double  m_num_pix;
    double  m_time1;
    double  m_time2;

public:
    the_application(agg::pix_format_e format, bool flip_y) :
        agg::platform_support(format, flip_y),
        m_step  (115,  5,    400, 11,     !flip_y),
        m_radius(115,  5+15, 400, 11+15,  !flip_y),
        m_filters(0.0, 0.0, 110.0, 210.0, !flip_y),
        m_normalize  (8.0, 215.0, "Normalize Filter", !flip_y),
        m_run        (8.0, 245.0, "RUN Test!", !flip_y),
        m_single_step(8.0, 230.0, "Single Step", !flip_y),
        m_refresh    (8.0, 265.0, "Refresh", !flip_y),
        m_cur_angle(0.0),
        m_cur_filter(1),
        m_num_steps(0),
        m_num_pix(0.0),
        m_time1(0),
        m_time2(0)
    {
        add_ctrl(m_radius);
        add_ctrl(m_step);
        add_ctrl(m_filters);
        add_ctrl(m_run);
        add_ctrl(m_single_step);
        add_ctrl(m_normalize);
        add_ctrl(m_refresh);
        m_run.text_size(7.5);
        m_single_step.text_size(7.5);
        m_normalize.text_size(7.5);
        m_refresh.text_size(7.5);
        m_normalize.status(true);

        m_radius.label("Filter Radius=%.3f");
        m_step.label("Step=%3.2f");
        m_radius.range(2.0, 8.0);
        m_radius.value(4.0);
        m_step.range(1.0, 10.0);
        m_step.value(5.0);

        m_filters.add_item("simple (NN)");
        m_filters.add_item("bilinear");
        m_filters.add_item("bicubic");
        m_filters.add_item("spline16");
        m_filters.add_item("spline36");
        m_filters.add_item("hanning");
        m_filters.add_item("hamming");
        m_filters.add_item("hermite");
        m_filters.add_item("kaiser");
        m_filters.add_item("quadric");
        m_filters.add_item("catrom");
        m_filters.add_item("gaussian");
        m_filters.add_item("bessel");
        m_filters.add_item("mitchell");
        m_filters.add_item("sinc");
        m_filters.add_item("lanczos");
        m_filters.add_item("blackman");
        m_filters.cur_item(1);

        m_filters.border_width(0, 0);
        m_filters.background_color(agg::rgba(0.0, 0.0, 0.0, 0.1));
        m_filters.text_size(6.0);
        m_filters.text_thickness(0.85);
    }

    virtual ~the_application()
    {
    }

    virtual void on_draw()
    {
        pixfmt pixf(rbuf_window());
        renderer_base rb(pixf);

        rb.clear(agg::rgba(1.0, 1.0, 1.0));
        rb.copy_from(rbuf_img(0), 0, 110, 35);

        agg::rasterizer_scanline_aa<> ras;
        agg::scanline_p8 sl;

        char buf[64]; 
        sprintf(buf, "NSteps=%d", m_num_steps);
        agg::gsv_text t;
        t.start_point(10.0, 295.0);
        t.size(10.0);
        t.text(buf);

        agg::conv_stroke<agg::gsv_text> pt(t);
        pt.width(1.5);

        ras.add_path(pt);
        agg::render_scanlines_aa_solid(ras, sl, rb, agg::rgba(0,0,0));

        if(m_time1 != m_time2 && m_num_pix > 0.0)
        {
#ifdef AGG_ACCURATE_TIME
            sprintf(buf, "%3.2f Kpix/sec", m_num_pix / (m_time2 - m_time1));
#else
            sprintf(buf, "%3.2f Kpix/sec", m_num_pix / 
                                          1000.0 /
                                          (double(m_time2 - m_time1) / CLOCKS_PER_SEC));
#endif
            t.start_point(10.0, 310.0);
            t.text(buf);
            ras.add_path(pt);
            agg::render_scanlines_aa_solid(ras, sl, rb, agg::rgba(0,0,0));
        }

        if(m_filters.cur_item() >= 14)
        {
            agg::render_ctrl(ras, sl, rb, m_radius);
        }
        agg::render_ctrl(ras, sl, rb, m_step);
        agg::render_ctrl(ras, sl, rb, m_filters);
        agg::render_ctrl(ras, sl, rb, m_run);
        agg::render_ctrl(ras, sl, rb, m_normalize);
        agg::render_ctrl(ras, sl, rb, m_single_step);
        agg::render_ctrl(ras, sl, rb, m_refresh);
    }



    void transform_image(double angle)
    {
        double width = rbuf_img(0).width();
        double height = rbuf_img(0).height();

        pixfmt pixf(rbuf_img(0));
        pixfmt_pre pixf_pre(rbuf_img(0));
        renderer_base rb(pixf);
        renderer_base_pre rb_pre(pixf_pre);

        rb.clear(agg::rgba(1.0, 1.0, 1.0));

        agg::rasterizer_scanline_aa<> ras;
        agg::scanline_u8 sl;
        agg::span_allocator<color_type> sa;

        agg::trans_affine src_mtx;
        src_mtx *= agg::trans_affine_translation(-width/2.0, -height/2.0);
        src_mtx *= agg::trans_affine_rotation(angle * agg::pi / 180.0);
        src_mtx *= agg::trans_affine_translation(width/2.0, height/2.0);

        agg::trans_affine img_mtx = src_mtx;
        img_mtx.invert();

        double r = width;
        if(height < r) r = height;

        r *= 0.5;
        r -= 4.0;
        agg::ellipse ell(width  / 2.0, 
                         height / 2.0, 
                         r, r, 200);
        agg::conv_transform<agg::ellipse> tr(ell, src_mtx);

        m_num_pix += r * r * agg::pi;

        typedef agg::span_interpolator_linear<> interpolator_type;
        interpolator_type interpolator(img_mtx); 

        agg::image_filter_lut filter;
        bool norm = m_normalize.status();

        typedef agg::image_accessor_clip<pixfmt> source_type;
        pixfmt pixf_img(rbuf_img(1));
        source_type source(pixf_img, agg::rgba_pre(0,0,0,0));

        switch(m_filters.cur_item())
        {
        case 0:
            {
                typedef agg::span_image_filter_nn<source_type, interpolator_type> span_gen_type;
                span_gen_type sg(source, interpolator);
                ras.add_path(tr);
                agg::render_scanlines_aa(ras, sl, rb_pre, sa, sg);
            }
            break;

        case 1:
            {
                typedef agg::span_image_filter_bilinear<pixfmt, interpolator_type> span_gen_type;
                span_gen_type sg(pixf_img, agg::rgba_pre(0,0,0,0), interpolator);
                ras.add_path(tr);
                agg::render_scanlines_aa(ras, sl, rb_pre, sa, sg);
            }
            break;

        case 5:
        case 6:
        case 7:
            {
                switch(m_filters.cur_item())
                {
                case 5:  filter.calculate(agg::image_filter_hanning(),  norm); break; 
                case 6:  filter.calculate(agg::image_filter_hamming(),  norm); break; 
                case 7:  filter.calculate(agg::image_filter_hermite(),  norm); break; 
                }

                typedef agg::span_image_filter_2x2<source_type, interpolator_type> span_gen_type;
                span_gen_type sg(source, interpolator, filter);
                ras.add_path(tr);
                agg::render_scanlines_aa(ras, sl, rb_pre, sa, sg);
            }
            break;

        case 2:
        case 3:
        case 4:
        case 8:  
        case 9:  
        case 10: 
        case 11: 
        case 12: 
        case 13: 
        case 14: 
        case 15: 
        case 16: 
            {
                switch(m_filters.cur_item())
                {
                case 2:  filter.calculate(agg::image_filter_bicubic(),                  norm); break; 
                case 3:  filter.calculate(agg::image_filter_spline16(),                 norm); break; 
                case 4:  filter.calculate(agg::image_filter_spline36(),                 norm); break; 
                case 8:  filter.calculate(agg::image_filter_kaiser(),                   norm); break; 
                case 9:  filter.calculate(agg::image_filter_quadric(),                  norm); break; 
                case 10: filter.calculate(agg::image_filter_catrom(),                   norm); break; 
                case 11: filter.calculate(agg::image_filter_gaussian(),                 norm); break; 
                case 12: filter.calculate(agg::image_filter_bessel(),                   norm); break; 
                case 13: filter.calculate(agg::image_filter_mitchell(),                 norm); break; 
                case 14: filter.calculate(agg::image_filter_sinc(m_radius.value()),     norm); break; 
                case 15: filter.calculate(agg::image_filter_lanczos(m_radius.value()),  norm); break; 
                case 16: filter.calculate(agg::image_filter_blackman(m_radius.value()), norm); break; 
                }

                typedef agg::span_image_filter<source_type, interpolator_type> span_gen_type;
                span_gen_type sg(source, interpolator, filter);
                ras.add_path(tr);
                agg::render_scanlines_aa(ras, sl, rb_pre, sa, sg);
            }
            break;
        }

    }


    void on_ctrl_change()
    {
        if(m_single_step.status())
        {
            m_cur_angle += m_step.value();
            copy_img_to_img(1, 0);
            transform_image(m_step.value());
            m_num_steps++;
            force_redraw();
            m_single_step.status(false);
        }

        if(m_run.status())
        {
#ifdef AGG_ACCURATE_TIME
            start_timer();
            m_time1 = m_time2 = elapsed_time();
#else
            m_time1 = m_time2 = clock();
#endif
            m_num_pix = 0.0;
            wait_mode(false);
        }
        if(m_refresh.status() || m_filters.cur_item() != m_cur_filter)
        {
#ifdef AGG_ACCURATE_TIME
            start_timer();
            m_time1 = m_time2 = 0;
#else
            m_time1 = m_time2 = clock();
#endif
            m_num_pix = 0.0;
            m_cur_angle = 0.0;
            copy_img_to_img(1, 2);
            transform_image(0.0);
            m_refresh.status(false);
            m_cur_filter = m_filters.cur_item();
            m_num_steps = 0;
            force_redraw();
        }
    }

    void on_idle()
    {
        if(m_run.status())
        {
            if(m_cur_angle < 360.0)
            {
                m_cur_angle += m_step.value();
                copy_img_to_img(1, 0);
#ifdef AGG_ACCURATE_TIME
                start_timer();
#endif
                transform_image(m_step.value());
#ifdef AGG_ACCURATE_TIME
                m_time2 += elapsed_time();
#endif
                m_num_steps++;
            }
            else
            {
                m_cur_angle = 0.0;
#ifndef AGG_ACCURATE_TIME
                m_time2 = clock();
#endif
                wait_mode(true);
                m_run.status(false);
            }
            force_redraw();
        }
        else
        {
            wait_mode(true);
        }
    }


};





int agg_main(int argc, char* argv[])
{
    the_application app(pix_format, flip_y);
    app.caption("Image transformation filters comparison");

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

    app.copy_img_to_img(1, 0);
    app.copy_img_to_img(2, 0);
    app.transform_image(0.0);

    unsigned w = app.rbuf_img(0).width() + 110;
    unsigned h = app.rbuf_img(0).height() + 40;

    if(w < 305) w = 305;
    if(h < 325) h = 325;

    if(app.init(w, h, 0))
    {
        return app.run();
    }
    return 0;
}


