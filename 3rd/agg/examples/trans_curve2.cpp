#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_p.h"
#include "agg_renderer_scanline.h"
#include "agg_conv_bspline.h"
#include "agg_conv_segmentator.h"
#include "agg_font_win32_tt.h"
#include "agg_trans_double_path.h"
#include "ctrl/agg_cbox_ctrl.h"
#include "ctrl/agg_slider_ctrl.h"
#include "platform/agg_platform_support.h"
#include "interactive_polygon.h"

#define AGG_BGR24
//#define AGG_RGB24
//#define AGG_BGRA32 
//#define AGG_RGBA32 
//#define AGG_ARGB32 
//#define AGG_ABGR32
//#define AGG_BGR96
//#define AGG_BGRA128
//#define AGG_RGB565
//#define AGG_RGB555
#include "pixel_formats.h"

enum flip_y_e { flip_y = true };




static char text[] = 
"Anti-Grain Geometry is designed as a set of loosely coupled "
"algorithms and class templates united with a common idea, "
"so that all the components can be easily combined. Also, "
"the template based design allows you to replace any part of "
"the library without the necessity to modify a single byte in "
"the existing code. ";




class the_application : public agg::platform_support
{
public:
    typedef agg::renderer_base<pixfmt> renderer_base;
    typedef agg::renderer_scanline_aa_solid<renderer_base> renderer_solid;
    typedef agg::scanline_p8 scanline_type;
    typedef agg::font_engine_win32_tt_int16 font_engine_type;
    typedef agg::font_cache_manager<font_engine_type> font_manager_type;

    font_engine_type             m_feng;
    font_manager_type            m_fman;
    agg::interactive_polygon     m_poly1;
    agg::interactive_polygon     m_poly2;
    agg::slider_ctrl<color_type> m_num_points;
    agg::cbox_ctrl<color_type>   m_fixed_len;
    agg::cbox_ctrl<color_type>   m_preserve_x_scale;
    agg::cbox_ctrl<color_type>   m_animate;
    double                       m_dx1[6];
    double                       m_dy1[6];
    double                       m_dx2[6];
    double                       m_dy2[6];
    bool                         m_prev_animate;

    the_application(HDC dc, agg::pix_format_e format, bool flip_y) :
        agg::platform_support(format, flip_y),
        m_feng(dc),
        m_fman(m_feng),
        m_poly1(6, 5.0),
        m_poly2(6, 5.0),
        m_num_points      (5.0, 5.0, 340.0, 12.0, !flip_y),
        m_fixed_len       (350, 5.0,  "Fixed Length", !flip_y),
        m_preserve_x_scale(465, 5.0,  "Preserve X scale", !flip_y),
        m_animate         (350, 25.0, "Animate", !flip_y),
        m_prev_animate(false)
    {
        add_ctrl(m_fixed_len);
        add_ctrl(m_preserve_x_scale);
        add_ctrl(m_animate);
        m_fixed_len.status(true);
        m_preserve_x_scale.status(true);
        m_num_points.range(10.0, 400.0);
        m_num_points.value(200.0);
        m_num_points.label("Number of intermediate Points = %.3f");
        add_ctrl(m_num_points);

        m_poly1.close(false);
        m_poly2.close(false);
    }


    virtual void on_init()
    {
        m_poly1.xn(0) =  10 + 50;
        m_poly1.yn(0) = -10 + 50;
        m_poly1.xn(1) =  10 + 150 + 20;
        m_poly1.yn(1) = -10 + 150 - 20;
        m_poly1.xn(2) =  10 + 250 - 20;
        m_poly1.yn(2) = -10 + 250 + 20;
        m_poly1.xn(3) =  10 + 350 + 20;
        m_poly1.yn(3) = -10 + 350 - 20;
        m_poly1.xn(4) =  10 + 450 - 20;
        m_poly1.yn(4) = -10 + 450 + 20;
        m_poly1.xn(5) =  10 + 550;
        m_poly1.yn(5) = -10 + 550;

        m_poly2.xn(0) = -10 + 50;
        m_poly2.yn(0) =  10 + 50;
        m_poly2.xn(1) = -10 + 150 + 20;
        m_poly2.yn(1) =  10 + 150 - 20;
        m_poly2.xn(2) = -10 + 250 - 20;
        m_poly2.yn(2) =  10 + 250 + 20;
        m_poly2.xn(3) = -10 + 350 + 20;
        m_poly2.yn(3) =  10 + 350 - 20;
        m_poly2.xn(4) = -10 + 450 - 20;
        m_poly2.yn(4) =  10 + 450 + 20;
        m_poly2.xn(5) = -10 + 550;
        m_poly2.yn(5) =  10 + 550;
    }




    virtual void on_draw()
    {
        pixfmt pixf(rbuf_window());
        renderer_base rb(pixf);
        renderer_solid r(rb);
        rb.clear(agg::rgba(1, 1, 1));

        scanline_type sl;
        agg::rasterizer_scanline_aa<> ras;

        agg::simple_polygon_vertex_source path1(m_poly1.polygon(), 
                                                m_poly1.num_points(), 
                                                false, 
                                                false);

        agg::simple_polygon_vertex_source path2(m_poly2.polygon(), 
                                                m_poly2.num_points(), 
                                                false, 
                                                false);


        typedef agg::conv_bspline<agg::simple_polygon_vertex_source> conv_bspline_type;
        conv_bspline_type bspline1(path1);
        conv_bspline_type bspline2(path2);
        bspline1.interpolation_step(1.0 / m_num_points.value());
        bspline2.interpolation_step(1.0 / m_num_points.value());


        typedef agg::conv_curve<font_manager_type::path_adaptor_type>            conv_font_curve_type;
        typedef agg::conv_segmentator<conv_font_curve_type>                      conv_font_segm_type;
        typedef agg::conv_transform<conv_font_segm_type, agg::trans_double_path> conv_font_trans_type;

        agg::trans_double_path tcurve;
        conv_font_curve_type fcurves(m_fman.path_adaptor());
        conv_font_segm_type  fsegm(fcurves);
        conv_font_trans_type ftrans(fsegm, tcurve);

        tcurve.preserve_x_scale(m_preserve_x_scale.status());
        if(m_fixed_len.status()) tcurve.base_length(1140.0);
        tcurve.base_height(30.0);

        tcurve.add_paths(bspline1, bspline2);
        fsegm.approximation_scale(3.0);
        fcurves.approximation_scale(5.0);



        m_feng.height(40.0);
        m_feng.hinting(false);
        m_feng.italic(true);

        if(m_feng.create_font("Times New Roman", agg::glyph_ren_outline))
        {
            double x = 0.0;
            double y = 3.0;
            const char* p = text;

            while(*p)
            {
                const agg::glyph_cache* glyph = m_fman.glyph(*p);
                if(glyph)
                {
                    if(x > tcurve.total_length1()) break;

                    m_fman.add_kerning(&x, &y);
                    m_fman.init_embedded_adaptors(glyph, x, y);

                    if(glyph->data_type == agg::glyph_data_outline)
                    {
                        ras.reset();
                        ras.add_path(ftrans);
                        r.color(agg::srgba8(0, 0, 0));
                        agg::render_scanlines(ras, sl, r);
                    }

                    // increment pen position
                    x += glyph->advance_x;
                    y += glyph->advance_y;
                }
                ++p;
            }

        }



        typedef agg::conv_stroke<conv_bspline_type> conv_stroke_type;
        conv_stroke_type stroke1(bspline1);
        conv_stroke_type stroke2(bspline2);

        stroke1.width(2.0);
        stroke2.width(2.0);

        r.color(agg::srgba8(170, 50, 20, 100));
        ras.add_path(stroke1);
        agg::render_scanlines(ras, sl, r);

        ras.add_path(stroke2);
        agg::render_scanlines(ras, sl, r);


        //--------------------------
        // Render the "poly" tool and controls
        r.color(agg::rgba(0, 0.3, 0.5, 0.2));
        ras.add_path(m_poly1);
        agg::render_scanlines(ras, sl, r);

        ras.add_path(m_poly2);
        agg::render_scanlines(ras, sl, r);


        agg::render_ctrl(ras, sl, rb, m_fixed_len);
        agg::render_ctrl(ras, sl, rb, m_preserve_x_scale);
        agg::render_ctrl(ras, sl, rb, m_animate);
        agg::render_ctrl(ras, sl, rb, m_num_points);
        //--------------------------

    }



    virtual void on_mouse_button_down(int x, int y, unsigned flags)
    {
        if(flags & agg::mouse_left)
        {
            if(m_poly1.on_mouse_button_down(x, y))
            {
                force_redraw();
            }
            if(m_poly2.on_mouse_button_down(x, y))
            {
                force_redraw();
            }
        }
    }


    virtual void on_mouse_move(int x, int y, unsigned flags)
    {
        if(flags & agg::mouse_left)
        {
            if(m_poly1.on_mouse_move(x, y))
            {
                force_redraw();
            }
            if(m_poly2.on_mouse_move(x, y))
            {
                force_redraw();
            }
        }
        if((flags & agg::mouse_left) == 0)
        {
            on_mouse_button_up(x, y, flags);
        }
    }


    virtual void on_mouse_button_up(int x, int y, unsigned flags)
    {
        if(m_poly1.on_mouse_button_up(x, y))
        {
            force_redraw();
        }
        if(m_poly2.on_mouse_button_up(x, y))
        {
            force_redraw();
        }
    }


    virtual void on_key(int x, int y, unsigned key, unsigned flags)
    {
    }


    virtual void on_ctrl_change()
    {
        if(m_animate.status() != m_prev_animate)
        {
            if(m_animate.status())
            {
                on_init();
                int i;
                for(i = 0; i < 6; i++)
                {
                    m_dx1[i] = ((rand() % 1000) - 500) * 0.01;
                    m_dy1[i] = ((rand() % 1000) - 500) * 0.01;
                    m_dx2[i] = ((rand() % 1000) - 500) * 0.01;
                    m_dy2[i] = ((rand() % 1000) - 500) * 0.01;
                }
                wait_mode(false);
            }
            else
            {
                wait_mode(true);
            }
            m_prev_animate = m_animate.status();
        }
    }


    void move_point(double& x, double& y, double& dx, double& dy)
    {
        if(x < 0.0)      { x = 0.0;      dx = -dx; }
        if(x > width())  { x = width();  dx = -dx; }
        if(y < 0.0)      { y = 0.0;      dy = -dy; }
        if(y > height()) { y = height(); dy = -dy; }
        x += dx;
        y += dy;
    }


    void normalize_point(unsigned i)
    {
        double d = agg::calc_distance(m_poly1.xn(i), m_poly1.yn(i), 
                                      m_poly2.xn(i), m_poly2.yn(i));
        // 28.8 is 20 * sqrt(2)
        if(d > 28.28)
        {
            m_poly2.xn(i) = m_poly1.xn(i) + (m_poly2.xn(i) - m_poly1.xn(i)) * 28.28 / d;
            m_poly2.yn(i) = m_poly1.yn(i) + (m_poly2.yn(i) - m_poly1.yn(i)) * 28.28 / d;
        }
    }



    virtual void on_idle()
    {
        int i;
        for(i = 0; i < 6; i++)
        {
            move_point(m_poly1.xn(i), m_poly1.yn(i), m_dx1[i], m_dy1[i]);
            move_point(m_poly2.xn(i), m_poly2.yn(i), m_dx2[i], m_dy2[i]);
            normalize_point(i);
        }
        force_redraw();
    }

};






int agg_main(int argc, char* argv[])
{
    HDC dc = ::GetDC(0);
    the_application app(dc, pix_format, flip_y);
    app.caption("AGG Example. Non-linear \"Along-A-Curve\" Transformer");

    if(app.init(600, 600, agg::window_resize))
    {
        return app.run();
    }
    ::ReleaseDC(0, dc);
    return 1;
}










