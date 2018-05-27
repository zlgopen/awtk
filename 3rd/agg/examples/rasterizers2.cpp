#include <math.h>
#include <stdio.h>
#include <time.h>
#include "agg_basics.h"
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_rasterizer_outline.h"
#include "agg_conv_transform.h"
#include "agg_conv_stroke.h"
#include "agg_scanline_p.h"
#include "agg_renderer_scanline.h"
#include "agg_renderer_primitives.h"
#include "agg_rasterizer_outline.h"
#include "agg_rasterizer_outline_aa.h"
#include "agg_pattern_filters_rgba.h"
#include "agg_renderer_outline_aa.h"
#include "agg_renderer_outline_image.h"
#include "ctrl/agg_slider_ctrl.h"
#include "ctrl/agg_cbox_ctrl.h"
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

static const agg::int32u pixmap_chain[] = 
{
    16, 7,
    0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0xb4c29999, 0xff9a5757, 0xff9a5757, 0xff9a5757, 0xff9a5757, 0xff9a5757, 0xff9a5757, 0xb4c29999, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff,
    0x00ffffff, 0x00ffffff, 0x0cfbf9f9, 0xff9a5757, 0xff660000, 0xff660000, 0xff660000, 0xff660000, 0xff660000, 0xff660000, 0xff660000, 0xff660000, 0xb4c29999, 0x00ffffff, 0x00ffffff, 0x00ffffff, 
    0x00ffffff, 0x5ae0cccc, 0xffa46767, 0xff660000, 0xff975252, 0x7ed4b8b8, 0x5ae0cccc, 0x5ae0cccc, 0x5ae0cccc, 0x5ae0cccc, 0xa8c6a0a0, 0xff7f2929, 0xff670202, 0x9ecaa6a6, 0x5ae0cccc, 0x00ffffff, 
    0xff660000, 0xff660000, 0xff660000, 0xff660000, 0xff660000, 0xff660000, 0xa4c7a2a2, 0x3affff00, 0x3affff00, 0xff975151, 0xff660000, 0xff660000, 0xff660000, 0xff660000, 0xff660000, 0xff660000, 
    0x00ffffff, 0x5ae0cccc, 0xffa46767, 0xff660000, 0xff954f4f, 0x7ed4b8b8, 0x5ae0cccc, 0x5ae0cccc, 0x5ae0cccc, 0x5ae0cccc, 0xa8c6a0a0, 0xff7f2929, 0xff670202, 0x9ecaa6a6, 0x5ae0cccc, 0x00ffffff, 
    0x00ffffff, 0x00ffffff, 0x0cfbf9f9, 0xff9a5757, 0xff660000, 0xff660000, 0xff660000, 0xff660000, 0xff660000, 0xff660000, 0xff660000, 0xff660000, 0xb4c29999, 0x00ffffff, 0x00ffffff, 0x00ffffff, 
    0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0xb4c29999, 0xff9a5757, 0xff9a5757, 0xff9a5757, 0xff9a5757, 0xff9a5757, 0xff9a5757, 0xb4c29999, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff
};



namespace agg
{
    class pattern_pixmap_argb32
    {
    public:
        typedef rgba color_type;

        pattern_pixmap_argb32(const int32u* pixmap) : m_pixmap(pixmap) {}

        unsigned width()  const { return m_pixmap[0]; }
        unsigned height() const { return m_pixmap[1]; }

        rgba pixel(int x, int y) const
        {
            int32u p = m_pixmap[y * width() + x + 2];
            srgba8 c((p >> 16) & 0xFF, (p >> 8) & 0xFF, p & 0xFF, p >> 24);
            return rgba(c).premultiply();
        }
    private:
        const int32u* m_pixmap;
    };
}



class spiral
{
public:
    spiral(double x, double y, double r1, double r2, double step, double start_angle=0) :
        m_x(x), 
        m_y(y), 
        m_r1(r1), 
        m_r2(r2), 
        m_step(step), 
        m_start_angle(start_angle),
        m_angle(start_angle),
        m_da(agg::deg2rad(8.0)),
        m_dr(m_step / 45.0)
    {
    }

    void rewind(unsigned) 
    { 
        m_angle = m_start_angle; 
        m_curr_r = m_r1; 
        m_start = true; 
    }

    unsigned vertex(double* x, double* y)
    {
        if(m_curr_r > m_r2) return agg::path_cmd_stop;

        *x = m_x + cos(m_angle) * m_curr_r;
        *y = m_y + sin(m_angle) * m_curr_r;
        m_curr_r += m_dr;
        m_angle += m_da;
        if(m_start) 
        {
            m_start = false;
            return agg::path_cmd_move_to;
        }
        return agg::path_cmd_line_to;
    }

private:
    double m_x;
    double m_y;
    double m_r1;
    double m_r2;
    double m_step;
    double m_start_angle;

    double m_angle;
    double m_curr_r;
    double m_da;
    double m_dr;
    bool   m_start;
};


struct roundoff
{
    void transform(double* x, double* y) const
    { 
        *x = floor(*x);
        *y = floor(*y);
    }
};


class the_application : public agg::platform_support
{
    agg::slider_ctrl<color_type> m_step;
    agg::slider_ctrl<color_type> m_width;
    agg::cbox_ctrl<color_type>   m_test;
    agg::cbox_ctrl<color_type>   m_rotate;
    agg::cbox_ctrl<color_type>   m_accurate_joins;
    agg::cbox_ctrl<color_type>   m_scale_pattern;
    double m_start_angle;


public:
    typedef agg::renderer_base<pixfmt_pre> renderer_base;
    typedef agg::renderer_scanline_aa_solid<renderer_base> renderer_aa;
    typedef agg::renderer_primitives<renderer_base> renderer_prim;
    typedef agg::rasterizer_outline<renderer_prim> rasterizer_outline;
    typedef agg::rasterizer_scanline_aa<> rasterizer_scanline;
    typedef agg::scanline_p8 scanline;
    typedef agg::renderer_outline_aa<renderer_base> renderer_oaa;
    typedef agg::pattern_filter_bilinear_rgba<color_type> pattern_filter;
    typedef agg::line_image_pattern_pow2<pattern_filter> image_pattern;
    typedef agg::renderer_outline_image<renderer_base, image_pattern> renderer_img;
    typedef agg::rasterizer_outline_aa<renderer_oaa> rasterizer_outline_aa;
    typedef agg::rasterizer_outline_aa<renderer_img> rasterizer_outline_img;
    


    the_application(agg::pix_format_e format, bool flip_y) :
        agg::platform_support(format, flip_y),
        m_step(10.0, 10.0 + 4.0, 150.0, 10.0 + 8.0 + 4.0, !flip_y),
        m_width(150.0 + 10.0, 10.0 + 4.0, 400 - 10.0, 10.0 + 8.0 + 4.0, !flip_y),
        m_test(10.0, 10.0 + 4.0 + 16.0,    "Test Performance", !flip_y),
        m_rotate(130 + 10.0, 10.0 + 4.0 + 16.0,    "Rotate", !flip_y),
        m_accurate_joins(200 + 10.0, 10.0 + 4.0 + 16.0, "Accurate Joins", !flip_y),
        m_scale_pattern(310 + 10.0, 10.0 + 4.0 + 16.0, "Scale Pattern", !flip_y),
        m_start_angle(0.0)
    {
        add_ctrl(m_step);
        m_step.range(0.0, 2.0);
        m_step.value(0.1);
        m_step.label("Step=%1.2f");
        m_step.no_transform();

        add_ctrl(m_width);
        m_width.range(0.0, 14.0);
        m_width.value(3.0);
        m_width.label("Width=%1.2f");
        m_width.no_transform();

        add_ctrl(m_test);
        m_test.text_size(9.0, 7.0);
        m_test.no_transform();

        add_ctrl(m_rotate);
        m_rotate.text_size(9.0, 7.0);
        m_rotate.no_transform();

        add_ctrl(m_accurate_joins);
        m_accurate_joins.text_size(9.0, 7.0);
        m_accurate_joins.no_transform();

        add_ctrl(m_scale_pattern);
        m_scale_pattern.text_size(9.0, 7.0);
        m_scale_pattern.no_transform();
        m_scale_pattern.status(true);
    }



    void draw_aliased_pix_accuracy(rasterizer_outline& ras, renderer_prim& prim)
    {
        spiral s1(width()/5, height()/4+50, 5, 70, 16, m_start_angle);
        roundoff rn;
        agg::conv_transform<spiral, roundoff> trans(s1, rn);
        prim.line_color(agg::rgba(0.4, 0.3, 0.1));
        ras.add_path(trans);
    }

    void draw_aliased_subpix_accuracy(rasterizer_outline& ras, renderer_prim& prim)
    {
        spiral s2(width()/2, height()/4+50, 5, 70, 16, m_start_angle);
        prim.line_color(agg::rgba(0.4, 0.3, 0.1));
        ras.add_path(s2);
    }

    void draw_anti_aliased_outline(rasterizer_outline_aa& ras, renderer_oaa& ren)
    {
        spiral s3(width()/5, height() - height()/4 + 20, 5, 70, 16, m_start_angle);
        ren.color(agg::rgba(0.4, 0.3, 0.1));
        ras.add_path(s3);
    }
    
    void draw_anti_aliased_scanline(rasterizer_scanline& ras, scanline& sl, renderer_aa& ren)
    {
        spiral s4(width()/2, height() - height()/4 + 20, 5, 70, 16, m_start_angle);
        agg::conv_stroke<spiral> stroke(s4);
        stroke.width(m_width.value());
        stroke.line_cap(agg::round_cap);
        ren.color(agg::rgba(0.4, 0.3, 0.1));
        ras.add_path(stroke);
        agg::render_scanlines(ras, sl, ren);
    }

    void draw_anti_aliased_outline_img(rasterizer_outline_img& ras, renderer_img& ren)
    {
        spiral s5(width() - width()/5, height() - height()/4 + 20, 5, 70, 16, m_start_angle);
        ras.add_path(s5);
    }
    


    void text(rasterizer_scanline& ras, 
              scanline& sl,
              renderer_aa& ren, 
              double x, double y, const char* txt)
    {
        agg::gsv_text t;
        t.size(8);
        t.text(txt);
        t.start_point(x, y);
        agg::conv_stroke<agg::gsv_text> stroke(t);
        stroke.width(0.7);
        ras.add_path(stroke);
        ren.color(agg::rgba(0,0,0));
        agg::render_scanlines(ras, sl, ren);
    }




    virtual void on_draw()
    {
        pixfmt_pre pf(rbuf_window());
        renderer_base ren_base(pf);
        renderer_aa ren_aa(ren_base);
        renderer_prim ren_prim(ren_base);
        rasterizer_scanline ras_aa;
        scanline sl;
        rasterizer_outline ras_al(ren_prim);
        agg::line_profile_aa prof;
        prof.width(m_width.value());
        renderer_oaa ren_oaa(ren_base, prof);
        rasterizer_outline_aa ras_oaa(ren_oaa);
        ras_oaa.line_join(m_accurate_joins.status() ? 
                              agg::outline_miter_accurate_join :
                              agg::outline_round_join); 
        ras_oaa.round_cap(true);

        pattern_filter filter;
        agg::pattern_pixmap_argb32 src(pixmap_chain);
        agg::line_image_scale<agg::pattern_pixmap_argb32> src_scaled(src, m_width.value());
        image_pattern pattern(filter);
        if(m_scale_pattern.status())
        {
            pattern.create(src_scaled);
        }
        else
        {
            pattern.create(src);
        }
        renderer_img ren_img(ren_base, pattern);
        if(m_scale_pattern.status())
        {
            ren_img.scale_x(m_width.value() / src.height());
        }
        rasterizer_outline_img ras_img(ren_img);

        ren_base.clear(agg::rgba(1.0, 1.0, 0.95));

        draw_aliased_pix_accuracy(ras_al, ren_prim);
        draw_aliased_subpix_accuracy(ras_al, ren_prim);
        draw_anti_aliased_outline(ras_oaa, ren_oaa);
        draw_anti_aliased_scanline(ras_aa, sl, ren_aa);
        draw_anti_aliased_outline_img(ras_img, ren_img);

        text(ras_aa, sl, ren_aa, 50, 80, "Bresenham lines,\n\nregular accuracy");
        text(ras_aa, sl, ren_aa, width()/2-50, 80, "Bresenham lines,\n\nsubpixel accuracy");
        text(ras_aa, sl, ren_aa, 50, height()/2+50, "Anti-aliased lines");
        text(ras_aa, sl, ren_aa, width()/2-50, height()/2+50, "Scanline rasterizer");
        text(ras_aa, sl, ren_aa, width() - width()/5 - 50, height()/2+50, "Arbitrary Image Pattern");

        // The source colors of controls are "plain".
        pixfmt pf2(rbuf_window());
        agg::renderer_base<pixfmt> ren_base2(pf2);
        agg::render_ctrl(ras_aa, sl, ren_base2, m_step);
        agg::render_ctrl(ras_aa, sl, ren_base2, m_width);
        agg::render_ctrl(ras_aa, sl, ren_base2, m_test);
        agg::render_ctrl(ras_aa, sl, ren_base2, m_rotate);
        agg::render_ctrl(ras_aa, sl, ren_base2, m_accurate_joins);
        agg::render_ctrl(ras_aa, sl, ren_base2, m_scale_pattern);



/*
// An example of using anti-aliased outline rasterizer. 
// Uncomment it to see the result
//

// Includes:
//#include "agg_pixfmt_rgb.h"           // or another
//#include "agg_renderer_outline_aa.h"
//#include "agg_rasterizer_outline_aa.h"

typedef agg::renderer_base<agg::pixfmt_bgr24> base_ren_type;
typedef agg::renderer_outline_aa<base_ren_type> renderer_type;
typedef agg::rasterizer_outline_aa<renderer_type> rasterizer_type;

double width = 5.0;
//-- create with specifying width
//-- min_width=1.0, smoother_width=1.0
//agg::line_profile_aa(width, agg::gamma_none()); 

//-- create uninitialized and set parameters
agg::line_profile_aa profile;
profile.gamma(agg::gamma_power(1.2));  //optional
profile.min_width(0.75);               //optional
profile.smoother_width(3.0);           //optional
profile.width(width);                  //mandatory!

agg::pixfmt_bgr24 pixf(rbuf_window()); //or another
base_ren_type base_ren(pixf);
renderer_type ren(base_ren, profile);
ren.color(agg::srgba8(0,0,0));          //mandatory!
rasterizer_type ras(ren);
ras.round_cap(true);                   //optional
ras.accurate_join(true);               //optional

//-- move_to/line_to interface
ras.move_to_d(100, 100);
ras.line_to_d(150, 200);
ras.render(false);     //false means "don't close 
                       //the polygon", i.e. polyline

//-- add_path interface
//-- doesn't require invoking render()
//ras.add_path(some_path);
*/




/*
// An example of using image pattern outline rasterizer
// Uncomment it to see the result
//

// Includes:
//#include "agg_pixfmt_rgb.h"           // or another
//#include "agg_pattern_filters_rgba.h"  // for all rgba-8-bit color formats
//#include "agg_renderer_outline_image.h"
//#include "agg_rasterizer_outline_aa.h"


agg::pattern_filter_bilinear_rgba8 fltr;          // Filtering functor

agg::pattern_pixmap_argb32 patt_src(pixmap_chain); // Source. Must have an interface:
                                                   // width() const
                                                   // height() const
                                                   // pixel(int x, int y) const
                                                   // Any agg::renderer_base<> or derived
                                                   // is good for the use as a source.

// agg::line_image_pattern is the main container for the patterns. It creates
// a copy of the patterns extended according to the needs of the filter.
// agg::line_image_pattern can operate with arbitrary image width, but if the 
// width of the pattern is power of 2, it's better to use the modified
// version agg::line_image_pattern_pow2 because it works about 15-25 percent
// faster than agg::line_image_pattern (because of using simple masking instead 
// of expensive '%' operation). 
typedef agg::line_image_pattern_pow2<agg::pattern_filter_bilinear_rgba8> pattern_type;

typedef agg::renderer_base<agg::pixfmt_bgr24> base_ren_type;
typedef agg::renderer_outline_image<base_ren_type, pattern_type> renderer_type;
typedef agg::rasterizer_outline_aa<renderer_type>                rasterizer_type;

//-- Create with specifying the source
pattern_type patt(fltr, src);   

//-- Create uninitialized and set the source
//pattern_type patt(fltr);        
//patt.create(src);

agg::pixfmt_bgr24 pixf(rbuf_window()); //or another
base_ren_type base_ren(pixf);
renderer_type ren(base_ren, patt);
//ren.scale_x(1.3);                    // Optional
rasterizer_type ras(ren);

//-- move_to/line_to interface
ras.move_to_d(100, 150);
ras.line_to_d(0, 0);
ras.line_to_d(300, 200);
//ras.line_to_d(10, 10);
ras.render(false);     //false means "don't close 
                       //the polygon", i.e. polyline

//-- add_path interface
//-- doesn't require invoking render()
//ras.add_path(some_path);
*/

    }


    virtual void on_idle()
    {
        m_start_angle += agg::deg2rad(m_step.value());
        if(m_start_angle > agg::deg2rad(360.0)) m_start_angle -= agg::deg2rad(360.0);
        force_redraw();
    }

    virtual void on_ctrl_change()
    {
        wait_mode(!m_rotate.status());

        if(m_test.status())
        {
            on_draw();
            update_window();

            pixfmt_pre pf(rbuf_window());
            renderer_base ren_base(pf);
            renderer_aa ren_aa(ren_base);
            renderer_prim ren_prim(ren_base);
            rasterizer_scanline ras_aa;
            scanline sl;
            rasterizer_outline ras_al(ren_prim);
            agg::line_profile_aa prof;
            prof.width(m_width.value());
            renderer_oaa ren_oaa(ren_base, prof);
            rasterizer_outline_aa ras_oaa(ren_oaa);
            ras_oaa.line_join(m_accurate_joins.status() ? 
                                  agg::outline_miter_accurate_join :
                                  agg::outline_round_join); 
            ras_oaa.round_cap(true);

            pattern_filter filter;
            agg::pattern_pixmap_argb32 src(pixmap_chain);
            agg::line_image_scale<agg::pattern_pixmap_argb32> src_scaled(src, m_width.value());
            image_pattern pattern(filter);
            if(m_scale_pattern.status())
            {
                pattern.create(src_scaled);
            }
            else
            {
                pattern.create(src);
            }
            renderer_img ren_img(ren_base, pattern);
            if(m_scale_pattern.status())
            {
                ren_img.scale_x(src.height() / m_width.value());
            }
            rasterizer_outline_img ras_img(ren_img);

            unsigned i;

            start_timer();
            for(i = 0; i < 200; i++)
            {
                draw_aliased_subpix_accuracy(ras_al, ren_prim);
                m_start_angle += agg::deg2rad(m_step.value());
            }
            double t2 = elapsed_time();

            start_timer();
            for(i = 0; i < 200; i++)
            {
                draw_anti_aliased_outline(ras_oaa, ren_oaa);
                m_start_angle += agg::deg2rad(m_step.value());
            }
            double t3 = elapsed_time();

            start_timer();
            for(i = 0; i < 200; i++)
            {
                draw_anti_aliased_scanline(ras_aa, sl, ren_aa);
                m_start_angle += agg::deg2rad(m_step.value());
            }
            double t4 = elapsed_time();

            start_timer();
            for(i = 0; i < 200; i++)
            {
                draw_anti_aliased_outline_img(ras_img, ren_img);
                m_start_angle += agg::deg2rad(m_step.value());
            }
            double t5 = elapsed_time();

            m_test.status(false);
            force_redraw();
            char buf[256];
            sprintf(buf, "Aliased=%1.2fms, Anti-Aliased=%1.2fms, Scanline=%1.2fms, Image-Pattern=%1.2fms", 
                          t2, t3, t4, t5);
            message(buf);
        }
    }
};


int agg_main(int argc, char* argv[])
{
    the_application app(pix_format, flip_y);
    app.caption("AGG Example. Line Join");

    if(app.init(500, 450, 0))
    {
        return app.run();
    }

    return 1;
}


