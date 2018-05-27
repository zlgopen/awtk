#include <stdio.h>
#include "agg_basics.h"
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_p.h"
#include "agg_renderer_scanline.h"
#include "agg_pixfmt_rgb.h"
#include "agg_ellipse.h"
#include "agg_rounded_rect.h"
#include "agg_conv_stroke.h"
#include "platform/agg_platform_support.h"
#include "ctrl/agg_slider_ctrl.h"
#include "ctrl/agg_cbox_ctrl.h"

#define AGG_BGR24
#include "pixel_formats.h"

enum flip_y_e { flip_y = true };


class the_application : public agg::platform_support
{
    double m_x[2];
    double m_y[2];
    double m_dx;
    double m_dy;
    int    m_idx;
    agg::slider_ctrl<color_type> m_radius;
    agg::slider_ctrl<color_type> m_offset;
    agg::cbox_ctrl<color_type>   m_white_on_black;


public:
    the_application(agg::pix_format_e format, bool flip_y) :
        agg::platform_support(format, flip_y),
        m_idx(-1),
        m_radius(10, 10, 600-10,   19,    !flip_y),
        m_offset(10, 10+20, 600-10, 19+20, !flip_y),
        m_white_on_black(10, 10+40, "White on black")
    {
        m_x[0] = 100;   m_y[0] = 100;
        m_x[1] = 500;   m_y[1] = 350;
        add_ctrl(m_radius);
        add_ctrl(m_offset);
        add_ctrl(m_white_on_black);

        m_radius.label("radius=%4.3f");
        m_radius.range(0.0, 50.0);
        m_radius.value(25.0);

        m_offset.label("subpixel offset=%4.3f");
        m_offset.range(-2.0, 3.0);

        m_white_on_black.text_color(agg::srgba8(127, 127, 127));
        m_white_on_black.inactive_color(agg::srgba8(127, 127, 127));
    }


    virtual void on_draw()
    {
        typedef agg::renderer_base<pixfmt> renderer_base;
        typedef agg::renderer_scanline_aa_solid<renderer_base> renderer_solid;

        pixfmt pixf(rbuf_window());
        renderer_base rb(pixf);
        renderer_solid ren(rb);

        rb.clear(m_white_on_black.status() ? agg::rgba(0,0,0) : agg::rgba(1,1,1));

        agg::rasterizer_scanline_aa<> ras;
        agg::scanline_p8 sl;

        agg::ellipse e;

        // Render two "control" circles
        ren.color(agg::srgba8(127,127,127));
        e.init(m_x[0], m_y[0], 3, 3, 16);
        ras.add_path(e);
        agg::render_scanlines(ras, sl, ren);
        e.init(m_x[1], m_y[1], 3, 3, 16);
        ras.add_path(e);
        agg::render_scanlines(ras, sl, ren);

        double d = m_offset.value();

        // Creating a rounded rectangle
        agg::rounded_rect r(m_x[0]+d, m_y[0]+d, m_x[1]+d, m_y[1]+d, m_radius.value());
        r.normalize_radius();

        // Drawing as an outline
        agg::conv_stroke<agg::rounded_rect> p(r);
        p.width(1.0);
        ras.add_path(p);
        ren.color(m_white_on_black.status() ? agg::rgba(1,1,1) : agg::rgba(0,0,0));
        agg::render_scanlines(ras, sl, ren);

        // Render the controls
        agg::render_ctrl(ras, sl, rb, m_radius);
        agg::render_ctrl(ras, sl, rb, m_offset);
        agg::render_ctrl(ras, sl, rb, m_white_on_black);
    }


    virtual void on_mouse_button_down(int x, int y, unsigned flags)
    {
        if(flags & agg::mouse_left)
        {
            unsigned i;
            for (i = 0; i < 2; i++)
            {
                if(sqrt( (x-m_x[i]) * (x-m_x[i]) + (y-m_y[i]) * (y-m_y[i]) ) < 5.0)
                {
                    m_dx = x - m_x[i];
                    m_dy = y - m_y[i];
                    m_idx = i;
                    break;
                }
            }
        }
    }


    virtual void on_mouse_move(int x, int y, unsigned flags)
    {
        if(flags & agg::mouse_left)
        {
            if(m_idx >= 0)
            {
                m_x[m_idx] = x - m_dx;
                m_y[m_idx] = y - m_dy;
                force_redraw();
            }
        }
        else
        {
            on_mouse_button_up(x, y, flags);
        }
    }

    virtual void on_mouse_button_up(int x, int y, unsigned flags)
    {
        m_idx = -1;
    }


};


int agg_main(int argc, char* argv[])
{
    the_application app(pix_format, flip_y);
    app.caption("AGG Example. Rounded rectangle");

    if(app.init(600, 400, agg::window_resize))
    {
        return app.run();
    }
    return 1;
}


