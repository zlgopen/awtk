#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include "agg_basics.h"
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_renderer_outline_aa.h"
#include "agg_rasterizer_outline_aa.h"
#include "agg_scanline_p.h"
#include "agg_renderer_scanline.h"
#include "agg_path_storage.h"
#include "agg_conv_transform.h"
#include "agg_bounding_rect.h"
#include "ctrl/agg_slider_ctrl.h"
#include "ctrl/agg_cbox_ctrl.h"
#include "platform/agg_platform_support.h"

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
agg::scanline_p8  g_scanline;
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


class the_application : public agg::platform_support
{
    agg::slider_ctrl<agg::rgba8> m_width_slider;
    agg::cbox_ctrl<agg::rgba8>   m_scanline;

public:
    the_application(agg::pix_format_e format, bool flip_y) :
        agg::platform_support(format, flip_y),
        m_width_slider(5, 5, 150, 12, !flip_y),
        m_scanline(160, 5, "Use Scanline Rasterizer", !flip_y)
    {
        parse_lion();
        add_ctrl(m_width_slider);
        m_width_slider.no_transform();
        m_width_slider.range(0.0, 4.0);
        m_width_slider.value(1.0);
        m_width_slider.label("Width %3.2f");

        add_ctrl(m_scanline);
        m_scanline.no_transform();
    }


    virtual void on_draw()
    {
        int width = rbuf_window().width();
        int height = rbuf_window().height();

        typedef agg::renderer_base<pixfmt> renderer_base;
        typedef agg::renderer_scanline_aa_solid<renderer_base> renderer_solid;

        pixfmt pixf(rbuf_window());
        renderer_base rb(pixf);
        renderer_solid r(rb);
        rb.clear(agg::rgba(1,1,1));

        agg::trans_affine mtx;
        mtx *= agg::trans_affine_translation(-g_base_dx, -g_base_dy);
        mtx *= agg::trans_affine_scaling(g_scale, g_scale);
        mtx *= agg::trans_affine_rotation(g_angle + agg::pi);
        mtx *= agg::trans_affine_skewing(g_skew_x/1000.0, g_skew_y/1000.0);
        mtx *= agg::trans_affine_translation(width/2, height/2);

        if(m_scanline.status())
        {
            agg::conv_stroke<agg::path_storage> stroke(g_path);
            stroke.width(m_width_slider.value());
            stroke.line_join(agg::round_join);
            agg::conv_transform<agg::conv_stroke<agg::path_storage> > trans(stroke, mtx);
            agg::render_all_paths(g_rasterizer, g_scanline, r, trans, g_colors, g_path_idx, g_npaths);
        }
        else
        {
            typedef agg::renderer_outline_aa<renderer_base> renderer_type;
            typedef agg::rasterizer_outline_aa<renderer_type> rasterizer_type;

            double w = m_width_slider.value() * mtx.scale();

            agg::line_profile_aa profile(w, agg::gamma_none());
            renderer_type ren(rb, profile);
            rasterizer_type ras(ren);

            agg::conv_transform<agg::path_storage> trans(g_path, mtx);

            ras.render_all_paths(trans, g_colors, g_path_idx, g_npaths);
        }


        agg::render_ctrl(g_rasterizer, g_scanline, rb, m_width_slider);
        agg::render_ctrl(g_rasterizer, g_scanline, rb, m_scanline);
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
    app.caption("AGG Example. Lion");

    if(app.init(512, 512, agg::window_resize))
    {
        return app.run();
    }
    return 1;
}






