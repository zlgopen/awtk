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
#include "agg_renderer_scanline.h"
#include "agg_pixfmt_rgba.h"
#include "agg_span_allocator.h"
#include "agg_span_image_filter_rgba.h"
#include "agg_span_interpolator_linear.h"
#include "agg_image_accessors.h"
#include "ctrl/agg_slider_ctrl.h"
#include "ctrl/agg_rbox_ctrl.h"
#include "ctrl/agg_cbox_ctrl.h"
#include "platform/agg_platform_support.h"

#define AGG_BGRA32
#include "pixel_formats.h"

enum flip_y_e { flip_y = true };

using agg::rgba;

static rgba g_image[] = 
{
    rgba(0,1,0,1), rgba(1,0,0,1), rgba(1,1,1,1), rgba(0,0,1,1), 
    rgba(0,0,1,1), rgba(0,0,0,1), rgba(1,1,1,1), rgba(1,1,1,1), 
    rgba(1,1,1,1), rgba(1,1,1,1), rgba(1,0,0,1), rgba(0,0,1,1), 
    rgba(1,0,0,1), rgba(1,1,1,1), rgba(0,0,0,1), rgba(0,1,0,1)
};

class the_application : public agg::platform_support
{
    typedef agg::renderer_base<pixfmt> renderer_base;
    typedef agg::renderer_base<pixfmt_pre> renderer_base_pre;

    agg::slider_ctrl<agg::rgba> m_radius;
    agg::rbox_ctrl<agg::rgba>   m_filters;
    agg::cbox_ctrl<agg::rgba>   m_normalize;

    double  m_cur_angle;
    int     m_cur_filter;
    int     m_num_steps;
    double  m_num_pix;
    clock_t m_time1;
    clock_t m_time2;

public:
    the_application(agg::pix_format_e format, bool flip_y) :
        agg::platform_support(format, flip_y),
        m_radius(115,  5, 500-5, 11,  !flip_y),
        m_filters(0.0, 0.0, 110.0, 210.0, !flip_y),
        m_normalize  (8.0, 215.0, "Normalize Filter", !flip_y),
        m_cur_angle(0.0),
        m_cur_filter(1),
        m_num_steps(0),
        m_num_pix(0.0),
        m_time1(0),
        m_time2(0)
    {
        add_ctrl(m_radius);
        add_ctrl(m_filters);
        add_ctrl(m_normalize);
        m_normalize.text_size(7.5);
        m_normalize.status(true);

        m_radius.label("Filter Radius=%.3f");
        m_radius.range(2.0, 8.0);
        m_radius.value(4.0);

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
        agg::scanline_u8 sl;

        agg::pod_array<agg::int8u> image(4 * 4 * pixfmt::pix_width);
        agg::rendering_buffer img_rbuf(image.data(), 4, 4, 4 * pixfmt::pix_width);
        pixfmt img_pixf(img_rbuf);
        for (int y = 0; y < 4; ++y)
            for (int x = 0; x < 4; ++x)
                img_pixf.copy_pixel(x, y, g_image[4 * y + x]);

        double para[] = { 200, 40, 200+300, 40, 200+300, 40+300, 200, 40+300 };
        agg::trans_affine img_mtx(para, 0,0,4,4);

        typedef agg::span_interpolator_linear<> interpolator_type;
        interpolator_type interpolator(img_mtx); 
        agg::span_allocator<color_type> sa;

        typedef agg::image_accessor_clone<pixfmt> img_source_type;
        img_source_type source(img_pixf);

        ras.reset();
        ras.move_to_d(para[0], para[1]);
        ras.line_to_d(para[2], para[3]);
        ras.line_to_d(para[4], para[5]);
        ras.line_to_d(para[6], para[7]);

        switch(m_filters.cur_item())
        {
        case 0:
            {
                typedef agg::span_image_filter_rgba_nn<img_source_type,
                                                       interpolator_type> span_gen_type;

                span_gen_type sg(source, interpolator);
                agg::render_scanlines_aa(ras, sl, rb, sa, sg);
            }
            break;

        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
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
                agg::image_filter_lut filter;
                bool norm = m_normalize.status();
                switch(m_filters.cur_item())
                {
                case 1:  filter.calculate(agg::image_filter_bilinear(),                 norm); break; 
                case 2:  filter.calculate(agg::image_filter_bicubic(),                  norm); break; 
                case 3:  filter.calculate(agg::image_filter_spline16(),                 norm); break; 
                case 4:  filter.calculate(agg::image_filter_spline36(),                 norm); break; 
                case 5:  filter.calculate(agg::image_filter_hanning(),                  norm); break; 
                case 6:  filter.calculate(agg::image_filter_hamming(),                  norm); break; 
                case 7:  filter.calculate(agg::image_filter_hermite(),                  norm); break; 
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

                typedef agg::span_image_filter_rgba<img_source_type,
                                                    interpolator_type> span_gen_type;

                span_gen_type sg(source, interpolator, filter);
                agg::render_scanlines_aa(ras, sl, rb, sa, sg);

                double x_start = 5.0;
                double x_end   = 195.0;
                double y_start = 235.0;
                double y_end   = initial_height() - 5.0;
                double x_center = (x_start + x_end) / 2;

                agg::path_storage p;
                agg::conv_stroke<agg::path_storage> stroke(p);
                stroke.width(0.8);

                unsigned i;
                for(i = 0; i <= 16; i++)
                {
                    double x = x_start + (x_end - x_start) * i / 16.0;
                    p.remove_all();
                    p.move_to(x+0.5, y_start);
                    p.line_to(x+0.5, y_end);
                    ras.add_path(stroke);
                    agg::render_scanlines_aa_solid(ras, sl, rb, 
                                                   agg::srgba8(0, 0, 0, i == 8 ? 255 : 100));
                }
                
                double ys = y_start + (y_end - y_start) / 6.0;
                p.remove_all();
                p.move_to(x_start, ys);
                p.line_to(x_end,   ys);
                ras.add_path(stroke);
                agg::render_scanlines_aa_solid(ras, sl, rb, agg::srgba8(0, 0, 0));

                double radius = filter.radius();
                unsigned n = unsigned(radius * 256 * 2);
                double dx = (x_end - x_start) * radius / 8.0;
                double dy = y_end - ys;

                const agg::int16* weights = filter.weight_array();
                double xs = (x_end + x_start)/2.0 - (filter.diameter() * (x_end - x_start) / 32.0);
                unsigned nn = filter.diameter() * 256;
                p.remove_all();
                p.move_to(xs+0.5, ys + dy * weights[0] / agg::image_filter_scale);
                for(i = 1; i < nn; i++)
                {
                    p.line_to(xs + dx * i / n + 0.5,
                              ys + dy * weights[i] / agg::image_filter_scale);
                }
                ras.add_path(stroke);
                agg::render_scanlines_aa_solid(ras, sl, rb, agg::srgba8(100, 0, 0));
            }
            break;
        }

        if(m_filters.cur_item() >= 14)
        {
            agg::render_ctrl(ras, sl, rb, m_radius);
        }
        agg::render_ctrl(ras, sl, rb, m_filters);
        agg::render_ctrl(ras, sl, rb, m_normalize);
    }

};





int agg_main(int argc, char* argv[])
{
    the_application app(pix_format, flip_y);
    app.caption("Image transformation filters comparison");

    if(app.init(500, 340, 0))
    {
        return app.run();
    }
    return 0;
}


