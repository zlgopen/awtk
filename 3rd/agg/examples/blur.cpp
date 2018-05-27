#include <math.h>
#include <stdio.h>
#include "agg_basics.h"
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_conv_curve.h"
#include "agg_conv_contour.h"
#include "agg_conv_stroke.h"
#include "agg_scanline_p.h"
#include "agg_renderer_scanline.h"
#include "agg_pixfmt_rgb.h"
#include "agg_pixfmt_rgba.h"
#include "agg_pixfmt_gray.h"
#include "agg_bounding_rect.h"
#include "agg_trans_perspective.h"
#include "agg_blur.h"
#include "ctrl/agg_slider_ctrl.h"
#include "ctrl/agg_rbox_ctrl.h"
#include "ctrl/agg_cbox_ctrl.h"
#include "ctrl/agg_polygon_ctrl.h"
#include "platform/agg_platform_support.h"

// Stack blur is limited to integer pixel formats, so we 
// can't use it for floating-point formats.
#define USE_STACK_BLUR 1

#define AGG_BGR24
#include "pixel_formats.h"

enum flip_y_e { flip_y = true };


class the_application : public agg::platform_support
{
    agg::rbox_ctrl<color_type>    m_method;
    agg::slider_ctrl<color_type>  m_radius;
    agg::polygon_ctrl<color_type> m_shadow_ctrl;
    agg::cbox_ctrl<color_type>    m_channel_r;
    agg::cbox_ctrl<color_type>    m_channel_g;
    agg::cbox_ctrl<color_type>    m_channel_b;

    agg::path_storage             m_path;
    typedef agg::conv_curve<agg::path_storage> shape_type;
    shape_type                    m_shape;

    agg::rasterizer_scanline_aa<> m_ras;
    agg::scanline_p8              m_sl;
    agg::rendering_buffer         m_rbuf2;

    agg::stack_blur    <color_type, agg::stack_blur_calc_rgb<> >     m_stack_blur;
    agg::recursive_blur<color_type, agg::recursive_blur_calc_rgb<> > m_recursive_blur;

    agg::rect_d m_shape_bounds;


public:
    the_application(agg::pix_format_e format, bool flip_y) :
        agg::platform_support(format, flip_y),
        m_method     (10.0, 10.0, 130.0, 70.0, !flip_y),
        m_radius     (130 + 10.0, 10.0 + 4.0, 130 + 300.0, 10.0 + 8.0 + 4.0, !flip_y),
        m_shadow_ctrl(4),
        m_channel_r  (10.0, 80.0,  "Red", !flip_y),
        m_channel_g  (10.0, 95.0,  "Green", !flip_y),
        m_channel_b  (10.0, 110.0, "Blue", !flip_y),
        m_shape(m_path)
    {
        add_ctrl(m_method);
        m_method.text_size(8);
#if USE_STACK_BLUR
        m_method.add_item("Stack blur");
#else
        m_method.add_item("No blur");
#endif
        m_method.add_item("Recursive blur");
        m_method.add_item("Channels");
        m_method.cur_item(0);

        add_ctrl(m_radius);
        m_radius.range(0.0, 40.0);
        m_radius.value(15.0);
        m_radius.label("Blur Radius=%1.2f");

        add_ctrl(m_shadow_ctrl);

        add_ctrl(m_channel_r);
        add_ctrl(m_channel_g);
        add_ctrl(m_channel_b);
        m_channel_g.status(true);

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

        agg::trans_affine shape_mtx;
        shape_mtx *= agg::trans_affine_scaling(4.0);
        shape_mtx *= agg::trans_affine_translation(150, 100);
        m_path.transform(shape_mtx);

        agg::bounding_rect_single(m_shape, 0, 
                                  &m_shape_bounds.x1, &m_shape_bounds.y1, 
                                  &m_shape_bounds.x2, &m_shape_bounds.y2);

        m_shadow_ctrl.xn(0) = m_shape_bounds.x1 + 10;
        m_shadow_ctrl.yn(0) = m_shape_bounds.y1 - 10;
        m_shadow_ctrl.xn(1) = m_shape_bounds.x2 + 10;
        m_shadow_ctrl.yn(1) = m_shape_bounds.y1 - 10;
        m_shadow_ctrl.xn(2) = m_shape_bounds.x2 + 10;
        m_shadow_ctrl.yn(2) = m_shape_bounds.y2 - 10;
        m_shadow_ctrl.xn(3) = m_shape_bounds.x1 + 10;
        m_shadow_ctrl.yn(3) = m_shape_bounds.y2 - 10;
        m_shadow_ctrl.line_color(agg::rgba(0, 0.3, 0.5, 0.3));
    }



    virtual void on_draw()
    {
        typedef agg::renderer_base<pixfmt> ren_base;

        pixfmt pixf(rbuf_window());
        ren_base renb(pixf);
        renb.clear(agg::rgba(1, 1, 1));
        m_ras.clip_box(0,0, width(), height());

        agg::trans_perspective shadow_persp(m_shape_bounds.x1, m_shape_bounds.y1, 
                                            m_shape_bounds.x2, m_shape_bounds.y2,
                                            m_shadow_ctrl.polygon());

        agg::conv_transform<shape_type, 
                            agg::trans_perspective> shadow_trans(m_shape, 
                                                                 shadow_persp);

        // Render shadow
        m_ras.add_path(shadow_trans);
        agg::render_scanlines_aa_solid(m_ras, m_sl, renb, agg::rgba(0.1, 0.1, 0.1));

        // Calculate the bounding box and extend it by the blur radius
        agg::rect_d bbox;
        agg::bounding_rect_single(shadow_trans, 0, &bbox.x1, &bbox.y1, &bbox.x2, &bbox.y2);

        bbox.x1 -= m_radius.value();
        bbox.y1 -= m_radius.value();
        bbox.x2 += m_radius.value();
        bbox.y2 += m_radius.value();

        // The recursive blur method represents the true Gussian Blur,
        // with theoretically infinite kernel. The restricted window size
        // results in extra influence of edge pixels. It's impossible to
        // solve correctly, but extending the right and top areas to another
        // radius value produces fair result.
        //------------------
        bbox.x2 += m_radius.value();
        bbox.y2 += m_radius.value();

        start_timer();
        if(m_method.cur_item() != 2)
        {
            // Create a new pixel renderer and attach it to the main one as a child image. 
            // It returns true if the attachment suceeded. It fails if the rectangle 
            // (bbox) is fully clipped.
            //------------------
            pixfmt pixf2(m_rbuf2);
            if(pixf2.attach(pixf, int(bbox.x1), int(bbox.y1), int(bbox.x2), int(bbox.y2)))
            {
                // Blur it
                if(m_method.cur_item() == 0)
                {
#if USE_STACK_BLUR
                    // More general method, but 30-40% slower.
                    //------------------
                    m_stack_blur.blur(pixf2, agg::uround(m_radius.value()));

                    // Faster, but bore specific. 
                    // Works only for 8 bits per channel and only with radii <= 254.
                    //------------------
                    //agg::stack_blur_rgb24(pixf2, agg::uround(m_radius.value()), 
                    //                             agg::uround(m_radius.value()));
#endif
                }
                else
                {
                    // True Gaussian Blur, 3-5 times slower than Stack Blur,
                    // but still constant time of radius. Very sensitive
                    // to precision, doubles are must here.
                    //------------------
                    m_recursive_blur.blur(pixf2, m_radius.value());
                }
            }
        }
        else
        {
            typedef agg::pixfmt_alpha_blend_gray<
                agg::blender_gray<gray_type>, 
                agg::rendering_buffer, 
                3, component_order::R> pixfmt_r;
            typedef agg::pixfmt_alpha_blend_gray<
                agg::blender_gray<gray_type>, 
                agg::rendering_buffer, 
                3, component_order::G> pixfmt_g;
            typedef agg::pixfmt_alpha_blend_gray<
                agg::blender_gray<gray_type>, 
                agg::rendering_buffer, 
                3, component_order::B> pixfmt_b;

            pixfmt_r pixf2r(m_rbuf2);
            pixfmt_g pixf2g(m_rbuf2);
            pixfmt_b pixf2b(m_rbuf2);

            agg::recursive_blur<gray_type, agg::recursive_blur_calc_gray<> > blurrer;

            // Blur separate channels
            //------------------

            if(m_channel_r.status())
            {
                if(pixf2r.attach(pixf, int(bbox.x1), int(bbox.y1), int(bbox.x2), int(bbox.y2)))
                {
                    blurrer.blur(pixf2r, agg::uround(m_radius.value()));
                }
            }

            if(m_channel_g.status())
            {
                if(pixf2g.attach(pixf, int(bbox.x1), int(bbox.y1), int(bbox.x2), int(bbox.y2)))
                {
                    blurrer.blur(pixf2g, agg::uround(m_radius.value()));
                }
            }

            if(m_channel_b.status())
            {
                if(pixf2b.attach(pixf, int(bbox.x1), int(bbox.y1), int(bbox.x2), int(bbox.y2)))
                {
                    blurrer.blur(pixf2b, agg::uround(m_radius.value()));
                }
            }
        }
        double tm = elapsed_time();

        agg::render_ctrl(m_ras, m_sl, renb, m_shadow_ctrl);

        // Render the shape itself
        //------------------
        m_ras.add_path(m_shape);
        agg::render_scanlines_aa_solid(m_ras, m_sl, renb, agg::rgba(0.6, 0.9, 0.7, 0.8));

        char buf[64]; 
        agg::gsv_text t;
        t.size(10.0);

        agg::conv_stroke<agg::gsv_text> st(t);
        st.width(1.5);

        sprintf(buf, "%3.2f ms", tm);
        t.start_point(140.0, 30.0);
        t.text(buf);

        m_ras.add_path(st);
        agg::render_scanlines_aa_solid(m_ras, m_sl, renb, agg::rgba(0,0,0));


        agg::render_ctrl(m_ras, m_sl, renb, m_method);
        agg::render_ctrl(m_ras, m_sl, renb, m_radius);
        agg::render_ctrl(m_ras, m_sl, renb, m_channel_r);
        agg::render_ctrl(m_ras, m_sl, renb, m_channel_g);
        agg::render_ctrl(m_ras, m_sl, renb, m_channel_b);
    }

};



int agg_main(int argc, char* argv[])
{
    the_application app(pix_format, flip_y);
    app.caption("AGG Example. Gaussian and Stack Blur");

    if(app.init(440, 330, 0))
    {
        return app.run();
    }
    return 1;
}


