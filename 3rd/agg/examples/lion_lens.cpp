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
#include "agg_trans_warp_magnifier.h"
#include "agg_conv_segmentator.h"
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
//#define AGG_BGR96
//#define AGG_BGRA128
//#define AGG_RGB565
//#define AGG_RGB555
//#define AGG_RGB_AAA
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
    agg::slider_ctrl<color_type> m_magn_slider;
    agg::slider_ctrl<color_type> m_radius_slider;

public:
    typedef agg::renderer_base<pixfmt> renderer_base;
    typedef agg::renderer_scanline_aa_solid<renderer_base> renderer_solid;

    the_application(agg::pix_format_e format, bool flip_y) :
        agg::platform_support(format, flip_y),
        m_magn_slider  (5,   5, 495,  12, !flip_y),
        m_radius_slider(5,  20, 495,  27, !flip_y)
    {
        parse_lion();
        add_ctrl(m_magn_slider);

        m_magn_slider.no_transform();
        m_magn_slider.range(0.01, 4.0);
        m_magn_slider.value(3.0);
        m_magn_slider.label("Scale=%3.2f");

        add_ctrl(m_radius_slider);
        m_radius_slider.no_transform();
        m_radius_slider.range(0.0, 100.0);
        m_radius_slider.value(70.0);
        m_radius_slider.label("Radius=%3.2f");

    }


    virtual void on_init()
    {
        g_x1 = 200;
        g_y1 = 150;
    }

    virtual void on_resize(int cx, int cy)
    {
    }

    virtual void on_draw()
    {
        pixfmt pixf(rbuf_window());
        renderer_base rb(pixf);
        renderer_solid r(rb);

        rb.clear(agg::rgba(1, 1, 1));

        agg::trans_warp_magnifier lens;
        lens.center(g_x1, g_y1);
        lens.magnification(m_magn_slider.value());
        lens.radius(m_radius_slider.value() / m_magn_slider.value());

        agg::conv_segmentator<agg::path_storage> segm(g_path);

        agg::trans_affine mtx;
        mtx *= agg::trans_affine_translation(-g_base_dx, -g_base_dy);
        mtx *= agg::trans_affine_rotation(g_angle + agg::pi);
        mtx *= agg::trans_affine_translation(width()/2, height()/2);

        agg::conv_transform<
            agg::conv_segmentator<
                agg::path_storage> > trans_mtx(segm, mtx);

        agg::conv_transform<
            agg::conv_transform<
                agg::conv_segmentator<
                    agg::path_storage> >, agg::trans_warp_magnifier> trans_lens(trans_mtx, lens);

        agg::render_all_paths(g_rasterizer, g_scanline, r, trans_lens, g_colors, g_path_idx, g_npaths);

        agg::render_ctrl(g_rasterizer, g_scanline, rb, m_magn_slider);
        agg::render_ctrl(g_rasterizer, g_scanline, rb, m_radius_slider);


        // Testing inverse_transform()
        //--------------------
        //double x, y;
        //for(y = 0; y < height(); y += 10)
        //{
        //    for(x = 0; x < height(); x += 10)
        //    {
        //        double x2 = x+0.5;
        //        double y2 = y+0.5;
        //        lens.transform(&x2, &y2);
        //        lens.inverse_transform(&x2, &y2);
        //        agg::ellipse e(x2, y2, 1, 1);
        //        g_rasterizer.add_path(e);
        //        r.color(agg::srgba8(0,0,0));
        //        agg::render_scanlines(g_rasterizer, g_scanline, r);
        //    }
        //}
    }



    virtual void on_mouse_button_down(int x, int y, unsigned flags)
    {
        if(flags & agg::mouse_left)
        {
            g_x1 = x;
            g_y1 = y;
            force_redraw();
        }
        if(flags & agg::mouse_right)
        {
            g_x2 = x;
            g_y2 = y;
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

    if(app.init(500, 600, agg::window_resize))
    {
        return app.run();
    }
    return 1;
}






