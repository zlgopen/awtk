#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_path_storage.h"
#include "agg_trans_affine.h"
#include "agg_conv_transform.h"
#include "agg_conv_smooth_poly1.h"
#include "agg_scanline_p.h"
#include "agg_renderer_scanline.h"
#include "agg_span_allocator.h"
#include "agg_span_pattern_gray.h"
#include "agg_span_pattern_rgb.h"
#include "agg_span_pattern_rgba.h"
#include "agg_image_accessors.h"
#include "ctrl/agg_slider_ctrl.h"
#include "ctrl/agg_rbox_ctrl.h"
#include "ctrl/agg_cbox_ctrl.h"
#include "platform/agg_platform_support.h"

enum flip_y_e { flip_y = true };

//#define AGG_GRAY8 
//#define AGG_BGR24 
//#define AGG_RGB24
#define AGG_BGRA32 
//#define AGG_RGBA32 
//#define AGG_ARGB32 
//#define AGG_ABGR32
//#define AGG_RGB565
//#define AGG_RGB555
#include "pixel_formats.h"




class the_application : public agg::platform_support
{
    agg::slider_ctrl<agg::rgba> m_polygon_angle;
    agg::slider_ctrl<agg::rgba> m_polygon_scale;

    agg::slider_ctrl<agg::rgba> m_pattern_angle;
    agg::slider_ctrl<agg::rgba> m_pattern_size;

    agg::slider_ctrl<agg::rgba> m_pattern_alpha;

    agg::cbox_ctrl<agg::rgba> m_rotate_polygon;
    agg::cbox_ctrl<agg::rgba> m_rotate_pattern;
    agg::cbox_ctrl<agg::rgba> m_tie_pattern;

    double m_polygon_cx;
    double m_polygon_cy;

    double m_dx;
    double m_dy;

    int m_flag;

    agg::int8u*           m_pattern;
    agg::rendering_buffer m_pattern_rbuf;

    agg::rasterizer_scanline_aa<> m_ras;
    agg::scanline_p8 m_sl;
    agg::path_storage m_ps;


public:
    //------------------------------------------------------------------------
    the_application(agg::pix_format_e format, bool flip_y) :
        agg::platform_support(format, flip_y),
        m_polygon_angle(5,    5,         145, 12,    !flip_y),
        m_polygon_scale(5,    5+14,      145, 12+14, !flip_y),
        m_pattern_angle(155,  5,         300, 12,    !flip_y),
        m_pattern_size (155,  5+14,      300, 12+14, !flip_y),
        m_pattern_alpha(310,  5,         460, 12,    !flip_y),
        m_rotate_polygon(5,   5+14+14,    "Rotate Polygon", !flip_y),
        m_rotate_pattern(5,   5+14+14+14, "Rotate Pattern", !flip_y),
        m_tie_pattern   (155, 5+14+14,    "Tie pattern to polygon", !flip_y),
        m_flag(0),
        m_pattern(0)
    {
        add_ctrl(m_polygon_angle);
        add_ctrl(m_polygon_scale);
        add_ctrl(m_pattern_angle);
        add_ctrl(m_pattern_size);
        add_ctrl(m_pattern_alpha);
        add_ctrl(m_rotate_polygon);
        add_ctrl(m_rotate_pattern);
        add_ctrl(m_tie_pattern);

        m_polygon_angle.label("Polygon Angle=%3.2f");
        m_polygon_angle.range(-180.0, 180.0);

        m_polygon_scale.label("Polygon Scale=%3.2f");
        m_polygon_scale.range(0.1, 5.0);
        m_polygon_scale.value(1.0);

        m_pattern_angle.label("Pattern Angle=%3.2f");
        m_pattern_angle.range(-180.0, 180.0);

        m_pattern_size.label("Pattern Size=%3.2f");
        m_pattern_size.range(10, 40);
        m_pattern_size.value(30);

        m_pattern_alpha.label("Background Alpha=%.2f");
        m_pattern_alpha.value(0.1);
    }

    //------------------------------------------------------------------------
    virtual ~the_application()
    {
        delete [] m_pattern;
    }


    //------------------------------------------------------------------------
    void create_star(double xc, double yc,
                     double r1, double r2, 
                     unsigned n, double start_angle = 0.0)
    {
        m_ps.remove_all();
        unsigned i;
        start_angle *= agg::pi / 180.0;
        for(i = 0; i < n; i++)
        {
            double a = agg::pi * 2.0 * i / n - agg::pi / 2.0;
            double dx = cos(a + start_angle);
            double dy = sin(a + start_angle);

            if(i & 1)
            {
                m_ps.line_to(xc + dx * r1, yc + dy * r1);
            }
            else
            {
                if(i) m_ps.line_to(xc + dx * r2, yc + dy * r2);
                else  m_ps.move_to(xc + dx * r2, yc + dy * r2);
            }
        }
        m_ps.close_polygon();
    }


    //------------------------------------------------------------------------
    void generate_pattern()
    {
        unsigned size = unsigned(m_pattern_size.value());

        create_star(m_pattern_size.value() / 2.0, 
                    m_pattern_size.value() / 2.0, 
                    m_pattern_size.value() / 2.5, 
                    m_pattern_size.value() / 6.0, 
                    6,
                    m_pattern_angle.value());

        agg::conv_smooth_poly1_curve<agg::path_storage> smooth(m_ps);
        agg::conv_stroke<agg::conv_smooth_poly1_curve<agg::path_storage> > stroke(smooth);

        smooth.smooth_value(1.0);
        smooth.approximation_scale(4.0);
        stroke.width(m_pattern_size.value() / 15.0);

        delete [] m_pattern;
        m_pattern = new agg::int8u[size * size * 4];
        m_pattern_rbuf.attach(m_pattern, size, size, size*4);

        pixfmt pixf(m_pattern_rbuf);
        agg::renderer_base<pixfmt> rb(pixf);
        agg::renderer_scanline_aa_solid<agg::renderer_base<pixfmt> > rs(rb);

        rb.clear(agg::rgba_pre(0.4, 0.0, 0.1, m_pattern_alpha.value())); // Pattern background color

        m_ras.add_path(smooth);
        rs.color(agg::rgba8(110,130,50));
        agg::render_scanlines(m_ras, m_sl, rs);

        m_ras.add_path(stroke);
        rs.color(agg::rgba8(0,50,80));
        agg::render_scanlines(m_ras, m_sl, rs);
    }


    //------------------------------------------------------------------------
    virtual void on_init()
    {
        m_polygon_cx = initial_width() / 2.0;
        m_polygon_cy = initial_height() / 2.0;
        generate_pattern();
    }


    //------------------------------------------------------------------------
    virtual void on_draw()
    {
        double width = rbuf_window().width();
        double height = rbuf_window().height();
    
        typedef agg::renderer_base<pixfmt>     renderer_base;
        typedef agg::renderer_base<pixfmt_pre> renderer_base_pre;
       
        pixfmt     pixf(rbuf_window());
        pixfmt_pre pixf_pre(rbuf_window());

        renderer_base     rb(pixf);
        renderer_base_pre rb_pre(pixf_pre);

        rb.clear(agg::rgba(1.0, 1.0, 1.0));

        agg::trans_affine polygon_mtx;

        polygon_mtx *= agg::trans_affine_translation(-m_polygon_cx, -m_polygon_cy);
        polygon_mtx *= agg::trans_affine_rotation(m_polygon_angle.value() * agg::pi / 180.0);
        polygon_mtx *= agg::trans_affine_scaling(m_polygon_scale.value());
        polygon_mtx *= agg::trans_affine_translation(m_polygon_cx, m_polygon_cy);

        double r = initial_width() / 3.0 - 8.0;
        create_star(m_polygon_cx, m_polygon_cy, r, r / 1.45, 14);

        agg::conv_transform<agg::path_storage> tr(m_ps, polygon_mtx);

        typedef agg::wrap_mode_reflect_auto_pow2 wrap_x_type;
        typedef agg::wrap_mode_reflect_auto_pow2 wrap_y_type;
        typedef agg::image_accessor_wrap<pixfmt, 
                                         wrap_x_type,
                                         wrap_y_type> img_source_type;
        typedef agg::span_pattern_rgba<img_source_type> span_gen_type;

        unsigned offset_x = 0;
        unsigned offset_y = 0;

        if(m_tie_pattern.status())
        {
            offset_x = unsigned(width-m_polygon_cx);
            offset_y = unsigned(height-m_polygon_cy);
        }

        agg::span_allocator<color_type> sa;
        pixfmt          img_pixf(m_pattern_rbuf);
        img_source_type img_src(img_pixf);
        span_gen_type sg(img_src, offset_x, offset_y);

        // Alpha is meaningful for RGB only because RGBA has its own
        sg.alpha(span_gen_type::value_type(m_pattern_alpha.value() * 255.0)); 

        m_ras.add_path(tr);
        agg::render_scanlines_aa(m_ras, m_sl, rb_pre, sa, sg);

        agg::render_ctrl(m_ras, m_sl, rb, m_polygon_angle);
        agg::render_ctrl(m_ras, m_sl, rb, m_polygon_scale);
        agg::render_ctrl(m_ras, m_sl, rb, m_pattern_angle);
        agg::render_ctrl(m_ras, m_sl, rb, m_pattern_size);
        agg::render_ctrl(m_ras, m_sl, rb, m_pattern_alpha);
        agg::render_ctrl(m_ras, m_sl, rb, m_rotate_polygon);
        agg::render_ctrl(m_ras, m_sl, rb, m_rotate_pattern);
        agg::render_ctrl(m_ras, m_sl, rb, m_tie_pattern);
    }



    //------------------------------------------------------------------------
    virtual void on_mouse_button_down(int x, int y, unsigned flags)
    {
        if(flags & agg::mouse_left)
        {
            agg::trans_affine polygon_mtx;

            polygon_mtx *= agg::trans_affine_translation(-m_polygon_cx, -m_polygon_cy);
            polygon_mtx *= agg::trans_affine_rotation(m_polygon_angle.value() * agg::pi / 180.0);
            polygon_mtx *= agg::trans_affine_scaling(m_polygon_scale.value(), m_polygon_scale.value());
            polygon_mtx *= agg::trans_affine_translation(m_polygon_cx, m_polygon_cy);

            double r = initial_width() / 3.0 - 8.0;
            create_star(m_polygon_cx, m_polygon_cy, r, r / 1.45, 14);

            agg::conv_transform<agg::path_storage> tr(m_ps, polygon_mtx);
            m_ras.add_path(tr);
            if(m_ras.hit_test(x, y))
            {
                m_dx = x - m_polygon_cx;
                m_dy = y - m_polygon_cy;
                m_flag = 1;
            }
        }
    }


    //------------------------------------------------------------------------
    virtual void on_mouse_move(int x, int y, unsigned flags)
    {
        if(flags & agg::mouse_left)
        {
            if(m_flag)
            {
                m_polygon_cx = x - m_dx;
                m_polygon_cy = y - m_dy;
                force_redraw();
            }
        }
        else
        {
            on_mouse_button_up(x, y, flags);
        }
    }



    //------------------------------------------------------------------------
    virtual void on_mouse_button_up(int x, int y, unsigned flags)
    {
        m_flag = 0;
    }



    //------------------------------------------------------------------------
    virtual void on_ctrl_change() 
    {
        if(m_rotate_polygon.status() || m_rotate_pattern.status())
        {
            wait_mode(false);
        }
        else
        {
            wait_mode(true);
        }

        generate_pattern();
        force_redraw();
    }


    //------------------------------------------------------------------------
    virtual void on_idle() 
    {
        bool redraw = false;
        if(m_rotate_polygon.status())
        {
            m_polygon_angle.value(m_polygon_angle.value() + 0.5);
            if(m_polygon_angle.value() >= 180.0) 
            {
                m_polygon_angle.value(m_polygon_angle.value() - 360.0);
            }
            redraw = true;
        }

        if(m_rotate_pattern.status())
        {
            m_pattern_angle.value(m_pattern_angle.value() - 0.5);
            if(m_pattern_angle.value() <= -180.0) 
            {
                m_pattern_angle.value(m_pattern_angle.value() + 360.0);
            }
            generate_pattern();
            redraw = true;
        }

        if(redraw) force_redraw();

    }


};





int agg_main(int argc, char* argv[])
{
    the_application app(pix_format, flip_y);
    app.caption("AGG Example: Pattern Filling");

    if(app.init(640, 480, 0))
    {
        return app.run();
    }
    return 0;
}


