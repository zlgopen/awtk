#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include "agg_basics.h"
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_u.h"
#include "agg_color_gray.h"
#include "agg_renderer_mclip.h"
#include "agg_renderer_scanline.h"
#include "agg_path_storage.h"
#include "agg_conv_transform.h"
#include "agg_bounding_rect.h"
#include "agg_renderer_outline_aa.h"
#include "agg_renderer_primitives.h"
#include "agg_renderer_markers.h"
#include "agg_span_allocator.h"
#include "agg_span_gradient.h"
#include "agg_span_interpolator_linear.h"
#include "agg_rasterizer_outline_aa.h"
#include "agg_ellipse.h"
#include "ctrl/agg_slider_ctrl.h"
#include "platform/agg_platform_support.h"


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

agg::rasterizer_scanline_aa<> g_rasterizer;
agg::scanline_u8  g_scanline;
agg::path_storage g_path;
agg::rgba8        g_colors[100];
unsigned          g_path_idx[100];
unsigned          g_npaths = 0;
double            g_x1 = 0;
double            g_y1 = 0;
double            g_x2 = 0;
double            g_y2 = 0;
double            g_base_dx = 0;
double            g_base_dy = 0;
double            g_angle = 0;
double            g_scale = 1.0;
double            g_skew_x = 0;
double            g_skew_y = 0;
int               g_nclick = 0;


unsigned parse_lion(agg::path_storage& ps, agg::rgba8* colors, unsigned* path_idx);
void parse_lion()
{
    g_npaths = parse_lion(g_path, g_colors, g_path_idx);
    agg::pod_array_adaptor<unsigned> path_idx(g_path_idx, 100);
    agg::bounding_rect(g_path, path_idx, 0, g_npaths, &g_x1, &g_y1, &g_x2, &g_y2);
    g_base_dx = (g_x2 - g_x1) / 2.0;
    g_base_dy = (g_y2 - g_y1) / 2.0;
}


namespace agg
{
    // Specializations of the gradient_linear_color for rgba8 and gray8
    // color types. Only for the sake of performance.

    //========================================================================
    template<> struct gradient_linear_color<rgba8>
    {
        typedef rgba8 color_type;

        gradient_linear_color() {}
        gradient_linear_color(const color_type& c1, const color_type& c2) :
            m_c1(c1), m_c2(c2) {}

        static unsigned size() { return 256; }
        color_type operator [] (unsigned v) const 
        {
            color_type c;
            c.r = (int8u)((((m_c2.r - m_c1.r) * int(v)) + (m_c1.r << 8)) >> 8);
            c.g = (int8u)((((m_c2.g - m_c1.g) * int(v)) + (m_c1.g << 8)) >> 8);
            c.b = (int8u)((((m_c2.b - m_c1.b) * int(v)) + (m_c1.b << 8)) >> 8);
            c.a = (int8u)((((m_c2.a - m_c1.a) * int(v)) + (m_c1.a << 8)) >> 8);
            return c;
        }

        void colors(const color_type& c1, const color_type& c2)
        {
            m_c1 = c1;
            m_c2 = c2;
        }

        color_type m_c1;
        color_type m_c2;
    };


    //========================================================================
    template<> struct gradient_linear_color<gray8>
    {
        typedef gray8 color_type;

        gradient_linear_color() {}
        gradient_linear_color(const color_type& c1, const color_type& c2) :
            m_c1(c1), m_c2(c2) {}

        static unsigned size() { return 256; }
        color_type operator [] (unsigned v) const 
        {
            color_type c;
            c.v = (int8u)((((m_c2.v - m_c1.v) * int(v)) + (m_c1.v << 8)) >> 8);
            c.a = (int8u)((((m_c2.a - m_c1.a) * int(v)) + (m_c1.a << 8)) >> 8);
            return c;
        }

        void colors(const color_type& c1, const color_type& c2)
        {
            m_c1 = c1;
            m_c2 = c2;
        }

        color_type m_c1;
        color_type m_c2;
    };

}




class the_application : public agg::platform_support
{
    agg::slider_ctrl<agg::rgba8> m_num_cb;

public:
    the_application(agg::pix_format_e format, bool flip_y) :
        agg::platform_support(format, flip_y),
        m_num_cb(5, 5, 150, 12, !flip_y)
    {
        parse_lion();
        add_ctrl(m_num_cb);
        m_num_cb.range(2, 10);
        //m_num_cb.num_steps(8);
        m_num_cb.label("N=%.2f");
        m_num_cb.no_transform();
    }

    virtual void on_draw()
    {
        unsigned i;
        int width = rbuf_window().width();
        int height = rbuf_window().height();

        pixfmt pf(rbuf_window());
        typedef agg::renderer_mclip<pixfmt> base_ren_type;
        
        base_ren_type r(pf);
        agg::renderer_scanline_aa_solid<base_ren_type> rs(r);

        agg::trans_affine mtx;
        mtx *= agg::trans_affine_translation(-g_base_dx, -g_base_dy);
        mtx *= agg::trans_affine_scaling(g_scale, g_scale);
        mtx *= agg::trans_affine_rotation(g_angle + agg::pi);
        mtx *= agg::trans_affine_skewing(g_skew_x/1000.0, g_skew_y/1000.0);
        mtx *= agg::trans_affine_translation(width/2, height/2);

        r.clear(agg::rgba(1, 1, 1));
        

        r.reset_clipping(false);  // Visibility: "false" means "no visible regions"
        int x, y;
        double n = m_num_cb.value();
        for(x = 0; x < n; x++)
        {
            for(y = 0; y < n; y++)
            {
                int x1 = int(width  * x / n);
                int y1 = int(height * y / n);
                int x2 = int(width  * (x + 1) / n);
                int y2 = int(height * (y + 1) / n);
                r.add_clip_box(x1 + 5, y1 + 5, x2 - 5, y2 - 5);
            }
        }



        // Render the lion
        agg::conv_transform<agg::path_storage, agg::trans_affine> trans(g_path, mtx);
        agg::render_all_paths(g_rasterizer, g_scanline, rs, trans, g_colors, g_path_idx, g_npaths);


// The scanline rasterizer allows you to perform clipping to multiple
// regions "manually", like in the following code, but the "embedded" method 
// shows much better performance.
// 
//for(i = 0; i < g_npaths; i++)
//{
//    g_rasterizer.reset();
//    g_rasterizer.add_path(trans, g_path_idx[i]);
//    rs.color(g_colors[i]);
//
//    int x, y;
//    double n = m_num_cb.value();
//    for(x = 0; x < n; x++)
//    {
//        for(y = 0; y < n; y++)
//        {
//            int x1 = int(width  * x / n);
//            int y1 = int(height * y / n);
//            int x2 = int(width  * (x + 1) / n);
//            int y2 = int(height * (y + 1) / n);
//            // r should be of type renderer_base<>
//            r.clip_box(agg::rect_i(x1 + 5, y1 + 5, x2 - 5, y2 - 5)); 
//            agg::render_scanlines(g_rasterizer, g_scanline, rs);
//        }
//    }
//}



        // Render random Bresenham lines and markers
        agg::renderer_markers<base_ren_type> m(r);
        for(i = 0; i < 50; i++)
        {
            m.line_color(agg::rgba8(rand() & 0x7F, 
                                    rand() & 0x7F, 
                                    rand() & 0x7F, 
                                    (rand() & 0x7F) + 0x7F)); 
            m.fill_color(agg::rgba8(rand() & 0x7F, 
                                    rand() & 0x7F, 
                                    rand() & 0x7F, 
                                    (rand() & 0x7F) + 0x7F));

            m.line(m.coord(rand() % width), m.coord(rand() % height), 
                   m.coord(rand() % width), m.coord(rand() % height));

            m.marker(rand() % width, rand() % height, rand() % 10 + 5,
                     agg::marker_e(rand() % agg::end_of_markers));
        }


        // Render random anti-aliased lines
        double w = 5.0;
        agg::line_profile_aa profile;
        profile.width(w);

        typedef agg::renderer_outline_aa<base_ren_type> renderer_type;
        renderer_type ren(r, profile);

        typedef agg::rasterizer_outline_aa<renderer_type> rasterizer_type;
        rasterizer_type ras(ren);
        ras.round_cap(true);

        for(i = 0; i < 50; i++)
        {
            ren.color(agg::rgba8(rand() & 0x7F, 
                                 rand() & 0x7F, 
                                 rand() & 0x7F, 
                                 (rand() & 0x7F) + 0x7F)); 
            ras.move_to_d(rand() % width, rand() % height);
            ras.line_to_d(rand() % width, rand() % height);
            ras.render(false);
        }


        // Render random circles with gradient
        typedef agg::gradient_linear_color<color_type> grad_color;
        typedef agg::gradient_circle grad_func;
        typedef agg::span_interpolator_linear<> interpolator_type;
        typedef agg::span_gradient<color_type, 
                                  interpolator_type, 
                                  grad_func, 
                                  grad_color> span_grad_type;

        agg::trans_affine grm;
        grad_func grf;
        grad_color grc(agg::rgba8(0,0,0), agg::rgba8(0,0,0));
        agg::ellipse ell;
        agg::span_allocator<color_type> sa;
        interpolator_type inter(grm);
        span_grad_type sg(inter, grf, grc, 0, 10);
        for(i = 0; i < 50; i++)
        {
            x = rand() % width;
            y = rand() % height;
            double radius = rand() % 10 + 5;
            grm.reset();
            grm *= agg::trans_affine_scaling(radius / 10.0);
            grm *= agg::trans_affine_translation(x, y);
            grm.invert();
            grc.colors(agg::rgba8(255, 255, 255, 0),
                       agg::rgba8(rand() & 0x7F, 
                                  rand() & 0x7F, 
                                  rand() & 0x7F, 
                                  255));
            sg.color_function(grc);
            ell.init(x, y, radius, radius, 32);
            g_rasterizer.add_path(ell);
            agg::render_scanlines_aa(g_rasterizer, g_scanline, r, sa, sg);
        }

        r.reset_clipping(true); // "true" means "all rendering buffer is visible".
        agg::render_ctrl(g_rasterizer, g_scanline, r, m_num_cb);
    }


    void transform(double width, double height, double x, double y)
    {
        x -= width / 2;
        y -= height / 2;
        g_angle = atan2(y, x);
        g_scale = sqrt(y * y + x * x) / 100.0;
    }


    virtual void on_mouse_button_down(int x, int y, unsigned flags)
    {
        if(flags & agg::mouse_left)
        {
            int width = rbuf_window().width();
            int height = rbuf_window().height();
            transform(width, height, x, y);
            force_redraw();
        }

        if(flags & agg::mouse_right)
        {
            g_skew_x = x;
            g_skew_y = y;
            force_redraw();
        }
    }


    virtual void on_mouse_move(int x, int y, unsigned flags)
    {
        on_mouse_button_down(x, y, flags);
    }

};






int agg_main(int argc, char* argv[])
{
    the_application app(pix_format, flip_y);
    app.caption("AGG Example. Clipping to multiple rectangle regions");

    if(app.init(512, 400, agg::window_resize))
    {
        return app.run();
    }
    return 1;
}






