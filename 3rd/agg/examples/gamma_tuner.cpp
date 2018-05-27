#include <stdio.h>
#include "agg_trans_affine.h"
#include "agg_conv_stroke.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_rendering_buffer.h"
#include "agg_scanline_u.h"
#include "agg_renderer_scanline.h"
#include "agg_gamma_lut.h"
#include "ctrl/agg_slider_ctrl.h"
#include "ctrl/agg_rbox_ctrl.h"
#include "platform/agg_platform_support.h"

#include "agg_pixfmt_rgb.h"

// This example only makes any sense in uncorrected sRGB.
#define AGG_SBGR24
#include "pixel_formats.h"

enum flip_y_e { flip_y = true };

class the_application : public agg::platform_support
{
    agg::slider_ctrl<color_type> m_gamma;
    agg::slider_ctrl<color_type> m_r;
    agg::slider_ctrl<color_type> m_g;
    agg::slider_ctrl<color_type> m_b;
    agg::rbox_ctrl  <agg::srgba8> m_pattern;


public:
    the_application(agg::pix_format_e format, bool flip_y) :
        agg::platform_support(format, flip_y),
        m_r        (5, 5,    350-5, 11,    !flip_y),
        m_g        (5, 5+15, 350-5, 11+15, !flip_y),
        m_b        (5, 5+30, 350-5, 11+30, !flip_y),
        m_gamma    (5, 5+45, 350-5, 11+45, !flip_y),
        m_pattern  (355, 1,  495,   60, !flip_y)
    {
        add_ctrl(m_r);
        m_r.value(1.0);
        m_r.label("R=%.2f");

        add_ctrl(m_g);
        m_g.value(1.0);
        m_g.label("G=%.2f");

        add_ctrl(m_b);
        m_b.value(1.0);
        m_b.label("B=%.2f");

        add_ctrl(m_gamma);
        m_gamma.range(0.5, 4.0);
        m_gamma.value(2.2);
        m_gamma.label("Gamma=%.2f");

        m_pattern.text_size(8);
        m_pattern.add_item("Horizontal");
        m_pattern.add_item("Vertical");
        m_pattern.add_item("Checkered");
        m_pattern.cur_item(2);
        add_ctrl(m_pattern);
    }

    virtual void on_init()
    {
    }

    virtual void on_draw()
    {
        typedef agg::gamma_lut<color_type::value_type, color_type::value_type, 
                               color_type::base_shift, color_type::base_shift> gamma_type;

        typedef pixfmt_gamma<gamma_type> pixfmt_type;
        typedef agg::renderer_base<pixfmt_type> ren_base;

        double g = m_gamma.value();
        gamma_type gamma(g);

        pixfmt_type pixf(rbuf_window(), gamma);

        ren_base renb(pixf);

        agg::rasterizer_scanline_aa<> ras;
        agg::scanline_u8 sl;


        enum 
        { 
            square_size = 400,
            ver_strips  = 5
        };

        color_type span1[square_size];
        color_type span2[square_size];

        color_type color(agg::rgba(m_r.value(), m_g.value(), m_b.value()));

        unsigned i, j;


        // Draw vertical gradient
        //-----------------------
        unsigned w = (unsigned)width();
        unsigned h = (unsigned)height();
        for(i = 0; i < h; i++)
        {
            double k = (i - 80) / double(square_size - 1);
            if(i < 80)              k = 0.0;
            if(i >= 80+square_size) k = 1.0;

            k = 1 - pow(k/2, 1/m_gamma.value());
            color_type c = color.gradient(color_type(0,0,0), k);
            renb.copy_hline(0, i, w-1, c);
        }

        // Calculate spans
        //-----------------------
        switch(m_pattern.cur_item())
        {
        case 0:
            for(i = 0; i < square_size; i++)
            {
                span1[i] = span2[i] = color;
                span1[i].a = i * color_type::full_value() / square_size;
                span2[i].a = color_type::full_value() - span1[i].a;
            }
            break;

        case 1:
            for(i = 0; i < square_size; i++)
            {
                span1[i] = span2[i] = color;
                if(i & 1)
                {
                    span1[i].a = i * color_type::full_value() / square_size;
                    span2[i].a = span1[i].a;
                }
                else
                {
                    span1[i].a = color_type::full_value() - 
                                 i * color_type::full_value() / square_size;
                    span2[i].a = span1[i].a;
                }
            }
            break;

        case 2:
            for(i = 0; i < square_size; i++)
            {
                span1[i] = span2[i] = color;
                if(i & 1)
                {
                    span1[i].a = i * color_type::full_value() / square_size;
                    span2[i].a = color_type::full_value() - span1[i].a;
                }
                else
                {
                    span2[i].a = i * color_type::full_value() / square_size;
                    span1[i].a = color_type::full_value() - span2[i].a;
                }
            }
            break;
        }

        // Clear the area
        //---------------------
        renb.copy_bar(50, 80, 50+square_size-1, 80+square_size-1, agg::rgba(0,0,0));


        // Draw the patern
        //---------------------
        for(i = 0; i < square_size; i += 2)
        {
            double k = i / double(square_size - 1);
            k = 1 - pow(k, 1/m_gamma.value());
            color_type c = color.gradient(agg::rgba(0,0,0), k);
            for(j = 0; j < square_size; j++)
            {
                span1[j].r = span2[j].r = c.r;
                span1[j].g = span2[j].g = c.g;
                span1[j].b = span2[j].b = c.b;
            }
            renb.blend_color_hspan(50, i + 80 + 0, square_size, span1, 0, 255);
            renb.blend_color_hspan(50, i + 80 + 1, square_size, span2, 0, 255);
        }


        // Draw vertical strips
        //---------------------
        for(i = 0; i < square_size; i++)
        {
            double k = i / double(square_size - 1);

            k = 1 - pow(k/2, 1/m_gamma.value());
            color_type c = color.gradient(color_type(0,0,0), k);
            for(j = 0; j < ver_strips; j++)
            {
                int xc = square_size * (j + 1) / (ver_strips + 1);
                renb.copy_hline(50+xc-10, i+80, 50+xc+10, c);
            }
        }

        agg::render_ctrl(ras, sl, renb, m_gamma);
        agg::render_ctrl(ras, sl, renb, m_r);
        agg::render_ctrl(ras, sl, renb, m_g);
        agg::render_ctrl(ras, sl, renb, m_b);
        agg::render_ctrl(ras, sl, renb, m_pattern);

    }


    virtual void on_mouse_button_down(int x, int y, unsigned flags)
    {
    }


    virtual void on_mouse_move(int x, int y, unsigned flags)
    {
    }

};


int agg_main(int argc, char* argv[])
{
    the_application app(pix_format, flip_y);
    app.caption("AGG Example. Gamma Tuner");

    if(app.init(500, 500, 0))
    {
        return app.run();
    }
    return 1;
}


