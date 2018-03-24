#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_p.h"
#include "agg_renderer_scanline.h"
#include "agg_conv_bspline.h"
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
//#define AGG_RGB565
//#define AGG_RGB555
#include "pixel_formats.h"

enum flip_y_e { flip_y = true };






class the_application : public agg::platform_support
{
public:
    typedef agg::renderer_base<pixfmt> renderer_base;
    typedef agg::scanline_p8 scanline_type;

    agg::interactive_polygon     m_poly;
    agg::slider_ctrl<agg::rgba8> m_num_points;
    agg::cbox_ctrl<agg::rgba8>   m_close;
    int                          m_flip;

    the_application(agg::pix_format_e format, bool flip_y) :
        agg::platform_support(format, flip_y),
        m_poly(6, 5.0),
        m_num_points(5.0, 5.0, 340.0, 12.0, !flip_y),
        m_close     (350, 5.0,  "Close", !flip_y),
        m_flip(0)
    {
        add_ctrl(m_close);
        m_num_points.range(1.0, 40.0);
        m_num_points.value(20.0);
        m_num_points.label("Number of intermediate Points = %.3f");
        add_ctrl(m_num_points);
    }


    virtual void on_init()
    {
        if(m_flip)
        {
            m_poly.xn(0) = 100;
            m_poly.yn(0) = height() - 100;
            m_poly.xn(1) = width() - 100;
            m_poly.yn(1) = height() - 100;
            m_poly.xn(2) = width() - 100;
            m_poly.yn(2) = 100;
            m_poly.xn(3) = 100;
            m_poly.yn(3) = 100;
        }
        else
        {
            m_poly.xn(0) = 100;
            m_poly.yn(0) = 100;
            m_poly.xn(1) = width() - 100;
            m_poly.yn(1) = 100;
            m_poly.xn(2) = width() - 100;
            m_poly.yn(2) = height() - 100;
            m_poly.xn(3) = 100;
            m_poly.yn(3) = height() - 100;
        }
        m_poly.xn(4) = width() / 2;
        m_poly.yn(4) = height() / 2;
        m_poly.xn(5) = width() / 2;
        m_poly.yn(5) = height() / 3;

    }




    virtual void on_draw()
    {
        pixfmt pixf(rbuf_window());
        renderer_base rb(pixf);
        rb.clear(agg::rgba(1, 1, 1));

        scanline_type sl;
        agg::rasterizer_scanline_aa<> ras;

        agg::simple_polygon_vertex_source path(m_poly.polygon(), 
                                               m_poly.num_points(), 
                                               false, 
                                               m_close.status());

        typedef agg::conv_bspline<agg::simple_polygon_vertex_source> conv_bspline_type;
        conv_bspline_type bspline(path);
        bspline.interpolation_step(1.0 / m_num_points.value());

        typedef agg::conv_stroke<conv_bspline_type> conv_stroke_type;
        conv_stroke_type stroke(bspline);

        stroke.width(2.0);

        ras.add_path(stroke);
        agg::render_scanlines_aa_solid(ras, sl, rb, agg::rgba(0, 0, 0));


        //--------------------------
        // Render the "poly" tool and controls
        ras.add_path(m_poly);
        agg::render_scanlines_aa_solid(ras, sl, rb, agg::rgba(0, 0.3, 0.5, 0.6));

        agg::render_ctrl(ras, sl, rb, m_close);
        agg::render_ctrl(ras, sl, rb, m_num_points);
        //--------------------------

    }



    virtual void on_mouse_button_down(int x, int y, unsigned flags)
    {
        if(flags & agg::mouse_left)
        {
            if(m_poly.on_mouse_button_down(x, y))
            {
                force_redraw();
            }
        }
    }


    virtual void on_mouse_move(int x, int y, unsigned flags)
    {
        if(flags & agg::mouse_left)
        {
            if(m_poly.on_mouse_move(x, y))
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
        if(m_poly.on_mouse_button_up(x, y))
        {
            force_redraw();
        }
    }


    virtual void on_key(int x, int y, unsigned key, unsigned flags)
    {
        if(key == ' ')
        {
            m_flip ^= 1;
            on_init();
            force_redraw();
        }
    }

};






int agg_main(int argc, char* argv[])
{
    the_application app(pix_format, flip_y);
    app.caption("AGG Example. BSpline Interpolator");

    if(app.init(600, 600, agg::window_resize))
    {
        return app.run();
    }
    return 1;
}










