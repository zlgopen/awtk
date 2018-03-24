#include <math.h>
#include <stdio.h>
#include "agg_basics.h"
#include "agg_ellipse.h"
#include "agg_gamma_lut.h"
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_rasterizer_compound_aa.h"
#include "agg_conv_curve.h"
#include "agg_conv_stroke.h"
#include "agg_scanline_u.h"
#include "agg_renderer_scanline.h"
#include "agg_span_allocator.h"
#include "agg_pixfmt_rgba.h"
#include "ctrl/agg_slider_ctrl.h"
#include "ctrl/agg_cbox_ctrl.h"
#include "platform/agg_platform_support.h"


enum flip_y_e { flip_y = true };


//-------------------------------------------------
class style_handler
{
public:
    style_handler(const agg::rgba8* styles, unsigned count) : 
        m_transparent(0, 0, 0, 0),
        m_styles(styles),
        m_count(count)
    {}

    bool is_solid(unsigned style) const { return true; }

    const agg::rgba8& color(unsigned style) const 
    {
        if (style < m_count)
            return m_styles[style];

        return m_transparent;
    }

    void generate_span(agg::rgba8* span, int x, int y, unsigned len, unsigned style)
    {
    }

private:
    agg::rgba8          m_transparent;
    const agg::rgba8*   m_styles;
    unsigned            m_count;
};


class the_application : public agg::platform_support
{
    agg::slider_ctrl<agg::rgba8> m_width;
    agg::slider_ctrl<agg::rgba8> m_alpha1;
    agg::slider_ctrl<agg::rgba8> m_alpha2;
    agg::slider_ctrl<agg::rgba8> m_alpha3;
    agg::slider_ctrl<agg::rgba8> m_alpha4;
    agg::cbox_ctrl<agg::rgba8>   m_invert_order;
    agg::path_storage            m_path;

public:
    the_application(agg::pix_format_e format, bool flip_y) :
        agg::platform_support(format, flip_y),
        m_width(180 + 10.0, 5.0, 130 + 300.0, 12, !flip_y),
        m_alpha1(5, 5,  180, 12, !flip_y),
        m_alpha2(5, 25, 180, 32, !flip_y),
        m_alpha3(5, 45, 180, 52, !flip_y),
        m_alpha4(5, 65, 180, 72, !flip_y),
        m_invert_order(190, 25, "Invert Z-Order")
    {
        add_ctrl(m_width);
        m_width.range(-20.0, 50.0);
        m_width.value(10.0);
        m_width.label("Width=%1.2f");

        add_ctrl(m_alpha1);
        m_alpha1.range(0, 1);
        m_alpha1.value(1);
        m_alpha1.label("Alpha1=%1.3f");

        add_ctrl(m_alpha2);
        m_alpha2.range(0, 1);
        m_alpha2.value(1);
        m_alpha2.label("Alpha2=%1.3f");

        add_ctrl(m_alpha3);
        m_alpha3.range(0, 1);
        m_alpha3.value(1);
        m_alpha3.label("Alpha3=%1.3f");

        add_ctrl(m_alpha4);
        m_alpha4.range(0, 1);
        m_alpha4.value(1);
        m_alpha4.label("Alpha4=%1.3f");

        add_ctrl(m_invert_order);
    }

    void compose_path()
    {
        m_path.remove_all();
        m_path.move_to(28.47, 6.45);
        m_path.curve3(21.58, 1.12, 19.82, 0.29);
        m_path.curve3(17.19, -0.93, 14.21, -0.93);
        m_path.curve3(9.57, -0.93, 6.57, 2.25);
        m_path.curve3(3.56, 5.42, 3.56, 10.60);
        m_path.curve3(3.56, 13.87, 5.03, 16.26);
        m_path.curve3(7.03, 19.58, 11.99, 22.51);
        m_path.curve3(16.94, 25.44, 28.47, 29.64);
        m_path.line_to(28.47, 31.40);
        m_path.curve3(28.47, 38.09, 26.34, 40.58);
        m_path.curve3(24.22, 43.07, 20.17, 43.07);
        m_path.curve3(17.09, 43.07, 15.28, 41.41);
        m_path.curve3(13.43, 39.75, 13.43, 37.60);
        m_path.line_to(13.53, 34.77);
        m_path.curve3(13.53, 32.52, 12.38, 31.30);
        m_path.curve3(11.23, 30.08, 9.38, 30.08);
        m_path.curve3(7.57, 30.08, 6.42, 31.35);
        m_path.curve3(5.27, 32.62, 5.27, 34.81);
        m_path.curve3(5.27, 39.01, 9.57, 42.53);
        m_path.curve3(13.87, 46.04, 21.63, 46.04);
        m_path.curve3(27.59, 46.04, 31.40, 44.04);
        m_path.curve3(34.28, 42.53, 35.64, 39.31);
        m_path.curve3(36.52, 37.21, 36.52, 30.71);
        m_path.line_to(36.52, 15.53);
        m_path.curve3(36.52, 9.13, 36.77, 7.69);
        m_path.curve3(37.01, 6.25, 37.57, 5.76);
        m_path.curve3(38.13, 5.27, 38.87, 5.27);
        m_path.curve3(39.65, 5.27, 40.23, 5.62);
        m_path.curve3(41.26, 6.25, 44.19, 9.18);
        m_path.line_to(44.19, 6.45);
        m_path.curve3(38.72, -0.88, 33.74, -0.88);
        m_path.curve3(31.35, -0.88, 29.93, 0.78);
        m_path.curve3(28.52, 2.44, 28.47, 6.45);
        m_path.close_polygon();

        m_path.move_to(28.47, 9.62);
        m_path.line_to(28.47, 26.66);
        m_path.curve3(21.09, 23.73, 18.95, 22.51);
        m_path.curve3(15.09, 20.36, 13.43, 18.02);
        m_path.curve3(11.77, 15.67, 11.77, 12.89);
        m_path.curve3(11.77, 9.38, 13.87, 7.06);
        m_path.curve3(15.97, 4.74, 18.70, 4.74);
        m_path.curve3(22.41, 4.74, 28.47, 9.62);
        m_path.close_polygon();
    }



    virtual void on_draw()
    {
        typedef agg::renderer_base<agg::pixfmt_bgra32>     ren_base;
        typedef agg::renderer_base<agg::pixfmt_bgra32_pre> ren_base_pre;

        agg::gamma_lut<agg::int8u, agg::int8u> lut(2.0);

        agg::pixfmt_bgra32 pixf(rbuf_window());
        ren_base renb(pixf);

        agg::pixfmt_bgra32_pre pixf_pre(rbuf_window());
        ren_base_pre renb_pre(pixf_pre);

        // Clear the window with a gradient
        agg::pod_vector<agg::rgba8> gr(pixf_pre.width());
        unsigned i;
        for(i = 0; i < pixf.width(); i++)
        {
            gr.add(agg::rgba8(255, 255, 0).gradient(agg::rgba8(0, 255, 255), 
                                                    double(i) / pixf.width()));
        }
        for(i = 0; i < pixf.height(); i++)
        {
            renb.copy_color_hspan(0, i, pixf.width(), &gr[0]);
        }
        pixf.apply_gamma_dir(lut);


        agg::rasterizer_scanline_aa<> ras;
        agg::rasterizer_compound_aa<agg::rasterizer_sl_clip_dbl> rasc;
        agg::scanline_u8 sl;
        agg::span_allocator<agg::rgba8> alloc;

        // Draw two triangles
        ras.move_to_d(0, 0);
        ras.line_to_d(width(), 0);
        ras.line_to_d(width(), height());
        agg::render_scanlines_aa_solid(ras, sl, renb, 
                                       agg::rgba8(lut.dir(0), 
                                                  lut.dir(100), 
                                                  lut.dir(0)));

        ras.move_to_d(0, 0);
        ras.line_to_d(0, height());
        ras.line_to_d(width(), 0);
        agg::render_scanlines_aa_solid(ras, sl, renb, 
                                       agg::rgba8(lut.dir(0), 
                                                  lut.dir(100), 
                                                  lut.dir(100)));

        agg::trans_affine mtx;
        mtx *= agg::trans_affine_scaling(4.0);
        mtx *= agg::trans_affine_translation(150, 100);

        agg::conv_transform<agg::path_storage> trans(m_path, mtx);
        agg::conv_curve<agg::conv_transform<agg::path_storage> > curve(trans);

        agg::conv_stroke
            <agg::conv_curve
                <agg::conv_transform
                    <agg::path_storage> > > stroke(curve);

        compose_path();

        agg::rgba8 styles[4];

        if(m_invert_order.status())
        {
            rasc.layer_order(agg::layer_inverse);
        }
        else
        {
            rasc.layer_order(agg::layer_direct);
        }

        styles[3] = agg::rgba8(lut.dir(255),
                               lut.dir(0),
                               lut.dir(108),
                               200).premultiply();

        styles[2] = agg::rgba8(lut.dir(51),
                               lut.dir(0),
                               lut.dir(151),
                               180).premultiply();

        styles[1] = agg::rgba8(lut.dir(143),
                               lut.dir(90),
                               lut.dir(6),
                               200).premultiply();

        styles[0] = agg::rgba8(lut.dir(0),
                               lut.dir(0),
                               lut.dir(255),
                               220).premultiply();

        style_handler sh(styles, 4);

        stroke.width(m_width.value());

        rasc.reset();
        rasc.master_alpha(3, m_alpha1.value());
        rasc.master_alpha(2, m_alpha2.value());
        rasc.master_alpha(1, m_alpha3.value());
        rasc.master_alpha(0, m_alpha4.value());

        agg::ellipse ell(220.0, 180.0, 120.0, 10.0, 128, false);
        agg::conv_stroke<agg::ellipse> str_ell(ell);
        str_ell.width(m_width.value() / 2);

        rasc.styles(3, -1);
        rasc.add_path(str_ell);

        rasc.styles(2, -1);
        rasc.add_path(ell);

        rasc.styles(1, -1);
        rasc.add_path(stroke);

        rasc.styles(0, -1);
        rasc.add_path(curve);

        agg::render_scanlines_compound_layered(rasc, sl, renb_pre, alloc, sh);
        agg::render_ctrl(ras, sl, renb, m_width);
        agg::render_ctrl(ras, sl, renb, m_alpha1);
        agg::render_ctrl(ras, sl, renb, m_alpha2);
        agg::render_ctrl(ras, sl, renb, m_alpha3);
        agg::render_ctrl(ras, sl, renb, m_alpha4);
        agg::render_ctrl(ras, sl, renb, m_invert_order);

        pixf.apply_gamma_inv(lut);
    }

};



int agg_main(int argc, char* argv[])
{
    the_application app(agg::pix_format_bgra32, flip_y);
    app.caption("AGG Example. Compound Rasterizer -- Geometry Flattening");

    if(app.init(440, 330, 0))
    {
        return app.run();
    }
    return 1;
}
