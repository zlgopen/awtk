#include <stdio.h>
#include "agg_trans_affine.h"
#include "agg_conv_stroke.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_rendering_buffer.h"
#include "agg_scanline_u.h"
#include "agg_renderer_scanline.h"
#include "agg_gamma_lut.h"
#include "ctrl/agg_slider_ctrl.h"
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

class the_application : public agg::platform_support
{
    agg::slider_ctrl<agg::rgba8> m_thickness;
    agg::slider_ctrl<agg::rgba8> m_gamma;
    agg::slider_ctrl<agg::rgba8> m_contrast;
    double m_rx;
    double m_ry;

public:
    the_application(agg::pix_format_e format, bool flip_y) :
        agg::platform_support(format, flip_y),
        m_thickness(5, 5,    400-5, 11,    !flip_y),
        m_gamma    (5, 5+15, 400-5, 11+15, !flip_y),
        m_contrast (5, 5+30, 400-5, 11+30, !flip_y)
    {
        add_ctrl(m_thickness);
        add_ctrl(m_gamma);
        add_ctrl(m_contrast);

        m_thickness.label("Thickness=%3.2f");
        m_gamma.label("Gamma=%3.2f");
        m_contrast.label("Contrast");

        m_thickness.range(0.0, 3.0);
        m_gamma.range(0.5, 3.0);
        m_contrast.range(0.0, 1.0);

        m_thickness.value(1.0);
        m_gamma.value(1.0);
        m_contrast.value(1.0);
    }

    virtual void on_init()
    {
        m_rx = width() / 3.0;
        m_ry = height() / 3.0;
    }

    virtual void on_draw()
    {
        typedef agg::gamma_lut<agg::int8u, agg::int8u, 8, 8> gamma_type;
        typedef pixfmt_gamma<gamma_type> pixfmt_type;
        typedef agg::renderer_base<pixfmt_type> ren_base;

        double g = m_gamma.value();
        gamma_type gamma(g);
        pixfmt_type pixf(rbuf_window(), gamma);
        ren_base renb(pixf);
        renb.clear(agg::rgba(1, 1, 1));


        double dark = 1.0 - m_contrast.value();
        double light = m_contrast.value();

        renb.copy_bar(0,0,int(width())/2, int(height()),                agg::rgba(dark,dark,dark));
        renb.copy_bar(int(width())/2+1,0, int(width()), int(height()),  agg::rgba(light,light,light));
        renb.copy_bar(0,int(height())/2+1, int(width()), int(height()), agg::rgba(1.0,dark,dark));

        agg::rasterizer_scanline_aa<> ras;
        agg::scanline_u8 sl;
        agg::path_storage path;


        unsigned i;
        double x = (width() - 256.0) / 2.0;
        double y = 50.0;
        path.remove_all();
        agg::gamma_power gp(g);
        for(i = 0; i < 256; i++)
        {
            double v = double(i) / 255.0;
            double gval = gp(v);
            double dy = gval * 255.0;
            if(i == 0) path.move_to(x + i, y + dy);
            else       path.line_to(x + i, y + dy);
        }
        agg::conv_stroke<agg::path_storage> gpoly(path);
        gpoly.width(2.0);
        ras.reset();
        ras.add_path(gpoly);
        agg::render_scanlines_aa_solid(ras, sl, renb, agg::rgba8(80,127,80));

        agg::ellipse ell(width() / 2, height() / 2, m_rx, m_ry, 150);
        agg::conv_stroke<agg::ellipse> poly(ell);
        poly.width(m_thickness.value());
        ras.reset();
        ras.add_path(poly);
        agg::render_scanlines_aa_solid(ras, sl, renb, agg::rgba8(255,0,0));

        ell.init(width() / 2, height() / 2, m_rx-5.0, m_ry-5.0, 150);
        ras.reset();
        ras.add_path(poly);
        agg::render_scanlines_aa_solid(ras, sl, renb, agg::rgba8(0,255,0));

        ell.init(width() / 2, height() / 2, m_rx-10.0, m_ry-10.0, 150);
        ras.reset();
        ras.add_path(poly);
        agg::render_scanlines_aa_solid(ras, sl, renb, agg::rgba8(0,0,255));

        ell.init(width() / 2, height() / 2, m_rx-15.0, m_ry-15.0, 150);
        ras.reset();
        ras.add_path(poly);
        agg::render_scanlines_aa_solid(ras, sl, renb, agg::rgba8(0,0,0));

        ell.init(width() / 2, height() / 2, m_rx-20.0, m_ry-20.0, 150);
        ras.reset();
        ras.add_path(poly);
        agg::render_scanlines_aa_solid(ras, sl, renb, agg::rgba8(255,255,255));

        agg::render_ctrl(ras, sl, renb, m_thickness);
        agg::render_ctrl(ras, sl, renb, m_gamma);
        agg::render_ctrl(ras, sl, renb, m_contrast);
    }


    virtual void on_mouse_button_down(int x, int y, unsigned flags)
    {
        if(flags & agg::mouse_left)
        {
            m_rx = fabs(width()/2 - x);
            m_ry = fabs(height()/2 - y);
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
    app.caption("AGG Example. Thin red ellipse");

    if(app.init(400, 320, 0))
    {
        return app.run();
    }
    return 1;
}


