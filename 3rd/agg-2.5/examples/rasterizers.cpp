#include <math.h>
#include <stdio.h>
#include <time.h>
#include "agg_basics.h"
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_rasterizer_outline.h"
#include "agg_scanline_p.h"
#include "agg_scanline_bin.h"
#include "agg_renderer_scanline.h"
#include "agg_renderer_primitives.h"
#include "ctrl/agg_slider_ctrl.h"
#include "ctrl/agg_cbox_ctrl.h"
#include "platform/agg_platform_support.h"

#define AGG_BGR24
#include "pixel_formats.h"


enum flip_y_e { flip_y = true };



class the_application : public agg::platform_support
{
    double m_x[3];
    double m_y[3];
    double m_dx;
    double m_dy;
    int    m_idx;
    agg::slider_ctrl<agg::rgba8>  m_gamma;
    agg::slider_ctrl<agg::rgba8>  m_alpha;
    agg::cbox_ctrl<agg::rgba8>    m_test;
    agg::rasterizer_scanline_aa<> m_ras;
    agg::scanline_p8              m_sl_p8;
    agg::scanline_bin             m_sl_bin;


public:
    the_application(agg::pix_format_e format, bool flip_y) :
        agg::platform_support(format, flip_y),
        m_idx(-1),
        m_gamma(130 + 10.0, 10.0 + 4.0, 130 + 150.0, 10.0 + 8.0 + 4.0, !flip_y),
        m_alpha(130 + 150.0 + 10.0, 10.0 + 4.0, 500 - 10.0, 10.0 + 8.0 + 4.0, !flip_y),
        m_test(130 + 10.0, 10.0 + 4.0 + 16.0,    "Test Performance", !flip_y)
    {
        m_x[0] = 100 + 120; m_y[0] = 60;
        m_x[1] = 369 + 120; m_y[1] = 170;
        m_x[2] = 143 + 120; m_y[2] = 310;

        add_ctrl(m_gamma);
        m_gamma.range(0.0, 1.0);
        m_gamma.value(0.5);
        m_gamma.label("Gamma=%1.2f");
        m_gamma.no_transform();

        add_ctrl(m_alpha);
        m_alpha.range(0.0, 1.0);
        m_alpha.value(1.0);
        m_alpha.label("Alpha=%1.2f");
        m_alpha.no_transform();

        add_ctrl(m_test);
        m_test.no_transform();
    }


    typedef agg::renderer_base<pixfmt> renderer_base;
    typedef agg::renderer_scanline_aa_solid<renderer_base> renderer_aa;
    typedef agg::renderer_scanline_bin_solid<renderer_base> renderer_bin;

    
    void draw_anti_aliased()
    {
        pixfmt pixf(rbuf_window());
        renderer_base rb(pixf);
        renderer_aa ren_aa(rb);

        agg::path_storage path;

        path.move_to(m_x[0], m_y[0]);
        path.line_to(m_x[1], m_y[1]);
        path.line_to(m_x[2], m_y[2]);
        path.close_polygon();

        ren_aa.color(agg::rgba(0.7, 0.5, 0.1, m_alpha.value()));

        m_ras.gamma(agg::gamma_power(m_gamma.value() * 2.0));
        m_ras.add_path(path);
        agg::render_scanlines(m_ras, m_sl_p8, ren_aa);
    }


    void draw_aliased()
    {
        pixfmt pixf(rbuf_window());
        renderer_base rb(pixf);
        renderer_bin ren_bin(rb);

        agg::path_storage path;
        

        path.move_to(m_x[0] - 200, m_y[0]);
        path.line_to(m_x[1] - 200, m_y[1]);
        path.line_to(m_x[2] - 200, m_y[2]);
        path.close_polygon();

        ren_bin.color(agg::rgba(0.1, 0.5, 0.7, m_alpha.value()));

        m_ras.gamma(agg::gamma_threshold(m_gamma.value()));
        m_ras.add_path(path);
        agg::render_scanlines(m_ras, m_sl_bin, ren_bin);

        //-- Drawing an outline with subpixel accuracy (aliased)
        //typedef agg::renderer_primitives<renderer_base> renderer_pr;
        //renderer_pr ren_pr(rb);
        //agg::rasterizer_outline<renderer_pr> ras_line(ren_pr);
        //ren_pr.line_color(agg::rgba(0.0, 0.0, 0.0));
        //ras_line.add_path(path);
    }

    virtual void on_draw()
    {
        pixfmt pixf(rbuf_window());
        renderer_base rb(pixf);
        renderer_aa ren_aa(rb);

        rb.clear(agg::rgba(1, 1, 1));

        agg::rasterizer_scanline_aa<> ras_aa;

        draw_anti_aliased();
        draw_aliased();

        agg::render_ctrl(ras_aa, m_sl_p8, rb, m_gamma);
        agg::render_ctrl(ras_aa, m_sl_p8, rb, m_alpha);
        agg::render_ctrl(ras_aa, m_sl_p8, rb, m_test);
    }


    virtual void on_mouse_button_down(int x, int y, unsigned flags)
    {
        if(flags & agg::mouse_left)
        {
            unsigned i;
            for (i = 0; i < 3; i++)
            {
                if(sqrt( (x-m_x[i])     * (x-m_x[i])     + (y-m_y[i]) * (y-m_y[i]) ) < 20.0 ||
                   sqrt( (x-m_x[i]+200) * (x-m_x[i]+200) + (y-m_y[i]) * (y-m_y[i]) ) < 20)
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
                                          x, y) ||
                   agg::point_in_triangle(m_x[0] - 200, m_y[0], 
                                          m_x[1] - 200, m_y[1],
                                          m_x[2] - 200, m_y[2],
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


    virtual void on_ctrl_change()
    {
        if(m_test.status())
        {
            on_draw();
            update_window();
            m_test.status(false);

            start_timer();
            int i;
            for(i = 0; i < 1000; i++)
            {
                draw_aliased();
            }
            double t1 = elapsed_time();

            start_timer();
            for(i = 0; i < 1000; i++)
            {
                draw_anti_aliased();
            }
            double t2 = elapsed_time();

            update_window();
            char buf[100];
            sprintf(buf, "Time Aliased=%.2fms Time Anti-Aliased=%.2fms", t1, t2);
            message(buf);
        }
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

    if(app.init(500, 330, agg::window_resize))
    {
        return app.run();
    }
    return 1;
}


