#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_u.h"
#include "agg_renderer_scanline.h"
#include "agg_path_storage.h"
#include "agg_conv_transform.h"
#include "agg_trans_affine.h"
#include "agg_span_allocator.h"
#include "agg_span_interpolator_linear.h"
#include "agg_span_interpolator_trans.h"
#include "agg_span_interpolator_persp.h"
#include "agg_span_subdiv_adaptor.h"
#include "agg_image_accessors.h"
#include "agg_gamma_lut.h"
#include "ctrl/agg_rbox_ctrl.h"
#include "ctrl/agg_slider_ctrl.h"
#include "platform/agg_platform_support.h"
#include "interactive_polygon.h"

#define AGG_BGRA32
//#define AGG_BGRA64
//#define AGG_BGRA128
#include "pixel_formats.h"

int global_offset = 0;


enum flip_y_e { flip_y = true };

agg::rasterizer_scanline_aa<> g_rasterizer;
agg::scanline_u8  g_scanline;
double            g_x1 = 0;
double            g_y1 = 0;
double            g_x2 = 0;
double            g_y2 = 0;

#include "agg_span_image_filter_rgba.h"
#define image_filter_2x2_type      agg::span_image_filter_rgba_2x2
#define image_resample_affine_type agg::span_image_resample_rgba_affine
#define image_resample_type        agg::span_image_resample_rgba

typedef color_type::value_type                         value_type;
typedef agg::renderer_base<pixfmt>                     renderer_base;
typedef agg::renderer_base<pixfmt_pre>                 renderer_base_pre;
typedef agg::renderer_scanline_aa_solid<renderer_base> renderer_solid;

class the_application : public agg::platform_support
{
public:
    agg::interactive_polygon     m_quad;
    agg::rbox_ctrl<agg::rgba>    m_trans_type;
    agg::slider_ctrl<agg::rgba>  m_blur;
    agg::slider_ctrl<agg::rgba>  m_gamma;

    the_application(agg::pix_format_e format, bool flip_y) :
        agg::platform_support(format, flip_y),
        m_quad(4, 5.0),
        m_trans_type(400, 5.0, 430+170.0, 100.0, !flip_y),
        m_blur (5.0, 5.0+15*0, 400-5, 10.0+15*0, !flip_y),
        m_gamma(5.0, 5.0+15*1, 400-5, 10.0+15*1, !flip_y)
    {
        m_trans_type.text_size(7);
        m_trans_type.add_item("Affine No Resample");
        m_trans_type.add_item("Affine Resample");
        m_trans_type.add_item("Perspective No Resample LERP");
        m_trans_type.add_item("Perspective No Resample Exact");
        m_trans_type.add_item("Perspective Resample LERP");
        m_trans_type.add_item("Perspective Resample Exact");
        m_trans_type.cur_item(4);
        add_ctrl(m_trans_type);

        m_blur.range(0.5, 5.0);
        m_blur.value(1.0);
        m_blur.label("Blur=%.3f");
        add_ctrl(m_blur);
    }


    virtual void on_init()
    {
        g_x1 = 0.0;
        g_y1 = 0.0;
        g_x2 = rbuf_img(0).width();
        g_y2 = rbuf_img(0).height();

        double x1 = g_x1;// * 100.0;
        double y1 = g_y1;// * 100.0;
        double x2 = g_x2;// * 100.0;
        double y2 = g_y2;// * 100.0;

        double dx = width()  / 2.0 - (x2 - x1) / 2.0;
        double dy = height() / 2.0 - (y2 - y1) / 2.0;
        m_quad.xn(0) = floor(x1 + dx);
        m_quad.yn(0) = floor(y1 + dy);// - 150;
        m_quad.xn(1) = floor(x2 + dx);
        m_quad.yn(1) = floor(y1 + dy);// - 110;
        m_quad.xn(2) = floor(x2 + dx);
        m_quad.yn(2) = floor(y2 + dy);// - 300;
        m_quad.xn(3) = floor(x1 + dx);
        m_quad.yn(3) = floor(y2 + dy);// - 200;
    }

    virtual void on_draw()
    {
        pixfmt            pixf(rbuf_window());
        pixfmt_pre        pixf_pre(rbuf_window());
        renderer_base     rb(pixf);
        renderer_base_pre rb_pre(pixf_pre);

        renderer_solid r(rb);

        rb.clear(agg::rgba(1, 1, 1));

        if(m_trans_type.cur_item() < 2)
        {
            // For the affine parallelogram transformations we
            // calculate the 4-th (implicit) point of the parallelogram
            m_quad.xn(3) = m_quad.xn(0) + (m_quad.xn(2) - m_quad.xn(1));
            m_quad.yn(3) = m_quad.yn(0) + (m_quad.yn(2) - m_quad.yn(1));
        }

        //--------------------------
        // Render the "quad" tool and controls
        g_rasterizer.add_path(m_quad);
        r.color(agg::rgba(0, 0.3, 0.5, 0.5));
        agg::render_scanlines(g_rasterizer, g_scanline, r);

        // Prepare the polygon to rasterize. Here we need to fill
        // the destination (transformed) polygon.
        g_rasterizer.clip_box(0, 0, width(), height());
        g_rasterizer.reset();
        int b = 0;
        g_rasterizer.move_to_d(m_quad.xn(0)-b, m_quad.yn(0)-b);
        g_rasterizer.line_to_d(m_quad.xn(1)+b, m_quad.yn(1)-b);
        g_rasterizer.line_to_d(m_quad.xn(2)+b, m_quad.yn(2)+b);
        g_rasterizer.line_to_d(m_quad.xn(3)-b, m_quad.yn(3)+b);

        typedef agg::span_allocator<color_type> span_alloc_type;
        span_alloc_type sa;
        agg::image_filter_bilinear filter_kernel;
        agg::image_filter_lut filter(filter_kernel, true);

        pixfmt pixf_img(rbuf_img(0));
        typedef agg::image_accessor_clone<pixfmt> source_type;
        source_type source(pixf_img);

        start_timer();
        switch(m_trans_type.cur_item())
        {
            case 0:
            {
                agg::trans_affine tr(m_quad.polygon(), g_x1, g_y1, g_x2, g_y2);

                typedef agg::span_interpolator_linear<agg::trans_affine> interpolator_type;
                interpolator_type interpolator(tr);

                typedef image_filter_2x2_type<source_type, 
                                              interpolator_type> span_gen_type;
                span_gen_type sg(source, interpolator, filter);
                agg::render_scanlines_aa(g_rasterizer, g_scanline, rb_pre, sa, sg);
                break;
            }

            case 1:
            {
                agg::trans_affine tr(m_quad.polygon(), g_x1, g_y1, g_x2, g_y2);

                typedef agg::span_interpolator_linear<agg::trans_affine> interpolator_type;
                typedef image_resample_affine_type<source_type> span_gen_type;

                interpolator_type interpolator(tr);
                span_gen_type sg(source, interpolator, filter);
                sg.blur(m_blur.value());
                agg::render_scanlines_aa(g_rasterizer, g_scanline, rb_pre, sa, sg);
                break;
            }

            case 2:
            {
                agg::trans_perspective tr(m_quad.polygon(), g_x1, g_y1, g_x2, g_y2);
                if(tr.is_valid())
                {
                    typedef agg::span_interpolator_linear_subdiv<agg::trans_perspective> interpolator_type;
                    interpolator_type interpolator(tr);

                    typedef image_filter_2x2_type<source_type,
                                                  interpolator_type> span_gen_type;
                    span_gen_type sg(source, interpolator, filter);
                    agg::render_scanlines_aa(g_rasterizer, g_scanline, rb_pre, sa, sg);
                }
                break;
            }

            case 3:
            {
                agg::trans_perspective tr(m_quad.polygon(), g_x1, g_y1, g_x2, g_y2);
                if(tr.is_valid())
                {
                    typedef agg::span_interpolator_trans<agg::trans_perspective> interpolator_type;
                    interpolator_type interpolator(tr);

                    typedef image_filter_2x2_type<source_type, 
                                                  interpolator_type> span_gen_type;
                    span_gen_type sg(source, interpolator, filter);
                    agg::render_scanlines_aa(g_rasterizer, g_scanline, rb_pre, sa, sg);
                }
                break;
            }

            case 4:
            {
                typedef agg::span_interpolator_persp_lerp<> interpolator_type;
                typedef agg::span_subdiv_adaptor<interpolator_type> subdiv_adaptor_type;

                interpolator_type interpolator(m_quad.polygon(), g_x1, g_y1, g_x2, g_y2);
                subdiv_adaptor_type subdiv_adaptor(interpolator);

                if(interpolator.is_valid())
                {
                    typedef image_resample_type<source_type, 
                                                subdiv_adaptor_type> span_gen_type;
                    span_gen_type sg(source, subdiv_adaptor, filter);
                    sg.blur(m_blur.value());
                    agg::render_scanlines_aa(g_rasterizer, g_scanline, rb_pre, sa, sg);
                }
                break;
            }

            case 5:
            {
                typedef agg::span_interpolator_persp_exact<> interpolator_type;
                typedef agg::span_subdiv_adaptor<interpolator_type> subdiv_adaptor_type;

                interpolator_type interpolator(m_quad.polygon(), g_x1, g_y1, g_x2, g_y2);
                subdiv_adaptor_type subdiv_adaptor(interpolator);

                if(interpolator.is_valid())
                {
                    typedef image_resample_type<source_type, 
                                                subdiv_adaptor_type> span_gen_type;
                    span_gen_type sg(source, subdiv_adaptor, filter);
                    sg.blur(m_blur.value());
                    agg::render_scanlines_aa(g_rasterizer, g_scanline, rb_pre, sa, sg);
                }
                break;
            }
        }
        double tm = elapsed_time();

        char buf[64]; 
        agg::gsv_text t;
        t.size(10.0);

        agg::conv_stroke<agg::gsv_text> pt(t);
        pt.width(1.5);

        sprintf(buf, "%3.2f ms", tm);
        t.start_point(10.0, 70.0);
        t.text(buf);

        g_rasterizer.add_path(pt);
        r.color(agg::rgba(0,0,0));
        agg::render_scanlines(g_rasterizer, g_scanline, r);

        //--------------------------
        agg::render_ctrl(g_rasterizer, g_scanline, rb, m_trans_type);
        agg::render_ctrl(g_rasterizer, g_scanline, rb, m_blur);
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

    virtual void on_key(int x, int y, unsigned key, unsigned flags) 
    {
        if(key == ' ')
        {
            double cx = (m_quad.xn(0) + m_quad.xn(1) + m_quad.xn(2) + m_quad.xn(3)) / 4;
            double cy = (m_quad.yn(0) + m_quad.yn(1) + m_quad.yn(2) + m_quad.yn(3)) / 4;
            agg::trans_affine tr = agg::trans_affine_translation(-cx, -cy);
            tr *= agg::trans_affine_rotation(agg::pi / 2.0);
            tr *= agg::trans_affine_translation(cx, cy);
            tr.transform(&m_quad.xn(0), &m_quad.yn(0));
            tr.transform(&m_quad.xn(1), &m_quad.yn(1));
            tr.transform(&m_quad.xn(2), &m_quad.yn(2));
            tr.transform(&m_quad.xn(3), &m_quad.yn(3));
            force_redraw();
        }
    }

};






int agg_main(int argc, char* argv[])
{
    the_application app(pix_format, flip_y);
    app.caption("AGG Example. Image Transformations with Resampling");

    const char* img_name = "spheres";
    if(argc >= 2) img_name = argv[1];
    if(!app.load_img(0, img_name)) 
    {
        char buf[256];
        if(strcmp(img_name, "spheres") == 0)
        {
            sprintf(buf, "File not found: %s%s. Download http://www.antigrain.com/%s%s\n"
                         "or copy it from another directory if available.",
                    img_name, app.img_ext(), img_name, app.img_ext());
        }
        else
        {
            sprintf(buf, "File not found: %s%s", img_name, app.img_ext());
        }
        app.message(buf);
        return 1;
    }
    

    if(app.init(600, 600, agg::window_resize))
    {
        return app.run();
    }
    return 1;
}






