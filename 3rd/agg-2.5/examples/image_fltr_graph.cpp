#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_ellipse.h"
#include "agg_trans_affine.h"
#include "agg_conv_transform.h"
#include "agg_conv_stroke.h"
#include "agg_pixfmt_rgb.h"
#include "agg_scanline_p.h"
#include "agg_renderer_scanline.h"
#include "agg_image_filters.h"
#include "ctrl/agg_slider_ctrl.h"
#include "ctrl/agg_rbox_ctrl.h"
#include "ctrl/agg_cbox_ctrl.h"
#include "platform/agg_platform_support.h"


enum flip_y_e { flip_y = true };


struct filter_base
{
    virtual double radius() const = 0;
    virtual void set_radius(double r) = 0;
    virtual double calc_weight(double x) const = 0;
};


template<class Filter> struct image_filter_const_radius_adaptor : filter_base
{
    virtual double radius() const { return m_filter.radius(); }
    virtual void set_radius(double r) {}
    virtual double calc_weight(double x) const { return m_filter.calc_weight(fabs(x)); }
    Filter m_filter;
};


template<class Filter> struct image_filter_variable_radius_adaptor : filter_base
{
    virtual double radius() const { return m_filter.radius(); }
    virtual double calc_weight(double x) const { return m_filter.calc_weight(fabs(x)); }
    virtual void set_radius(double r) { m_filter = Filter(r); }
    image_filter_variable_radius_adaptor() : m_filter(2.0) {}
    Filter m_filter;
};



class the_application : public agg::platform_support
{
    agg::slider_ctrl<agg::rgba>  m_radius;
    agg::cbox_ctrl<agg::rgba>  m_bilinear;
    agg::cbox_ctrl<agg::rgba>  m_bicubic;
    agg::cbox_ctrl<agg::rgba>  m_spline16;
    agg::cbox_ctrl<agg::rgba>  m_spline36;
    agg::cbox_ctrl<agg::rgba>  m_hanning;
    agg::cbox_ctrl<agg::rgba>  m_hamming;
    agg::cbox_ctrl<agg::rgba>  m_hermite;
    agg::cbox_ctrl<agg::rgba>  m_kaiser;
    agg::cbox_ctrl<agg::rgba>  m_quadric;
    agg::cbox_ctrl<agg::rgba>  m_catrom;
    agg::cbox_ctrl<agg::rgba>  m_gaussian;
    agg::cbox_ctrl<agg::rgba>  m_bessel;
    agg::cbox_ctrl<agg::rgba>  m_mitchell;
    agg::cbox_ctrl<agg::rgba>  m_sinc;
    agg::cbox_ctrl<agg::rgba>  m_lanczos;
    agg::cbox_ctrl<agg::rgba>  m_blackman;
    agg::cbox_ctrl<agg::rgba>* m_filters[32];

    image_filter_const_radius_adaptor<agg::image_filter_bilinear>    m_filter_bilinear;
    image_filter_const_radius_adaptor<agg::image_filter_bicubic>     m_filter_bicubic;
    image_filter_const_radius_adaptor<agg::image_filter_spline16>    m_filter_spline16;
    image_filter_const_radius_adaptor<agg::image_filter_spline36>    m_filter_spline36;
    image_filter_const_radius_adaptor<agg::image_filter_hanning>     m_filter_hanning;
    image_filter_const_radius_adaptor<agg::image_filter_hamming>     m_filter_hamming;
    image_filter_const_radius_adaptor<agg::image_filter_hermite>     m_filter_hermite;
    image_filter_const_radius_adaptor<agg::image_filter_kaiser>      m_filter_kaiser;
    image_filter_const_radius_adaptor<agg::image_filter_quadric>     m_filter_quadric;
    image_filter_const_radius_adaptor<agg::image_filter_catrom>      m_filter_catrom;
    image_filter_const_radius_adaptor<agg::image_filter_gaussian>    m_filter_gaussian;
    image_filter_const_radius_adaptor<agg::image_filter_bessel>      m_filter_bessel;
    image_filter_const_radius_adaptor<agg::image_filter_mitchell>    m_filter_mitchell;
    image_filter_variable_radius_adaptor<agg::image_filter_sinc>     m_filter_sinc;
    image_filter_variable_radius_adaptor<agg::image_filter_lanczos>  m_filter_lanczos;
    image_filter_variable_radius_adaptor<agg::image_filter_blackman> m_filter_blackman;

    filter_base* m_filter_func[32];
    unsigned     m_num_filters;


public:
    the_application(agg::pix_format_e format, bool flip_y) :
        agg::platform_support(format, flip_y),
        m_radius     (5.0, 5.0, 780-5, 10.0,     !flip_y),
        m_bilinear (8.0, 30.0+15*0,  "bilinear", !flip_y), 
        m_bicubic  (8.0, 30.0+15*1,  "bicubic ", !flip_y),
        m_spline16 (8.0, 30.0+15*2,  "spline16", !flip_y),
        m_spline36 (8.0, 30.0+15*3,  "spline36", !flip_y),
        m_hanning  (8.0, 30.0+15*4,  "hanning ", !flip_y), 
        m_hamming  (8.0, 30.0+15*5,  "hamming ", !flip_y),
        m_hermite  (8.0, 30.0+15*6,  "hermite ", !flip_y),
        m_kaiser   (8.0, 30.0+15*7,  "kaiser  ", !flip_y), 
        m_quadric  (8.0, 30.0+15*8,  "quadric ", !flip_y), 
        m_catrom   (8.0, 30.0+15*9,  "catrom  ", !flip_y), 
        m_gaussian (8.0, 30.0+15*10, "gaussian", !flip_y), 
        m_bessel   (8.0, 30.0+15*11, "bessel  ", !flip_y),
        m_mitchell (8.0, 30.0+15*12, "mitchell", !flip_y), 
        m_sinc     (8.0, 30.0+15*13, "sinc    ", !flip_y),
        m_lanczos  (8.0, 30.0+15*14, "lanczos ", !flip_y), 
        m_blackman (8.0, 30.0+15*15, "blackman", !flip_y), 
        m_num_filters(0)
    {
        m_filters[m_num_filters++] = &m_bilinear;
        m_filters[m_num_filters++] = &m_bicubic;
        m_filters[m_num_filters++] = &m_spline16;
        m_filters[m_num_filters++] = &m_spline36;
        m_filters[m_num_filters++] = &m_hanning;
        m_filters[m_num_filters++] = &m_hamming;
        m_filters[m_num_filters++] = &m_hermite;
        m_filters[m_num_filters++] = &m_kaiser;
        m_filters[m_num_filters++] = &m_quadric;
        m_filters[m_num_filters++] = &m_catrom;
        m_filters[m_num_filters++] = &m_gaussian;
        m_filters[m_num_filters++] = &m_bessel;
        m_filters[m_num_filters++] = &m_mitchell;
        m_filters[m_num_filters++] = &m_sinc;
        m_filters[m_num_filters++] = &m_lanczos;
        m_filters[m_num_filters++] = &m_blackman;

        unsigned i = 0;

        m_filter_func[i++] = &m_filter_bilinear;
        m_filter_func[i++] = &m_filter_bicubic;
        m_filter_func[i++] = &m_filter_spline16;
        m_filter_func[i++] = &m_filter_spline36;
        m_filter_func[i++] = &m_filter_hanning;
        m_filter_func[i++] = &m_filter_hamming;
        m_filter_func[i++] = &m_filter_hermite;
        m_filter_func[i++] = &m_filter_kaiser;
        m_filter_func[i++] = &m_filter_quadric;
        m_filter_func[i++] = &m_filter_catrom;
        m_filter_func[i++] = &m_filter_gaussian;
        m_filter_func[i++] = &m_filter_bessel;
        m_filter_func[i++] = &m_filter_mitchell;
        m_filter_func[i++] = &m_filter_sinc;
        m_filter_func[i++] = &m_filter_lanczos;
        m_filter_func[i++] = &m_filter_blackman;
        for(i = 0; i < m_num_filters; i++)
        {
            add_ctrl(*m_filters[i]);
        }

        m_radius.range(2.0, 8.0);
        m_radius.value(4.0);
        m_radius.label("Radius=%.3f");
        add_ctrl(m_radius);
    }

    virtual ~the_application()
    {
    }

    virtual void on_draw()
    {
        typedef agg::pixfmt_bgr24 pixfmt; 
        typedef agg::renderer_base<pixfmt> renderer_base;
        typedef agg::renderer_scanline_aa_solid<renderer_base> renderer_solid;
       
        pixfmt pixf(rbuf_window());
        renderer_base rb(pixf);
        renderer_solid rs(rb);

        rb.clear(agg::rgba(1.0, 1.0, 1.0));
        agg::rasterizer_scanline_aa<> ras;
        agg::scanline_p8 sl;

        double x_start = 125.0;
        double x_end   = initial_width() - 15.0;
        double y_start = 10.0;
        double y_end   = initial_height() - 10.0;
        double x_center = (x_start + x_end) / 2;

        unsigned i;

        agg::path_storage p;
        agg::conv_stroke<agg::path_storage> pl(p);
        agg::conv_transform<agg::conv_stroke<agg::path_storage> > tr(pl, trans_affine_resizing());

        for(i = 0; i <= 16; i++)
        {
            double x = x_start + (x_end - x_start) * i / 16.0;
            p.remove_all();
            p.move_to(x+0.5, y_start);
            p.line_to(x+0.5, y_end);
            ras.add_path(tr);
            rs.color(agg::rgba8(0, 0, 0, i == 8 ? 255 : 100));
            agg::render_scanlines(ras, sl, rs);
        }

        double ys = y_start + (y_end - y_start) / 6.0;

        p.remove_all();
        p.move_to(x_start, ys);
        p.line_to(x_end,   ys);
        ras.add_path(tr);
        rs.color(agg::rgba8(0, 0, 0));
        agg::render_scanlines(ras, sl, rs);


        pl.width(1.0);
        
        for(i = 0; i < m_num_filters; i++)
        {
            if(m_filters[i]->status())
            {
                m_filter_func[i]->set_radius(m_radius.value());
                unsigned j;

                double radius = m_filter_func[i]->radius();
                unsigned n = unsigned(radius * 256 * 2);
                double dy = y_end - ys;

                double xs = (x_end + x_start)/2.0 - (radius * (x_end - x_start) / 16.0);
                double dx = (x_end - x_start) * radius / 8.0;

                p.remove_all();
                p.move_to(xs+0.5, ys + dy * m_filter_func[i]->calc_weight(-radius));
                for(j = 1; j < n; j++)
                {
                    p.line_to(xs + dx * j / n + 0.5,
                              ys + dy * m_filter_func[i]->calc_weight(j / 256.0 - radius));
                }
                ras.add_path(tr);
                rs.color(agg::rgba8(100, 0, 0));
                agg::render_scanlines(ras, sl, rs);

                p.remove_all();
                unsigned xint;
                int ir = int(ceil(radius) + 0.1);

                for(xint = 0; xint < 256; xint++)
                {
                    int xfract;
                    double sum = 0;
                    for(xfract = -ir; xfract < ir; xfract++) 
                    {
                        double xf = xint/256.0 + xfract;
                        if(xf >= -radius || xf <= radius)
                        {
                            sum += m_filter_func[i]->calc_weight(xf);
                        }
                    }

                    double x = x_center + ((-128.0 + xint) / 128.0) * radius * (x_end - x_start) / 16.0;
                    double y = ys + sum * 256 - 256;

                    if(xint == 0) p.move_to(x, y);
                    else          p.line_to(x, y);
                }
                ras.add_path(tr);
                rs.color(agg::rgba8(0, 100, 0));
                agg::render_scanlines(ras, sl, rs);

                agg::image_filter_lut normalized(*m_filter_func[i]);
                const agg::int16* weights = normalized.weight_array();

                xs = (x_end + x_start)/2.0 - (normalized.diameter() * (x_end - x_start) / 32.0);
                unsigned nn = normalized.diameter() * 256;
                p.remove_all();
                p.move_to(xs+0.5, ys + dy * weights[0] / agg::image_filter_scale);
                for(j = 1; j < nn; j++)
                {
                    p.line_to(xs + dx * j / n + 0.5,
                              ys + dy * weights[j] / agg::image_filter_scale);
                }
                ras.add_path(tr);
                rs.color(agg::rgba8(0, 0, 100, 255));
                agg::render_scanlines(ras, sl, rs);
            }
        }

        for(i = 0; i < m_num_filters; i++)
        {
            agg::render_ctrl(ras, sl, rb, *m_filters[i]);
        }
        if(m_sinc.status() || m_lanczos.status() || m_blackman.status())
        {
            agg::render_ctrl(ras, sl, rb, m_radius);
        }
    }
};







int agg_main(int argc, char* argv[])
{
    the_application app(agg::pix_format_bgr24, flip_y);
    app.caption("Image filters' shape comparison");

    if(app.init(780, 300, agg::window_resize))
    {
        return app.run();
    }
    return 0;
}


