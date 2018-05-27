#include <stdio.h>
#include "agg_basics.h"
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_u.h"
#include "agg_scanline_p.h"
#include "agg_renderer_scanline.h"
#include "agg_span_allocator.h"
#include "agg_span_gouraud_rgba.h"
#include "agg_span_gouraud_gray.h"
#include "agg_span_solid.h"
#include "platform/agg_platform_support.h"

#include "ctrl/agg_slider_ctrl.h"


//#define AGG_GRAY8
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


#include "agg_math.h"
#include "agg_dda_line.h"


class the_application : public agg::platform_support
{
    double m_x[3];
    double m_y[3];
    double m_dx;
    double m_dy;
    int    m_idx;

    agg::slider_ctrl<agg::rgba> m_dilation;
    agg::slider_ctrl<agg::rgba> m_gamma;
    agg::slider_ctrl<agg::rgba> m_alpha;


public:
    the_application(agg::pix_format_e format, bool flip_y) :
        agg::platform_support(format, flip_y),
        m_idx(-1),
        m_dilation(5, 5,    400-5, 11,    !flip_y),
        m_gamma   (5, 5+15, 400-5, 11+15, !flip_y),
        m_alpha   (5, 5+30, 400-5, 11+30, !flip_y)

    {
        m_x[0] = 57;    m_y[0] = 60;
        m_x[1] = 369;   m_y[1] = 170;
        m_x[2] = 143;   m_y[2] = 310;
       
        add_ctrl(m_dilation);
        add_ctrl(m_gamma);
        add_ctrl(m_alpha);

        m_dilation.label("Dilation=%3.2f");
        m_gamma.label("Linear gamma=%3.2f");
        m_alpha.label("Opacity=%3.2f");

        m_dilation.value(0.175);
        m_gamma.value(0.809);
        m_alpha.value(1.0);
    }


    template<class Scanline, class Ras> 
    void render_gouraud(Scanline& sl, Ras& ras)
    {
        double alpha = m_alpha.value();
        double brc = 1;

        typedef agg::renderer_base<pixfmt> base_ren_type;
#ifdef AGG_GRAY8
        typedef agg::span_gouraud_gray<color_type> span_gen_type;
#else
        typedef agg::span_gouraud_rgba<color_type> span_gen_type;
#endif
        typedef agg::span_allocator<color_type> span_alloc_type;
        
        pixfmt pf(rbuf_window());
        base_ren_type ren_base(pf);

        span_alloc_type span_alloc;
        span_gen_type   span_gen;

        ras.gamma(agg::gamma_linear(0.0, m_gamma.value()));

        double d = m_dilation.value();

        // Single triangle
        //span_gen.colors(agg::rgba(1,   0,   0,  alpha),
        //                agg::rgba(0,   1,   0,  alpha),
        //                agg::rgba(0,   0,   1,  alpha));
        //span_gen.triangle(m_x[0], m_y[0], m_x[1], m_y[1], m_x[2], m_y[2], d);
        //ras.add_path(span_gen);
        //agg::render_scanlines_aa(ras, sl, ren_base, span_alloc, span_gen);


        // Six triangles
        double xc = (m_x[0] + m_x[1] + m_x[2]) / 3.0;
        double yc = (m_y[0] + m_y[1] + m_y[2]) / 3.0;

        double x1 = (m_x[1] + m_x[0]) / 2 - (xc - (m_x[1] + m_x[0]) / 2);
        double y1 = (m_y[1] + m_y[0]) / 2 - (yc - (m_y[1] + m_y[0]) / 2);

        double x2 = (m_x[2] + m_x[1]) / 2 - (xc - (m_x[2] + m_x[1]) / 2);
        double y2 = (m_y[2] + m_y[1]) / 2 - (yc - (m_y[2] + m_y[1]) / 2);

        double x3 = (m_x[0] + m_x[2]) / 2 - (xc - (m_x[0] + m_x[2]) / 2);
        double y3 = (m_y[0] + m_y[2]) / 2 - (yc - (m_y[0] + m_y[2]) / 2);

        span_gen.colors(agg::rgba(1,   0,   0,    alpha),
                        agg::rgba(0,   1,   0,    alpha),
                        agg::rgba(brc, brc, brc,  alpha));
        span_gen.triangle(m_x[0], m_y[0], m_x[1], m_y[1], xc, yc, d);
        ras.add_path(span_gen);
        agg::render_scanlines_aa(ras, sl, ren_base, span_alloc, span_gen);


        span_gen.colors(agg::rgba(0,   1,   0,    alpha),
                        agg::rgba(0,   0,   1,    alpha),
                        agg::rgba(brc, brc, brc,  alpha));
        span_gen.triangle(m_x[1], m_y[1], m_x[2], m_y[2], xc, yc, d);
        ras.add_path(span_gen);
        agg::render_scanlines_aa(ras, sl, ren_base, span_alloc, span_gen);


        span_gen.colors(agg::rgba(0,   0,   1,   alpha),
                        agg::rgba(1,   0,   0,   alpha),
                        agg::rgba(brc, brc, brc, alpha));
        span_gen.triangle(m_x[2], m_y[2], m_x[0], m_y[0], xc, yc, d);
        ras.add_path(span_gen);
        agg::render_scanlines_aa(ras, sl, ren_base, span_alloc, span_gen);


        brc = 1-brc;
        span_gen.colors(agg::rgba(1,   0,   0,    alpha),
                        agg::rgba(0,   1,   0,    alpha),
                        agg::rgba(brc, brc, brc,  alpha));
        span_gen.triangle(m_x[0], m_y[0], m_x[1], m_y[1], x1, y1, d);
        ras.add_path(span_gen);
        agg::render_scanlines_aa(ras, sl, ren_base, span_alloc, span_gen);


        span_gen.colors(agg::rgba(0,   1,   0,    alpha),
                        agg::rgba(0,   0,   1,    alpha),
                        agg::rgba(brc, brc, brc,  alpha));
        span_gen.triangle(m_x[1], m_y[1], m_x[2], m_y[2], x2, y2, d);
        ras.add_path(span_gen);
        agg::render_scanlines_aa(ras, sl, ren_base, span_alloc, span_gen);


        span_gen.colors(agg::rgba(0,   0,   1,    alpha),
                        agg::rgba(1,   0,   0,    alpha),
                        agg::rgba(brc, brc, brc,  alpha));
        span_gen.triangle(m_x[2], m_y[2], m_x[0], m_y[0], x3, y3, d);
        ras.add_path(span_gen);
        agg::render_scanlines_aa(ras, sl, ren_base, span_alloc, span_gen);
    }








    virtual void on_draw()
    {
        typedef agg::renderer_base<pixfmt> base_ren_type;

        pixfmt pf(rbuf_window());
        base_ren_type ren_base(pf);
        ren_base.clear(agg::rgba(1,1,1));

        agg::scanline_u8 sl;
        agg::rasterizer_scanline_aa<> ras;

        render_gouraud(sl, ras);

        ras.gamma(agg::gamma_none());
        agg::render_ctrl(ras, sl, ren_base, m_dilation);
        agg::render_ctrl(ras, sl, ren_base, m_gamma);
        agg::render_ctrl(ras, sl, ren_base, m_alpha);
    }


    virtual void on_mouse_button_down(int x, int y, unsigned flags)
    {
        unsigned i;
        if(flags & agg::mouse_right)
        {
            agg::scanline_u8 sl;
            agg::rasterizer_scanline_aa<> ras;
            start_timer();
            for(i = 0; i < 100; i++)
            {
                render_gouraud(sl, ras);
            }
            char buf[100];
            sprintf(buf, "Time=%2.2f ms", elapsed_time());
            message(buf);
        }

        if(flags & agg::mouse_left)
        {
            for (i = 0; i < 3; i++)
            {
                if(sqrt( (x-m_x[i]) * (x-m_x[i]) + (y-m_y[i]) * (y-m_y[i]) ) < 10.0)
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
    app.caption("AGG Example. Gouraud Shading");

    if(app.init(400, 320, agg::window_resize))
    {
        return app.run();
    }
    return 1;
}


