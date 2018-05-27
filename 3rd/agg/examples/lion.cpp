#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include "agg_basics.h"
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_p.h"
#include "agg_renderer_scanline.h"
#include "agg_path_storage.h"
#include "agg_conv_transform.h"
#include "agg_bounding_rect.h"
#include "ctrl/agg_slider_ctrl.h"
#include "platform/agg_platform_support.h"

//#define AGG_GRAY16
//#define AGG_GRAY32
#define AGG_BGR24
//#define AGG_BGR48 
//#define AGG_RGB_AAA
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

agg::rasterizer_scanline_aa<> g_rasterizer;
agg::scanline_p8  g_scanline;
agg::path_storage g_path;
agg::srgba8        g_colors[100];
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

unsigned parse_lion(agg::path_storage& ps, agg::srgba8* colors, unsigned* path_idx);
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
    agg::slider_ctrl<color_type> m_alpha_slider;

public:
    typedef agg::renderer_base<pixfmt> renderer_base;
    typedef agg::renderer_scanline_aa_solid<renderer_base> renderer_solid;

    the_application(agg::pix_format_e format, bool flip_y) :
        agg::platform_support(format, flip_y),
        m_alpha_slider(5, 5, 512-5, 12, !flip_y)
    {
        parse_lion();
        add_ctrl(m_alpha_slider);
        m_alpha_slider.no_transform();
        m_alpha_slider.label("Alpha%3.3f");
        m_alpha_slider.value(0.1);
    }

    virtual void on_resize(int cx, int cy)
    {
        pixfmt pf(rbuf_window());
        renderer_base r(pf);
        r.clear(agg::rgba(1, 1, 1));
    }

    virtual void on_draw()
    {
        int width = rbuf_window().width();
        int height = rbuf_window().height();

        unsigned i;
        for(i = 0; i < g_npaths; i++)
        {
            g_colors[i].a = agg::int8u(m_alpha_slider.value() * 255);
        }

        pixfmt pixf(rbuf_window());
        renderer_base rb(pixf);
        renderer_solid r(rb);

        agg::trans_affine mtx;
        mtx *= agg::trans_affine_translation(-g_base_dx, -g_base_dy);
        mtx *= agg::trans_affine_scaling(g_scale, g_scale);
        mtx *= agg::trans_affine_rotation(g_angle + agg::pi);
        mtx *= agg::trans_affine_skewing(g_skew_x/1000.0, g_skew_y/1000.0);
        mtx *= agg::trans_affine_translation(width/2, height/2);

        // This code renders the lion:
        agg::conv_transform<agg::path_storage, agg::trans_affine> trans(g_path, mtx);
        agg::render_all_paths(g_rasterizer, g_scanline, r, trans, g_colors, g_path_idx, g_npaths);

        // Render the control
        agg::render_ctrl(g_rasterizer, g_scanline, rb, m_alpha_slider);
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

    if(app.init(512, 400, agg::window_resize))
    {
        return app.run();
    }
    return 1;
}






