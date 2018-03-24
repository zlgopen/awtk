#include <math.h>
#include <stdio.h>
#include <time.h>
#include "agg_basics.h"
#include "agg_rendering_buffer.h"
#include "agg_conv_transform.h"
#include "agg_conv_stroke.h"
#include "agg_conv_clip_polyline.h"
#include "agg_scanline_p.h"
#include "agg_renderer_scanline.h"
#include "agg_rasterizer_outline_aa.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_pattern_filters_rgba.h"
#include "agg_renderer_outline_aa.h"
#include "agg_renderer_outline_image.h"
#include "agg_pixfmt_rgb.h"
#include "ctrl/agg_slider_ctrl.h"
#include "ctrl/agg_bezier_ctrl.h"
#include "platform/agg_platform_support.h"


enum flip_y_e { flip_y = true };

typedef agg::pixfmt_bgr24 pixfmt;


static agg::int8u brightness_to_alpha[256 * 3] = 
{
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 254, 254, 254, 254, 254, 254, 
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
    254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 253, 253, 
    253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 252, 
    252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 252, 251, 251, 251, 251, 251, 
    251, 251, 251, 251, 250, 250, 250, 250, 250, 250, 250, 250, 249, 249, 249, 249, 
    249, 249, 249, 248, 248, 248, 248, 248, 248, 248, 247, 247, 247, 247, 247, 246, 
    246, 246, 246, 246, 246, 245, 245, 245, 245, 245, 244, 244, 244, 244, 243, 243, 
    243, 243, 243, 242, 242, 242, 242, 241, 241, 241, 241, 240, 240, 240, 239, 239, 
    239, 239, 238, 238, 238, 238, 237, 237, 237, 236, 236, 236, 235, 235, 235, 234, 
    234, 234, 233, 233, 233, 232, 232, 232, 231, 231, 230, 230, 230, 229, 229, 229, 
    228, 228, 227, 227, 227, 226, 226, 225, 225, 224, 224, 224, 223, 223, 222, 222, 
    221, 221, 220, 220, 219, 219, 219, 218, 218, 217, 217, 216, 216, 215, 214, 214, 
    213, 213, 212, 212, 211, 211, 210, 210, 209, 209, 208, 207, 207, 206, 206, 205, 
    204, 204, 203, 203, 202, 201, 201, 200, 200, 199, 198, 198, 197, 196, 196, 195, 
    194, 194, 193, 192, 192, 191, 190, 190, 189, 188, 188, 187, 186, 186, 185, 184, 
    183, 183, 182, 181, 180, 180, 179, 178, 177, 177, 176, 175, 174, 174, 173, 172, 
    171, 171, 170, 169, 168, 167, 166, 166, 165, 164, 163, 162, 162, 161, 160, 159, 
    158, 157, 156, 156, 155, 154, 153, 152, 151, 150, 149, 148, 148, 147, 146, 145, 
    144, 143, 142, 141, 140, 139, 138, 137, 136, 135, 134, 133, 132, 131, 130, 129, 
    128, 128, 127, 125, 124, 123, 122, 121, 120, 119, 118, 117, 116, 115, 114, 113, 
    112, 111, 110, 109, 108, 107, 106, 105, 104, 102, 101, 100,  99,  98,  97,  96,  
     95,  94,  93,  91,  90,  89,  88,  87,  86,  85,  84,  82,  81,  80,  79,  78, 
     77,  75,  74,  73,  72,  71,  70,  69,  67,  66,  65,  64,  63,  61,  60,  59, 
     58,  57,  56,  54,  53,  52,  51,  50,  48,  47,  46,  45,  44,  42,  41,  40, 
     39,  37,  36,  35,  34,  33,  31,  30,  29,  28,  27,  25,  24,  23,  22,  20, 
     19,  18,  17,  15,  14,  13,  12,  11,   9,   8,   7,   6,   4,   3,   2,   1
};


class pattern_src_brightness_to_alpha_rgba8
{
public:
    pattern_src_brightness_to_alpha_rgba8(agg::rendering_buffer& rb) : 
        m_rb(&rb), m_pf(*m_rb) {}

    unsigned width()  const { return m_pf.width();  }
    unsigned height() const { return m_pf.height(); }
    agg::rgba8 pixel(int x, int y) const
    {
        agg::rgba8 c = m_pf.pixel(x, y);
        c.a = brightness_to_alpha[c.r + c.g + c.b];
        return c;
    }

private:
    agg::rendering_buffer* m_rb;
    pixfmt m_pf;
};


class the_application : public agg::platform_support
{
    agg::rgba8 m_ctrl_color;
    agg::bezier_ctrl<agg::rgba8> m_curve1;
    agg::bezier_ctrl<agg::rgba8> m_curve2;
    agg::bezier_ctrl<agg::rgba8> m_curve3;
    agg::bezier_ctrl<agg::rgba8> m_curve4;
    agg::bezier_ctrl<agg::rgba8> m_curve5;
    agg::bezier_ctrl<agg::rgba8> m_curve6;
    agg::bezier_ctrl<agg::rgba8> m_curve7;
    agg::bezier_ctrl<agg::rgba8> m_curve8;
    agg::bezier_ctrl<agg::rgba8> m_curve9;
    agg::slider_ctrl<agg::rgba8> m_scale_x;
    agg::slider_ctrl<agg::rgba8> m_start_x;

public:
    typedef agg::renderer_base<pixfmt> renderer_base;
    typedef agg::renderer_scanline_aa_solid<renderer_base> renderer_scanline;
    typedef agg::rasterizer_scanline_aa<> rasterizer_scanline;
    typedef agg::scanline_p8 scanline;


    the_application(agg::pix_format_e format, bool flip_y) :
        agg::platform_support(format, flip_y),
        m_ctrl_color(agg::rgba(0, 0.3, 0.5, 0.3)),
        m_scale_x(5.0,   5.0, 240.0, 12.0, !flip_y),
        m_start_x(250.0, 5.0, 495.0, 12.0, !flip_y)
    {
        m_curve1.line_color(m_ctrl_color);
        m_curve2.line_color(m_ctrl_color);
        m_curve3.line_color(m_ctrl_color);
        m_curve4.line_color(m_ctrl_color);
        m_curve5.line_color(m_ctrl_color);
        m_curve6.line_color(m_ctrl_color);
        m_curve7.line_color(m_ctrl_color);
        m_curve8.line_color(m_ctrl_color);
        m_curve9.line_color(m_ctrl_color);

        m_curve1.curve(64, 19, 14, 126, 118, 266, 19, 265);
        m_curve2.curve(112, 113, 178, 32, 200, 132, 125, 438);
        m_curve3.curve(401, 24, 326, 149, 285, 11, 177, 77);
        m_curve4.curve(188, 427, 129, 295, 19, 283, 25, 410);
        m_curve5.curve(451, 346, 302, 218, 265, 441, 459, 400);
        m_curve6.curve(454, 198, 14, 13, 220, 291, 483, 283);
        m_curve7.curve(301, 398, 355, 231, 209, 211, 170, 353);
        m_curve8.curve(484, 101, 222, 33, 486, 435, 487, 138);
        m_curve9.curve(143, 147, 11, 45, 83, 427, 132, 197);

        add_ctrl(m_curve1);
        add_ctrl(m_curve2);
        add_ctrl(m_curve3);
        add_ctrl(m_curve4);
        add_ctrl(m_curve5);
        add_ctrl(m_curve6);
        add_ctrl(m_curve7);
        add_ctrl(m_curve8);
        add_ctrl(m_curve9);

        m_curve1.no_transform();
        m_curve2.no_transform();
        m_curve3.no_transform();
        m_curve4.no_transform();
        m_curve5.no_transform();
        m_curve6.no_transform();
        m_curve7.no_transform();
        m_curve8.no_transform();
        m_curve9.no_transform();

        m_scale_x.label("Scale X=%.2f");
        m_scale_x.range(0.2, 3.0);
        m_scale_x.value(1.0);
        m_scale_x.no_transform();
        add_ctrl(m_scale_x);

        m_start_x.label("Start X=%.2f");
        m_start_x.range(0.0, 10.0);
        m_start_x.value(0.0);
        m_start_x.no_transform();
        add_ctrl(m_start_x);
    }


    template<class Pattern, 
             class Rasterizer, 
             class Renderer, 
             class PatternSource, 
             class VertexSource>
    void draw_curve(Pattern& patt, 
                    Rasterizer& ras, 
                    Renderer& ren, 
                    PatternSource& src, 
                    VertexSource& vs)
    {
        patt.create(src);
        ren.scale_x(m_scale_x.value());
        ren.start_x(m_start_x.value());
        ras.add_path(vs);
    }


    virtual void on_draw()
    {
        pixfmt pf(rbuf_window());
        renderer_base ren_base(pf);
        ren_base.clear(agg::rgba(1.0, 1.0, 0.95));
        renderer_scanline ren(ren_base);

        rasterizer_scanline ras;
        scanline sl;

        // Pattern source. Must have an interface:
        // width() const
        // height() const
        // pixel(int x, int y) const
        // Any agg::renderer_base<> or derived
        // is good for the use as a source.
        //-----------------------------------
        pattern_src_brightness_to_alpha_rgba8 p1(rbuf_img(0));
        pattern_src_brightness_to_alpha_rgba8 p2(rbuf_img(1));
        pattern_src_brightness_to_alpha_rgba8 p3(rbuf_img(2));
        pattern_src_brightness_to_alpha_rgba8 p4(rbuf_img(3));
        pattern_src_brightness_to_alpha_rgba8 p5(rbuf_img(4));
        pattern_src_brightness_to_alpha_rgba8 p6(rbuf_img(5));
        pattern_src_brightness_to_alpha_rgba8 p7(rbuf_img(6));
        pattern_src_brightness_to_alpha_rgba8 p8(rbuf_img(7));
        pattern_src_brightness_to_alpha_rgba8 p9(rbuf_img(8));

        agg::pattern_filter_bilinear_rgba8 fltr;           // Filtering functor

        // agg::line_image_pattern is the main container for the patterns. It creates
        // a copy of the patterns extended according to the needs of the filter.
        // agg::line_image_pattern can operate with arbitrary image width, but if the 
        // width of the pattern is power of 2, it's better to use the modified
        // version agg::line_image_pattern_pow2 because it works about 15-25 percent
        // faster than agg::line_image_pattern (because of using simple masking instead 
        // of expensive '%' operation). 
        typedef agg::line_image_pattern<agg::pattern_filter_bilinear_rgba8> pattern_type;
        typedef agg::renderer_base<pixfmt> base_ren_type;
        typedef agg::renderer_outline_image<base_ren_type, pattern_type> renderer_type;
        typedef agg::rasterizer_outline_aa<renderer_type>                rasterizer_type;

        //-- Create with specifying the source
        //pattern_type patt(fltr, src);   

        //-- Create uninitialized and set the source
        pattern_type patt(fltr);        
        renderer_type ren_img(ren_base, patt);
        rasterizer_type ras_img(ren_img);

        draw_curve(patt, ras_img, ren_img, p1, m_curve1.curve());
        draw_curve(patt, ras_img, ren_img, p2, m_curve2.curve());
        draw_curve(patt, ras_img, ren_img, p3, m_curve3.curve());
        draw_curve(patt, ras_img, ren_img, p4, m_curve4.curve());
        draw_curve(patt, ras_img, ren_img, p5, m_curve5.curve());
        draw_curve(patt, ras_img, ren_img, p6, m_curve6.curve());
        draw_curve(patt, ras_img, ren_img, p7, m_curve7.curve());
        draw_curve(patt, ras_img, ren_img, p8, m_curve8.curve());
        draw_curve(patt, ras_img, ren_img, p9, m_curve9.curve());

        agg::render_ctrl(ras, sl, ren_base, m_curve1);
        agg::render_ctrl(ras, sl, ren_base, m_curve2);
        agg::render_ctrl(ras, sl, ren_base, m_curve3);
        agg::render_ctrl(ras, sl, ren_base, m_curve4);
        agg::render_ctrl(ras, sl, ren_base, m_curve5);
        agg::render_ctrl(ras, sl, ren_base, m_curve6);
        agg::render_ctrl(ras, sl, ren_base, m_curve7);
        agg::render_ctrl(ras, sl, ren_base, m_curve8);
        agg::render_ctrl(ras, sl, ren_base, m_curve9);

        agg::render_ctrl(ras, sl, ren_base, m_scale_x);
        agg::render_ctrl(ras, sl, ren_base, m_start_x);
    }


    virtual void on_key(int x, int y, unsigned key, unsigned flags)
    {
        if(key == ' ')
        {
            FILE* fd = fopen(full_file_name("coord"), "w");
            fprintf(fd, "%.0f, %.0f, %.0f, %.0f, %.0f, %.0f, %.0f, %.0f", 
                         m_curve1.x1(), m_curve1.y1(), 
                         m_curve1.x2(), m_curve1.y2(), 
                         m_curve1.x3(), m_curve1.y3(), 
                         m_curve1.x4(), m_curve1.y4());
            fclose(fd);
        }
    }

    virtual void on_ctrl_change()
    {
    }
};


int agg_main(int argc, char* argv[])
{
    the_application app(agg::pix_format_bgr24, flip_y);
    app.caption("AGG Example. Drawing Lines with Image Patterns");

    if(!app.load_img(0, "1") ||
       !app.load_img(1, "2") ||
       !app.load_img(2, "3") ||
       !app.load_img(3, "4") ||
       !app.load_img(4, "5") ||
       !app.load_img(5, "6") ||
       !app.load_img(6, "7") ||
       !app.load_img(7, "8") ||
       !app.load_img(8, "9"))
    {
        char buf[256];
        sprintf(buf, "There must be files 1%s...9%s\n"
                     "Download and unzip:\n"
                     "http://www.antigrain.com/line_patterns.bmp.zip\n"
                     "or\n"
                     "http://www.antigrain.com/line_patterns.ppm.tar.gz\n", 
                     app.img_ext(), app.img_ext());
        app.message(buf);
        return 1;
    }

    if(app.init(500, 450, agg::window_resize))
    {
        return app.run();
    }

    return 1;
}


