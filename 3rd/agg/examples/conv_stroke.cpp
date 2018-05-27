#include <math.h>
#include <stdio.h>
#include "agg_basics.h"
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_conv_stroke.h"
#include "agg_conv_dash.h"
#include "agg_conv_curve.h"
#include "agg_conv_contour.h"
#include "agg_conv_smooth_poly1.h"
#include "agg_conv_marker.h"
#include "agg_arrowhead.h"
#include "agg_vcgen_markers_term.h"
#include "agg_scanline_p.h"
#include "agg_renderer_scanline.h"
#include "agg_pixfmt_rgb.h"
#include "platform/agg_platform_support.h"
#include "ctrl/agg_slider_ctrl.h"
#include "ctrl/agg_rbox_ctrl.h"

#define AGG_BGR24
//#define AGG_BGR96
#include "pixel_formats.h"

enum flip_y_e { flip_y = true };



class the_application : public agg::platform_support
{
    double m_x[3];
    double m_y[3];
    double m_dx;
    double m_dy;
    int    m_idx;
    agg::rbox_ctrl<color_type> m_join;
    agg::rbox_ctrl<color_type> m_cap;
    agg::slider_ctrl<color_type> m_width;
    agg::slider_ctrl<color_type> m_miter_limit;


public:
    the_application(agg::pix_format_e format, bool flip_y) :
        agg::platform_support(format, flip_y),
        m_idx(-1),
        m_join(10.0, 10.0, 133.0, 80.0, !flip_y),
        m_cap(10.0, 80.0 + 10.0, 133.0, 80.0 + 80.0, !flip_y),
        m_width(130 + 10.0, 10.0 + 4.0, 500.0 - 10.0, 10.0 + 8.0 + 4.0, !flip_y),
        m_miter_limit(130 + 10.0, 20.0 + 10.0 + 4.0, 500.0 - 10.0, 20.0 + 10.0 + 8.0 + 4.0, !flip_y)
    {
        m_x[0] = 57  + 100; m_y[0] = 60;
        m_x[1] = 369 + 100; m_y[1] = 170;
        m_x[2] = 143 + 100; m_y[2] = 310;

        add_ctrl(m_join);
        m_join.text_size(7.5);
        m_join.text_thickness(1.0);
        m_join.add_item("Miter Join");
        m_join.add_item("Miter Join Revert");
        m_join.add_item("Round Join");
        m_join.add_item("Bevel Join");
        m_join.cur_item(2);

        add_ctrl(m_cap);
        m_cap.add_item("Butt Cap");
        m_cap.add_item("Square Cap");
        m_cap.add_item("Round Cap");
        m_cap.cur_item(2);

        add_ctrl(m_width);
        m_width.range(3.0, 40.0);
        m_width.value(20.0);
        m_width.label("Width=%1.2f");

        add_ctrl(m_miter_limit);
        m_miter_limit.range(1.0, 10.0);
        m_miter_limit.value(4.0);
        m_miter_limit.label("Miter Limit=%1.2f");

        m_join.no_transform();
        m_cap.no_transform();
        m_width.no_transform();
        m_miter_limit.no_transform();
    }

    virtual void on_init()
    {
    }

    virtual void on_draw()
    {
        typedef agg::renderer_base<pixfmt> ren_base;

        pixfmt pixf(rbuf_window());
        ren_base renb(pixf);
        renb.clear(agg::rgba(1, 1, 1));

        agg::rasterizer_scanline_aa<> ras;
        agg::scanline_p8 sl;

        agg::path_storage path;

        path.move_to(m_x[0], m_y[0]);
        path.line_to((m_x[0] + m_x[1]) / 2, (m_y[0] + m_y[1]) / 2); // This point is added only to check for numerical stability
        path.line_to(m_x[1], m_y[1]);
        path.line_to(m_x[2], m_y[2]);
        path.line_to(m_x[2], m_y[2]);                               // This point is added only to check for numerical stability

        path.move_to((m_x[0] + m_x[1]) / 2, (m_y[0] + m_y[1]) / 2);
        path.line_to((m_x[1] + m_x[2]) / 2, (m_y[1] + m_y[2]) / 2);
        path.line_to((m_x[2] + m_x[0]) / 2, (m_y[2] + m_y[0]) / 2);
        path.close_polygon();

        agg::line_cap_e           cap = agg::butt_cap;
        if(m_cap.cur_item() == 1) cap = agg::square_cap;
        if(m_cap.cur_item() == 2) cap = agg::round_cap;
        
        agg::line_join_e           join = agg::miter_join;
        if(m_join.cur_item() == 1) join = agg::miter_join_revert;
        if(m_join.cur_item() == 2) join = agg::round_join;
        if(m_join.cur_item() == 3) join = agg::bevel_join;



        // (1)
        agg::conv_stroke<agg::path_storage> stroke(path);
        stroke.line_join(join);
        stroke.line_cap(cap);
        stroke.miter_limit(m_miter_limit.value());
        stroke.width(m_width.value());
        ras.add_path(stroke);
        agg::render_scanlines_aa_solid(ras, sl, renb, agg::rgba(0.8, 0.7, 0.6));
        // (1)


        // (2)
        agg::conv_stroke<agg::path_storage> poly1(path);
        poly1.width(1.5);
        ras.add_path(poly1);
        agg::render_scanlines_aa_solid(ras, sl, renb, agg::rgba(0,0,0));
        // (2)



        // (3)
        agg::conv_dash<agg::conv_stroke<agg::path_storage> > poly2_dash(stroke);
        agg::conv_stroke<agg::conv_dash<agg::conv_stroke<agg::path_storage> > > poly2(poly2_dash);
        poly2.miter_limit(4.0);
        poly2.width(m_width.value() / 5.0);
        poly2.line_cap(cap);
        poly2.line_join(join);
        poly2_dash.add_dash(20.0, m_width.value() / 2.5);
        ras.add_path(poly2);
        agg::render_scanlines_aa_solid(ras, sl, renb, agg::rgba(0,0,0.3));
        // (3)



        // (4)
        ras.add_path(path);
        agg::render_scanlines_aa_solid(ras, sl, renb, agg::rgba(0.0, 0.0, 0.0, 0.2));
        // (4)



        agg::render_ctrl(ras, sl, renb, m_join);
        agg::render_ctrl(ras, sl, renb, m_cap);
        agg::render_ctrl(ras, sl, renb, m_width);
        agg::render_ctrl(ras, sl, renb, m_miter_limit);
    }


    virtual void on_mouse_button_down(int x, int y, unsigned flags)
    {
        if(flags & agg::mouse_left)
        {
            unsigned i;
            for (i = 0; i < 3; i++)
            {
                if(sqrt( (x-m_x[i]) * (x-m_x[i]) + (y-m_y[i]) * (y-m_y[i]) ) < 20.0)
                {
                    m_dx = x - m_x[i];
                    m_dy = y - m_y[i];
                    m_idx = i;
                    break;
                }
            }
            if(i == 3)
            {
                if(agg::point_in_triangle(m_x[0], m_y[0], 
                                          m_x[1], m_y[1],
                                          m_x[2], m_y[2],
                                          x, y))
                {
                    m_dx = x - m_x[0];
                    m_dy = y - m_y[0];
                    m_idx = 3;
                }

            }
        }
    }


    virtual void on_mouse_move(int x, int y, unsigned flags)
    {
        if(flags & agg::mouse_left)
        {
            if(m_idx == 3)
            {
                double dx = x - m_dx;
                double dy = y - m_dy;
                m_x[1] -= m_x[0] - dx;
                m_y[1] -= m_y[0] - dy;
                m_x[2] -= m_x[0] - dx;
                m_y[2] -= m_y[0] - dy;
                m_x[0] = dx;
                m_y[0] = dy;
                force_redraw();
                return;
            }

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

    
    virtual void on_key(int x, int y, unsigned key, unsigned flags)
    {
        double dx = 0;
        double dy = 0;
        switch(key)
        {
        case agg::key_left:  dx = -0.1; break;
        case agg::key_right: dx =  0.1; break;
        case agg::key_up:    dy =  0.1; break;
        case agg::key_down:  dy = -0.1; break;
        }
        m_x[0] += dx;
        m_y[0] += dy;
        m_x[1] += dx;
        m_y[1] += dy;
        force_redraw();
    }

};



int agg_main(int argc, char* argv[])
{
    the_application app(pix_format, flip_y);
    app.caption("AGG Example. Line Join");

    if(app.init(500, 330, 0))
    {
        return app.run();
    }
    return 1;
}


