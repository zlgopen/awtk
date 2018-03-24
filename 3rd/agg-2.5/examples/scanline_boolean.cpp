#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include "agg_basics.h"
#include "agg_array.h"
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_p.h"
#include "agg_scanline_u.h"
#include "agg_scanline_bin.h"
#include "agg_scanline_boolean_algebra.h"
#include "agg_scanline_storage_aa.h"
#include "agg_scanline_storage_bin.h"
#include "agg_renderer_scanline.h"
#include "agg_path_storage.h"
#include "agg_conv_transform.h"
#include "agg_ellipse.h"
#include "ctrl/agg_rbox_ctrl.h"
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









void generate_circles(agg::path_storage& ps, 
                      const double* quad, 
                      unsigned num_circles, 
                      double radius)
{
    ps.remove_all();
    unsigned i;
    for(i = 0; i < 4; ++i)
    {
        unsigned n1 = i * 2;
        unsigned n2 = (i < 3) ? i * 2 + 2 : 0; 
        unsigned j;
        for(j = 0; j < num_circles; j++)
        {
            agg::ellipse ell(quad[n1]     + (quad[n2]     - quad[n1])     * j / num_circles, 
                             quad[n1 + 1] + (quad[n2 + 1] - quad[n1 + 1]) * j / num_circles, 
                             radius, radius, 100);
            ps.concat_path(ell);
        }
    }
}





class the_application : public agg::platform_support
{
public:
    typedef agg::renderer_base<pixfmt> renderer_base;
    typedef agg::renderer_scanline_aa_solid<renderer_base> renderer_solid;
    typedef agg::scanline_p8 scanline_type;

    agg::interactive_polygon     m_quad1;
    agg::interactive_polygon     m_quad2;
    agg::rbox_ctrl<agg::rgba8>   m_trans_type;
    agg::cbox_ctrl<agg::rgba8>   m_reset;
    agg::slider_ctrl<agg::rgba8> m_mul1;
    agg::slider_ctrl<agg::rgba8> m_mul2;

    the_application(agg::pix_format_e format, bool flip_y) :
        agg::platform_support(format, flip_y),
        m_quad1(4, 5.0),
        m_quad2(4, 5.0),
        m_trans_type(420, 5.0, 420+130.0, 145.0, !flip_y),
        m_reset     (350, 5.0,  "Reset", !flip_y),
        m_mul1      (5.0,  5.0, 340.0, 12.0, !flip_y),
        m_mul2      (5.0, 20.0, 340.0, 27.0, !flip_y)
    {
        m_trans_type.add_item("Union");
        m_trans_type.add_item("Intersection");
        m_trans_type.add_item("Linear XOR");
        m_trans_type.add_item("Saddle XOR");
        m_trans_type.add_item("Abs Diff XOR");
        m_trans_type.add_item("A-B");
        m_trans_type.add_item("B-A");
        m_trans_type.cur_item(0);
        add_ctrl(m_trans_type);
        add_ctrl(m_reset);
        add_ctrl(m_mul1);
        add_ctrl(m_mul2);
        m_mul1.value(1.0);
        m_mul2.value(1.0);
        m_mul1.label("Opacity1=%.3f");
        m_mul2.label("Opacity2=%.3f");
    }


    virtual void on_init()
    {
        m_quad1.xn(0) = 50;
        m_quad1.yn(0) = 200 - 20;
        m_quad1.xn(1) = width() / 2 - 25;
        m_quad1.yn(1) = 200;
        m_quad1.xn(2) = width() / 2 - 25;
        m_quad1.yn(2) = height() - 50 - 20;
        m_quad1.xn(3) = 50;
        m_quad1.yn(3) = height() - 50;

        m_quad2.xn(0) = width() / 2 + 25;
        m_quad2.yn(0) = 200 - 20;
        m_quad2.xn(1) = width() - 50;
        m_quad2.yn(1) = 200;
        m_quad2.xn(2) = width() - 50;
        m_quad2.yn(2) = height() - 50 - 20;
        m_quad2.xn(3) = width() / 2 + 25;
        m_quad2.yn(3) = height() - 50;
    }




    virtual void on_draw()
    {
        pixfmt pixf(rbuf_window());
        renderer_base rb(pixf);
        renderer_solid r(rb);
        rb.clear(agg::rgba(1, 1, 1));

        scanline_type sl;
        agg::rasterizer_scanline_aa<> ras;
        agg::rasterizer_scanline_aa<> ras1;
        agg::rasterizer_scanline_aa<> ras2;

        agg::sbool_op_e op = (agg::sbool_op_e)m_trans_type.cur_item();

        ras1.gamma(agg::gamma_multiply(m_mul1.value()));
        ras2.gamma(agg::gamma_multiply(m_mul2.value()));

        ras.clip_box(0, 0, width(), height());
        
        agg::path_storage ps1;
        generate_circles(ps1, m_quad1.polygon(), 5, 20);

        agg::path_storage ps2;
        generate_circles(ps2, m_quad2.polygon(), 5, 20);

        ras1.filling_rule(agg::fill_even_odd);


        r.color(agg::rgba8(240, 255, 200, 100));
        ras1.add_path(ps1);
        agg::render_scanlines(ras1, sl, r);

        r.color(agg::rgba8(255, 240, 240, 100));
        ras2.add_path(ps2);
        agg::render_scanlines(ras2, sl, r);


        
        typedef agg::scanline_p8                               sbool_scanline_type;
        typedef agg::renderer_scanline_aa_solid<renderer_base> sbool_renderer_type;

        sbool_scanline_type sl_result;
        sbool_scanline_type sl1;
        sbool_scanline_type sl2;
        sbool_renderer_type sren(rb);

        sren.color(agg::rgba8(0, 0, 0));

        agg::sbool_combine_shapes_aa(op, ras1, ras2, sl1, sl2, sl_result, sren);

        //--------------------------
        // Render the "quad" tools and controls
        r.color(agg::rgba(0, 0.3, 0.5, 0.6));
        ras.add_path(m_quad1);
        agg::render_scanlines(ras, sl, r);
        ras.add_path(m_quad2);
        agg::render_scanlines(ras, sl, r);
        agg::render_ctrl(ras, sl, rb, m_trans_type);
        agg::render_ctrl(ras, sl, rb, m_reset);
        agg::render_ctrl(ras, sl, rb, m_mul1);
        agg::render_ctrl(ras, sl, rb, m_mul2);
        //--------------------------

    }



    virtual void on_mouse_button_down(int x, int y, unsigned flags)
    {
        if(flags & agg::mouse_left)
        {
            if(m_quad1.on_mouse_button_down(x, y) ||
               m_quad2.on_mouse_button_down(x, y) )
            {
                force_redraw();
            }
        }
    }


    virtual void on_mouse_move(int x, int y, unsigned flags)
    {
        if(flags & agg::mouse_left)
        {
            if(m_quad1.on_mouse_move(x, y) ||
               m_quad2.on_mouse_move(x, y) )
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
        if(m_quad1.on_mouse_button_up(x, y) ||
           m_quad2.on_mouse_button_up(x, y) )
        {
            force_redraw();
        }
    }


    virtual void on_ctrl_change()
    {
        if(m_reset.status())
        {
            on_init();
            m_reset.status(false);
            force_redraw();
        }
    }
};






int agg_main(int argc, char* argv[])
{
    the_application app(pix_format, flip_y);
    app.caption("AGG Example. Scanline Boolean");

    if(app.init(800, 600, agg::window_resize))
    {
        return app.run();
    }
    return 1;
}










