#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include "agg_basics.h"
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_u.h"
#include "agg_renderer_scanline.h"
#include "agg_path_storage.h"
#include "agg_conv_transform.h"
#include "agg_trans_affine.h"
#include "agg_trans_bilinear.h"
#include "agg_trans_perspective.h"
#include "agg_span_allocator.h"
#include "agg_span_interpolator_linear.h"
#include "agg_span_interpolator_trans.h"
#include "agg_span_subdiv_adaptor.h"
#include "agg_pixfmt_rgba.h"
#include "agg_image_accessors.h"
#include "agg_span_image_filter_rgba.h"
#include "ctrl/agg_rbox_ctrl.h"
#include "platform/agg_platform_support.h"
#include "interactive_polygon.h"

#define AGG_BGRA32
//#define AGG_BGRA128
#include "pixel_formats.h"

enum flip_y_e { flip_y = true };

agg::rasterizer_scanline_aa<> g_rasterizer;
agg::scanline_u8  g_scanline;
double            g_x1 = 0;
double            g_y1 = 0;
double            g_x2 = 0;
double            g_y2 = 0;



class the_application : public agg::platform_support
{
public:
    typedef agg::renderer_base<pixfmt>                     renderer_base;
    typedef agg::renderer_scanline_aa_solid<renderer_base> renderer_solid;
    typedef agg::renderer_base<pixfmt_pre> renderer_base_pre;

    agg::interactive_polygon   m_quad;
    agg::rbox_ctrl<color_type> m_trans_type;

    the_application(agg::pix_format_e format, bool flip_y) :
        agg::platform_support(format, flip_y),
        m_quad(4, 5.0),
        m_trans_type(420, 5.0, 420+170.0, 70.0, !flip_y)
    {
        m_trans_type.add_item("Affine Parallelogram");
        m_trans_type.add_item("Bilinear");
        m_trans_type.add_item("Perspective");
        m_trans_type.cur_item(2);
        add_ctrl(m_trans_type);
    }


    virtual void on_init()
    {
        double d = 0.0;
        g_x1 = d;
        g_y1 = d;
        g_x2 = rbuf_img(0).width() - d;
        g_y2 = rbuf_img(0).height() - d;

        m_quad.xn(0) = 100;
        m_quad.yn(0) = 100;
        m_quad.xn(1) = width()  - 100;
        m_quad.yn(1) = 100;
        m_quad.xn(2) = width()  - 100;
        m_quad.yn(2) = height() - 100;
        m_quad.xn(3) = 100;
        m_quad.yn(3) = height() - 100;
    }

    virtual void on_draw()
    {
        pixfmt            pixf(rbuf_window());
        pixfmt_pre        pixf_pre(rbuf_window());
        renderer_base     rb(pixf);
        renderer_base_pre rb_pre(pixf_pre);

        renderer_solid r(rb);


        rb.clear(agg::rgba(1, 1, 1));

        if(m_trans_type.cur_item() == 0)
        {
            // For the affine parallelogram transformations we
            // calculate the 4-th (implicit) point of the parallelogram
            m_quad.xn(3) = m_quad.xn(0) + (m_quad.xn(2) - m_quad.xn(1));
            m_quad.yn(3) = m_quad.yn(0) + (m_quad.yn(2) - m_quad.yn(1));
        }

        //--------------------------
        // Render the "quad" tool and controls
        g_rasterizer.add_path(m_quad);
        agg::render_scanlines_aa_solid(g_rasterizer, g_scanline, rb, 
                                       agg::rgba(0, 0.3, 0.5, 0.6));

        // Prepare the polygon to rasterize. Here we need to fill
        // the destination (transformed) polygon.
        g_rasterizer.clip_box(0, 0, width(), height());
        g_rasterizer.reset();
        g_rasterizer.move_to_d(m_quad.xn(0), m_quad.yn(0));
        g_rasterizer.line_to_d(m_quad.xn(1), m_quad.yn(1));
        g_rasterizer.line_to_d(m_quad.xn(2), m_quad.yn(2));
        g_rasterizer.line_to_d(m_quad.xn(3), m_quad.yn(3));

        agg::span_allocator<color_type> sa;
        agg::image_filter_bilinear filter_kernel;
        agg::image_filter_lut filter(filter_kernel, false);

        pixfmt pixf_img(rbuf_img(0));

        //typedef agg::image_accessor_wrap<pixfmt, 
        //                                 agg::wrap_mode_reflect,
        //                                 agg::wrap_mode_reflect> img_accessor_type;
        //img_accessor_type ia(pixf_img);

        //typedef agg::image_accessor_clip<pixfmt> img_accessor_type;
        //img_accessor_type ia(pixf_img, agg::rgba(1,1,1));

        typedef agg::image_accessor_clone<pixfmt> img_accessor_type;
        img_accessor_type ia(pixf_img);

        start_timer();
        switch(m_trans_type.cur_item())
        {
            case 0:
            {
                // Note that we consruct an affine matrix that transforms
                // a parallelogram to a rectangle, i.e., it's inverted.
                // It's actually the same as:
                // tr(g_x1, g_y1, g_x2, g_y2, m_triangle.polygon());
                // tr.invert();
                agg::trans_affine tr(m_quad.polygon(), g_x1, g_y1, g_x2, g_y2);

                // Also note that we can use the linear interpolator instead of 
                // arbitrary span_interpolator_trans. It works much faster, 
                // but the transformations must be linear and parellel.
                typedef agg::span_interpolator_linear<agg::trans_affine> interpolator_type;
                interpolator_type interpolator(tr);

                typedef agg::span_image_filter_rgba_nn<img_accessor_type,
                                                       interpolator_type> span_gen_type;
                span_gen_type sg(ia, interpolator);
                agg::render_scanlines_aa(g_rasterizer, g_scanline, rb_pre, sa, sg);
                break;
            }

            case 1:
            {
                agg::trans_bilinear tr(m_quad.polygon(), g_x1, g_y1, g_x2, g_y2);
                if(tr.is_valid())
                {
                    typedef agg::span_interpolator_linear<agg::trans_bilinear> interpolator_type;
                    interpolator_type interpolator(tr);

                    typedef agg::span_image_filter_rgba_2x2<img_accessor_type,
                                                            interpolator_type> span_gen_type;
                    span_gen_type sg(ia, interpolator, filter);
                    agg::render_scanlines_aa(g_rasterizer, g_scanline, rb_pre, sa, sg);
                }
                break;
            }

            case 2:
            {
                agg::trans_perspective tr(m_quad.polygon(), g_x1, g_y1, g_x2, g_y2);
                if(tr.is_valid())
                {
                    // Subdivision and linear interpolation (faster, but less accurate)
                    //-----------------------
                    //typedef agg::span_interpolator_linear<agg::trans_perspective> interpolator_type;
                    //typedef agg::span_subdiv_adaptor<interpolator_type> subdiv_adaptor_type;
                    //interpolator_type interpolator(tr);
                    //subdiv_adaptor_type subdiv_adaptor(interpolator);
                    //
                    //typedef agg::span_image_filter_rgba_2x2<img_accessor_type,
                    //                                        subdiv_adaptor_type> span_gen_type;
                    //span_gen_type sg(ia, subdiv_adaptor, filter);
                    //-----------------------

                    // Direct calculations of the coordinates
                    //-----------------------
                    typedef agg::span_interpolator_trans<agg::trans_perspective> interpolator_type;
                    interpolator_type interpolator(tr);
                    typedef agg::span_image_filter_rgba_2x2<img_accessor_type,
                                                            interpolator_type> span_gen_type;
                    span_gen_type sg(ia, interpolator, filter);
                    //-----------------------

                    agg::render_scanlines_aa(g_rasterizer, g_scanline, rb_pre, sa, sg);
                }
                break;
            }
        }
        double tm = elapsed_time();

        char buf[128]; 
        agg::gsv_text t;
        t.size(10.0);

        agg::conv_stroke<agg::gsv_text> pt(t);
        pt.width(1.5);

        sprintf(buf, "%3.2f ms", tm);
        t.start_point(10.0, 10.0);
        t.text(buf);

        g_rasterizer.add_path(pt);
        agg::render_scanlines_aa_solid(g_rasterizer, g_scanline, rb, 
                                       agg::rgba(0,0,0));

        //--------------------------
        agg::render_ctrl(g_rasterizer, g_scanline, rb, m_trans_type);
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
    app.caption("AGG Example. Image Perspective Transformations");

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
    
/*
    // Testing the "black border" issue with alpha channel
    //----------------------------------------
    the_application::pixfmt pixf(app.rbuf_img(0));
    the_application::renderer_base rbase(pixf);
    rbase.clear(agg::srgba8(0,0,0,0));
    unsigned i;
    for(i = 0; i < 50; i++)
    {
        agg::ellipse ell(rand() % rbase.width(), 
                         rand() % rbase.height(),
                         rand() % 20 + 5,
                         rand() % 20 + 5,
                         100);
        g_rasterizer.add_path(ell);
        agg::render_scanlines_aa_solid(g_rasterizer, g_scanline, rbase, 
                                       agg::srgba8((rand() & 0x7F) + 127, 
                                                  (rand() & 0x7F) + 127, 
                                                  (rand() & 0x7F) + 127, 
                                                  255));
    }
*/

    if(app.init(600, 600, agg::window_resize))
    {
        return app.run();
    }
    return 1;
}






