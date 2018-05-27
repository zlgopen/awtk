#include <math.h>
#include <stdio.h>
#include <time.h>
#include "agg_math.h"
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
#include "agg_path_storage.h"
#include "agg_pixfmt_rgb.h"
#include "ctrl/agg_slider_ctrl.h"
#include "ctrl/agg_bezier_ctrl.h"
#include "platform/agg_platform_support.h"

#define AGG_BGRA32
//#define AGG_BGRA128
#include "pixel_formats.h"

enum flip_y_e { flip_y = true };


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


class pattern_src_brightness_to_alpha
{
public:
    pattern_src_brightness_to_alpha(agg::rendering_buffer& rb) : 
        m_rb(&rb), m_pf(*m_rb) {}

    unsigned width()  const { return m_pf.width();  }
    unsigned height() const { return m_pf.height(); }
    color_type pixel(int x, int y) const
    {
        color_type c = m_pf.pixel(x, y);
        // It's a bit of a hack, but we can treat the 8-bit alpha value as a cover.
        color_type::calc_type sum = c.r + c.g + c.b;
        int i = int(sum * sizeof(brightness_to_alpha) / (3 * color_type::full_value()));
        agg::cover_type cover = brightness_to_alpha[i];
        c.a = color_type::mult_cover(color_type::full_value(), cover);
        return c;
    }

private:
    agg::rendering_buffer* m_rb;
    pixfmt m_pf;
};


class the_application : public agg::platform_support
{
    agg::srgba8 m_ctrl_color;
    agg::polygon_ctrl<color_type> m_line1;
    agg::slider_ctrl<color_type>  m_scale_x;
    agg::slider_ctrl<color_type>  m_start_x;
    agg::trans_affine             m_scale;

public:
    typedef agg::renderer_base<pixfmt> renderer_base;
    typedef agg::renderer_scanline_aa_solid<renderer_base> renderer_scanline;
    typedef agg::rasterizer_scanline_aa<agg::rasterizer_sl_clip_int_sat> rasterizer_scanline;
    typedef agg::scanline_p8 scanline;


    the_application(agg::pix_format_e format, bool flip_y) :
        agg::platform_support(format, flip_y),
        m_ctrl_color(agg::rgba(0, 0.3, 0.5, 0.3)),
        m_line1(5),
        m_scale_x(5.0,   5.0, 240.0, 12.0, !flip_y),
        m_start_x(250.0, 5.0, 495.0, 12.0, !flip_y)
    {
        m_line1.line_color(m_ctrl_color);
        m_line1.xn(0) = 20;
        m_line1.yn(0) = 20;
        m_line1.xn(1) = 500-20;
        m_line1.yn(1) = 500-20;
        m_line1.xn(2) = 500-60;
        m_line1.yn(2) = 20;
        m_line1.xn(3) = 40;
        m_line1.yn(3) = 500-40;
        m_line1.xn(4) = 100;
        m_line1.yn(4) = 300;
        m_line1.close(false);

        add_ctrl(m_line1);
        m_line1.transform(m_scale);

        m_scale_x.label("Scale X=%.2f");
        m_scale_x.range(0.2, 3.0);
        m_scale_x.value(1.0);
        add_ctrl(m_scale_x);
        m_scale_x.no_transform();

        m_start_x.label("Start X=%.2f");
        m_start_x.range(0.0, 10.0);
        m_start_x.value(0.0);
        add_ctrl(m_start_x);
        m_start_x.no_transform();
    }


    template<class Rasterizer, class Renderer>
    void draw_polyline(Rasterizer& ras, 
                       Renderer& ren, 
                       const double* polyline, 
                       int num_points)
    {
        agg::poly_plain_adaptor<double> vs(polyline, num_points, m_line1.close());
        agg::conv_transform<agg::poly_plain_adaptor<double> > trans(vs, m_scale);
        ras.add_path(trans);
    }


    virtual void on_draw()
    {
        pixfmt pf(rbuf_window());
        renderer_base ren_base(pf);
        ren_base.clear(agg::rgba(0.5, 0.75, 0.85));
        renderer_scanline ren(ren_base);

        rasterizer_scanline ras;
        scanline sl;

        ras.clip_box(0, 0, width(), height());

        // Pattern source. Must have an interface:
        // width() const
        // height() const
        // pixel(int x, int y) const
        // Any agg::renderer_base<> or derived
        // is good for the use as a source.
        //-----------------------------------
        pattern_src_brightness_to_alpha p1(rbuf_img(0));

        agg::pattern_filter_bilinear_rgba<color_type> fltr;           // Filtering functor

        // agg::line_image_pattern is the main container for the patterns. It creates
        // a copy of the patterns extended according to the needs of the filter.
        // agg::line_image_pattern can operate with arbitrary image width, but if the 
        // width of the pattern is power of 2, it's better to use the modified
        // version agg::line_image_pattern_pow2 because it works about 15-25 percent
        // faster than agg::line_image_pattern (because of using simple masking instead 
        // of expensive '%' operation). 
        typedef agg::line_image_pattern<agg::pattern_filter_bilinear_rgba<color_type> > pattern_type;
        typedef agg::renderer_base<pixfmt> base_ren_type;
        typedef agg::renderer_outline_image<base_ren_type, pattern_type> renderer_img_type;
        typedef agg::rasterizer_outline_aa<renderer_img_type, agg::line_coord_sat> rasterizer_img_type;

        typedef agg::renderer_outline_aa<base_ren_type> renderer_line_type;
        typedef agg::rasterizer_outline_aa<renderer_line_type, agg::line_coord_sat> rasterizer_line_type;


        //-- Create with specifying the source
        //pattern_type patt(fltr, src);   

        //-- Create uninitialized and set the source
        pattern_type patt(fltr);        
        patt.create(p1);
        renderer_img_type ren_img(ren_base, patt);
        rasterizer_img_type ras_img(ren_img);


        //-- create uninitialized and set parameters
        agg::line_profile_aa profile;
        profile.smoother_width(10.0);                    //optional
        profile.width(8.0);                              //mandatory!
        renderer_line_type ren_line(ren_base, profile);
        ren_line.color(agg::srgba8(0,0,127));            //mandatory!
        rasterizer_line_type ras_line(ren_line);
        ras_line.round_cap(true);                       //optional
        //ras_line.line_join(agg::outline_no_join);     //optional

        // Calculate the dilation value so that, the line caps were
        // drawn correctly.
        //---------------
        double w2 = 9.0;//p1.height() / 2 + 2;


        // Set the clip box a bit bigger than you expect. You need it
        // to draw the clipped line caps correctly. The correct result
        // is achieved with raster clipping.
        //------------------------
        ren_img.scale_x(m_scale_x.value());
        ren_img.start_x(m_start_x.value());
        ren_img.clip_box (50-w2, 50-w2, width()-50+w2, height()-50+w2);
        ren_line.clip_box(50-w2, 50-w2, width()-50+w2, height()-50+w2);

        // First, draw polyline without raster clipping just to show the idea
        //------------------------
        draw_polyline(ras_line, ren_line, m_line1.polygon(), m_line1.num_points());
        draw_polyline(ras_img,  ren_img,  m_line1.polygon(), m_line1.num_points());

        // Clear the area, almost opaque, but not completely
        //------------------------
        ren_base.blend_bar(0, 0, (int)width(), (int)height(), agg::rgba(1,1,1), 200);


        // Set the raster clip box and then, draw again. 
        // In reality there shouldn't be two calls above. 
        // It's done only for demonstration
        //------------------------
        ren_base.clip_box((int)50, (int)50, (int)width()-50, (int)height()-50);

        // This "copy_bar" is also for demonstration only
        //------------------------
        ren_base.copy_bar(0, 0, (int)width(), (int)height(), agg::rgba(1,1,1));

        // Finally draw polyline correctly clipped: We use double clipping, 
        // first is vector clipping, with extended clip box, second is raster 
        // clipping with normal clip box.
        //------------------------
        ren_img.scale_x(m_scale_x.value());
        ren_img.start_x(m_start_x.value());
        draw_polyline(ras_line, ren_line, m_line1.polygon(), m_line1.num_points());
        draw_polyline(ras_img, ren_img,   m_line1.polygon(), m_line1.num_points());


        // Reset clipping and draw the controls and stuff
        ren_base.reset_clipping(true);

        m_line1.line_width(1/m_scale.scale());
        m_line1.point_radius(5/m_scale.scale());

        agg::render_ctrl(ras, sl, ren_base, m_line1);
        agg::render_ctrl(ras, sl, ren_base, m_scale_x);
        agg::render_ctrl(ras, sl, ren_base, m_start_x);


        char buf[256]; 
        agg::gsv_text t;
        t.size(10.0);

        agg::conv_stroke<agg::gsv_text> pt(t);
        pt.width(1.5);
        pt.line_cap(agg::round_cap);

        const double* p = m_line1.polygon();
        sprintf(buf, "Len=%.2f", agg::calc_distance(p[0], p[1], p[2], p[3]) * m_scale.scale());

        t.start_point(10.0, 30.0);
        t.text(buf);

        ras.add_path(pt);
        ren.color(agg::rgba(0,0,0));
        agg::render_scanlines(ras, sl, ren);

    }


    virtual void on_key(int x, int y, unsigned key, unsigned flags)
    {
        if(key == '+' || key == agg::key_kp_plus)
        {
            m_scale *= agg::trans_affine_translation(-x, -y);
            m_scale *= agg::trans_affine_scaling(1.1);
            m_scale *= agg::trans_affine_translation(x, y);
            force_redraw();
        }

        if(key == '-' || key == agg::key_kp_minus)
        {
            m_scale *= agg::trans_affine_translation(-x, -y);
            m_scale *= agg::trans_affine_scaling(1/1.1);
            m_scale *= agg::trans_affine_translation(x, y);
            force_redraw();
        }
    }

    virtual void on_ctrl_change()
    {
    }
};


int agg_main(int argc, char* argv[])
{
    the_application app(pix_format, flip_y);
    app.caption("AGG Example. Clipping Lines with Image Patterns");

    if(!app.load_img(0, "1"))
    {
        char buf[256];
        sprintf(buf, "There must be file 1%s\n", app.img_ext());
        app.message(buf);
        return 1;
    }

    if(app.init(500, 500, agg::window_resize))
    {
        return app.run();
    }

    return 1;
}


