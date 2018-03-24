#include <stdio.h>
#include "agg_basics.h"
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_u.h"
#include "agg_renderer_scanline.h"
#include "agg_pixfmt_rgb.h"
#include "agg_span_allocator.h"
#include "agg_span_gradient.h"
#include "agg_span_interpolator_linear.h"
#include "agg_glyph_raster_bin.h"
#include "agg_renderer_raster_text.h"
#include "agg_embedded_raster_fonts.h"
#include "platform/agg_platform_support.h"
#include "ctrl/agg_slider_ctrl.h"
#include "ctrl/agg_cbox_ctrl.h"


enum flip_y_e { flip_y = true };




//------------------------------------------------------------------------
template<class GradientF> class gradient_sine_repeat_adaptor
{
public:
    gradient_sine_repeat_adaptor() : m_periods(agg::pi* 2.0) {}

    void periods(double p) { m_periods = p * agg::pi * 2.0; }

    int calculate(int x, int y, int d) const
    {
        return int((1.0 + sin(m_gradient.calculate(x, y, d) * m_periods / d)) * d/2);
    }

private:
    GradientF m_gradient;
    double    m_periods;
};




//------------------------------------------------------------------------
class the_application : public agg::platform_support
{
public:
    typedef agg::pixfmt_bgr24 pixfmt;
    typedef agg::renderer_base<pixfmt> ren_base;
    typedef agg::glyph_raster_bin<agg::rgba8> glyph_gen;

    the_application(agg::pix_format_e format, bool flip_y) :
        agg::platform_support(format, flip_y)
    {
    }


    virtual void on_draw()
    {
        struct font_type
        {
            const agg::int8u* font;
            const char* name;
        }
        fonts[] = 
        {
            { agg::gse4x6,                  "gse4x6"               },
            { agg::gse4x8,                  "gse4x8"               },
            { agg::gse5x7,                  "gse5x7"               },
            { agg::gse5x9,                  "gse5x9"               },
            { agg::gse6x9,                  "gse6x9"               },
            { agg::gse6x12,                 "gse6x12"              },
            { agg::gse7x11,                 "gse7x11"              },
            { agg::gse7x11_bold,            "gse7x11_bold"         },
            { agg::gse7x15,                 "gse7x15"              },
            { agg::gse7x15_bold,            "gse7x15_bold"         },
            { agg::gse8x16,                 "gse8x16"              },
            { agg::gse8x16_bold,            "gse8x16_bold"         },
            { agg::mcs11_prop,              "mcs11_prop"           },
            { agg::mcs11_prop_condensed,    "mcs11_prop_condensed" },
            { agg::mcs12_prop,              "mcs12_prop"           },
            { agg::mcs13_prop,              "mcs13_prop"           },
            { agg::mcs5x10_mono,            "mcs5x10_mono"         },
            { agg::mcs5x11_mono,            "mcs5x11_mono"         },
            { agg::mcs6x10_mono,            "mcs6x10_mono"         },
            { agg::mcs6x11_mono,            "mcs6x11_mono"         },
            { agg::mcs7x12_mono_high,       "mcs7x12_mono_high"    },
            { agg::mcs7x12_mono_low,        "mcs7x12_mono_low"     },
            { agg::verdana12,               "verdana12"            },
            { agg::verdana12_bold,          "verdana12_bold"       },
            { agg::verdana13,               "verdana13"            },
            { agg::verdana13_bold,          "verdana13_bold"       },
            { agg::verdana14,               "verdana14"            },
            { agg::verdana14_bold,          "verdana14_bold"       },
            { agg::verdana16,               "verdana16"            },
            { agg::verdana16_bold,          "verdana16_bold"       },
            { agg::verdana17,               "verdana17"            },
            { agg::verdana17_bold,          "verdana17_bold"       },
            { agg::verdana18,               "verdana18"            },
            { agg::verdana18_bold,          "verdana18_bold"       },
            0, 0
        };



        glyph_gen glyph(0);
        pixfmt pixf(rbuf_window());
        ren_base rb(pixf);
        rb.clear(agg::rgba(1,1,1));


        agg::renderer_raster_htext_solid<ren_base, glyph_gen> rt(rb, glyph);

        int i;
        double y = 5;
        rt.color(agg::rgba(0,0,0));
        for(i = 0; fonts[i].font; i++)
        {
            char buf[100];
            strcpy(buf, "A quick brown fox jumps over the lazy dog 0123456789: ");
            strcat(buf, fonts[i].name);

            // Testing "wide-char"
            unsigned wbuf[100];
            unsigned* wp = wbuf;
            const char* p = buf;
            while(*p) *wp++ = *(unsigned char*)p++;
            *wp++ = 0;

            glyph.font(fonts[i].font);
            rt.render_text(5, y, wbuf, !flip_y());
            y += glyph.height() + 1;
        }


        // Rendering raster text with a custom span generator, gradient

        typedef agg::span_interpolator_linear<> interpolator_type;
        typedef agg::span_allocator<agg::rgba8> span_alloc_type;
        typedef agg::span_gradient<agg::rgba8, 
                                   interpolator_type, 
                                   gradient_sine_repeat_adaptor<agg::gradient_circle>, 
                                   agg::gradient_linear_color<agg::rgba8> > span_gen_type;
        typedef agg::renderer_scanline_aa<ren_base, 
                                          span_alloc_type,
                                          span_gen_type> ren_type;

        agg::trans_affine mtx;
        gradient_sine_repeat_adaptor<agg::gradient_circle> grad_func;
        grad_func.periods(5);
        agg::gradient_linear_color<agg::rgba8> color_func;
        color_func.colors(agg::rgba(1.0,0,0), agg::rgba(0,0.5,0));
        interpolator_type inter(mtx);
        span_alloc_type sa;
        span_gen_type sg(inter, grad_func, color_func, 0, 150.0);
        ren_type ren(rb, sa, sg);

        agg::renderer_raster_htext<ren_type, glyph_gen> rt2(ren, glyph);
        rt2.render_text(5, 465, (unsigned char*)"RADIAL REPEATING GRADIENT: A quick brown fox jumps over the lazy dog", !flip_y());
    }

};


int agg_main(int argc, char* argv[])
{
    the_application app(agg::pix_format_bgr24, flip_y);
    app.caption("AGG Example. Raster Text");

    if(app.init(640, 480, agg::window_resize))
    {
        return app.run();
    }
    return 1;
}


