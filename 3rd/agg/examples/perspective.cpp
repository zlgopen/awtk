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
#include "agg_conv_clip_polygon.h"
#include "agg_bounding_rect.h"
#include "agg_ellipse.h"
#include "agg_trans_bilinear.h"
#include "agg_trans_perspective.h"
#include "ctrl/agg_rbox_ctrl.h"
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
    g_path.flip_x(g_x1, g_x2);
    g_path.flip_y(g_y1, g_y2);
}




namespace agg
{
}







class the_application : public agg::platform_support
{
public:
    typedef agg::renderer_base<pixfmt> renderer_base;
    typedef agg::renderer_scanline_aa_solid<renderer_base> renderer_solid;

    agg::interactive_polygon   m_quad;
    agg::rbox_ctrl<color_type> m_trans_type;

    the_application(agg::pix_format_e format, bool flip_y) :
        agg::platform_support(format, flip_y),
        m_quad(4, 5.0),
        m_trans_type(420, 5.0, 420+130.0, 55.0, !flip_y)
    {
        parse_lion();
        m_quad.xn(0) = g_x1;
        m_quad.yn(0) = g_y1;
        m_quad.xn(1) = g_x2;
        m_quad.yn(1) = g_y1;
        m_quad.xn(2) = g_x2;
        m_quad.yn(2) = g_y2;
        m_quad.xn(3) = g_x1;
        m_quad.yn(3) = g_y2;

        m_trans_type.add_item("Bilinear");
        m_trans_type.add_item("Perspective");
        m_trans_type.cur_item(0);
        add_ctrl(m_trans_type);
    }


    virtual void on_init()
    {
        double dx = width()  / 2.0 - (m_quad.xn(1) - m_quad.xn(0)) / 2.0;
        double dy = height() / 2.0 - (m_quad.yn(2) - m_quad.yn(0)) / 2.0;
        m_quad.xn(0) += dx;
        m_quad.yn(0) += dy;
        m_quad.xn(1) += dx;
        m_quad.yn(1) += dy;
        m_quad.xn(2) += dx;
        m_quad.yn(2) += dy;
        m_quad.xn(3) += dx;
        m_quad.yn(3) += dy;
    }

    virtual void on_draw()
    {
        pixfmt pixf(rbuf_window());
        renderer_base rb(pixf);
        renderer_solid r(rb);
        rb.clear(agg::rgba(1, 1, 1));

        g_rasterizer.clip_box(0, 0, width(), height());

        if(m_trans_type.cur_item() == 0)
        {
            agg::trans_bilinear tr(g_x1, g_y1, g_x2, g_y2, m_quad.polygon());
            if(tr.is_valid())
            {
                //--------------------------
                // Render transformed lion
                //
                agg::conv_transform<agg::path_storage, 
                                    agg::trans_bilinear> trans(g_path, tr);

                agg::render_all_paths(g_rasterizer, g_scanline, r, trans, g_colors, g_path_idx, g_npaths);
                //--------------------------



                //--------------------------
                // Render transformed ellipse
                //
                agg::ellipse ell((g_x1 + g_x2) * 0.5, (g_y1 + g_y2) * 0.5, 
                                 (g_x2 - g_x1) * 0.5, (g_y2 - g_y1) * 0.5,
                                 200);
                agg::conv_stroke<agg::ellipse> ell_stroke(ell);
                ell_stroke.width(3.0);
                agg::conv_transform<agg::ellipse, 
                                    agg::trans_bilinear> trans_ell(ell, tr);

                agg::conv_transform<agg::conv_stroke<agg::ellipse>, 
                                    agg::trans_bilinear> trans_ell_stroke(ell_stroke, tr);

                g_rasterizer.add_path(trans_ell);
                r.color(agg::rgba(0.5, 0.3, 0.0, 0.3));
                agg::render_scanlines(g_rasterizer, g_scanline, r);

                g_rasterizer.add_path(trans_ell_stroke);
                r.color(agg::rgba(0.0, 0.3, 0.2, 1.0));
                agg::render_scanlines(g_rasterizer, g_scanline, r);
                //--------------------------

            }
        }
        else
        {
            agg::trans_perspective tr(g_x1, g_y1, g_x2, g_y2, m_quad.polygon());
            if(tr.is_valid())
            {

                //--------------------------
                // Render transformed lion
                //
                agg::conv_transform<agg::path_storage, 
                                    agg::trans_perspective> trans(g_path, tr);

                agg::render_all_paths(g_rasterizer, g_scanline, r, trans, g_colors, g_path_idx, g_npaths);

                //--------------------------



                //--------------------------
                // Render transformed ellipse
                //
                agg::ellipse ell((g_x1 + g_x2) * 0.5, (g_y1 + g_y2) * 0.5, 
                                 (g_x2 - g_x1) * 0.5, (g_y2 - g_y1) * 0.5,
                                 200);

                agg::conv_stroke<agg::ellipse> ell_stroke(ell);
                ell_stroke.width(3.0);
                agg::conv_transform<agg::ellipse, 
                                    agg::trans_perspective> trans_ell(ell, tr);


                agg::conv_transform<agg::conv_stroke<agg::ellipse>, 
                                    agg::trans_perspective> trans_ell_stroke(ell_stroke, tr);


                g_rasterizer.add_path(trans_ell);
                r.color(agg::rgba(0.5, 0.3, 0.0, 0.3));
                agg::render_scanlines(g_rasterizer, g_scanline, r);

                g_rasterizer.add_path(trans_ell_stroke);
                r.color(agg::rgba(0.0, 0.3, 0.2, 1.0));
                agg::render_scanlines(g_rasterizer, g_scanline, r);
                //--------------------------


                // Testing the reverse transformations
                //agg::trans_perspective tr2(m_quad.polygon(), g_x1, g_y1, g_x2, g_y2);
                //if(tr2.is_valid())
                //{
                //    double x, y;
                //    x = m_quad.xn(0); y = m_quad.yn(0); tr2.transform(&x, &y);
                //    g_rasterizer.move_to_d(x, y);
                //    x = m_quad.xn(1); y = m_quad.yn(1); tr2.transform(&x, &y);
                //    g_rasterizer.line_to_d(x, y);
                //    x = m_quad.xn(2); y = m_quad.yn(2); tr2.transform(&x, &y);
                //    g_rasterizer.line_to_d(x, y);
                //    x = m_quad.xn(3); y = m_quad.yn(3); tr2.transform(&x, &y);
                //    g_rasterizer.line_to_d(x, y);
                //    r.color(agg::rgba(0.5, 0.0, 0.0, 0.5));
                //    agg::render_scanlines(g_rasterizer, g_scanline, r);
                //}
                //else
                //{
                //    message("Singularity...");
                //}
            }
        }




        //--------------------------
        // Render the "quad" tool and controls
        g_rasterizer.add_path(m_quad);
        r.color(agg::rgba(0, 0.3, 0.5, 0.6));
        agg::render_scanlines(g_rasterizer, g_scanline, r);
        agg::render_ctrl(g_rasterizer, g_scanline, rb, m_trans_type);
        //--------------------------
    }



    virtual void on_mouse_button_down(int x, int y, unsigned flags)
    {
        if(flags & agg::mouse_left)
        {
            if(m_quad.on_mouse_button_down(x, y))
            {
                force_redraw();
            }
        }
    }


    virtual void on_mouse_move(int x, int y, unsigned flags)
    {
        if(flags & agg::mouse_left)
        {
            if(m_quad.on_mouse_move(x, y))
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
        if(m_quad.on_mouse_button_up(x, y))
        {
            force_redraw();
        }
    }

};






int agg_main(int argc, char* argv[])
{
    the_application app(pix_format, flip_y);
    app.caption("AGG Example. Perspective Transformations");

    if(app.init(600, 600, agg::window_resize))
    {
        return app.run();
    }
    return 1;
}






