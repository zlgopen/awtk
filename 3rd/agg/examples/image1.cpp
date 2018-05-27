#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_ellipse.h"
#include "agg_trans_affine.h"
#include "agg_conv_transform.h"
#include "agg_span_image_filter_rgb.h"
#include "agg_span_image_filter_rgba.h"
#include "agg_span_image_filter_gray.h"
#include "agg_pixfmt_rgba.h"
#include "agg_scanline_u.h"
#include "agg_renderer_scanline.h"
#include "agg_span_allocator.h"
#include "agg_span_interpolator_linear.h"
#include "agg_image_accessors.h"
#include "ctrl/agg_slider_ctrl.h"
#include "platform/agg_platform_support.h"

enum flip_y_e { flip_y = true };

#define AGG_BGR24
//#define AGG_RGB565
#include "pixel_formats.h"


class the_application : public agg::platform_support
{
    agg::slider_ctrl<color_type> m_angle;
    agg::slider_ctrl<color_type> m_scale;

public:
    the_application(agg::pix_format_e format, bool flip_y) :
        agg::platform_support(format, flip_y),
        m_angle(5,  5,    300, 12,    !flip_y),
        m_scale(5,  5+15, 300, 12+15, !flip_y)
    {
        add_ctrl(m_angle);
        add_ctrl(m_scale);
        m_angle.label("Angle=%3.2f");
        m_scale.label("Scale=%3.2f");
        m_angle.range(-180.0, 180.0);
        m_angle.value(0.0);
        m_scale.range(0.1, 5.0);
        m_scale.value(1.0);
    }

    virtual ~the_application()
    {
    }

    virtual void on_draw()
    {
        typedef agg::renderer_base<pixfmt>     renderer_base;
        typedef agg::renderer_base<pixfmt_pre> renderer_base_pre;
       
        pixfmt            pixf(rbuf_window());
        pixfmt_pre        pixf_pre(rbuf_window());
        renderer_base     rb(pixf);
        renderer_base_pre rb_pre(pixf_pre);

        rb.clear(agg::rgba(1.0, 1.0, 1.0));

        agg::trans_affine src_mtx;
        src_mtx *= agg::trans_affine_translation(-initial_width()/2 - 10, -initial_height()/2 - 20 - 10);
        src_mtx *= agg::trans_affine_rotation(m_angle.value() * agg::pi / 180.0);
        src_mtx *= agg::trans_affine_scaling(m_scale.value());
        src_mtx *= agg::trans_affine_translation(initial_width()/2, initial_height()/2 + 20);
        src_mtx *= trans_affine_resizing();

        agg::trans_affine img_mtx;
        img_mtx *= agg::trans_affine_translation(-initial_width()/2 + 10, -initial_height()/2 + 20 + 10);
        img_mtx *= agg::trans_affine_rotation(m_angle.value() * agg::pi / 180.0);
        img_mtx *= agg::trans_affine_scaling(m_scale.value());
        img_mtx *= agg::trans_affine_translation(initial_width()/2, initial_height()/2 + 20);
        img_mtx *= trans_affine_resizing();
        img_mtx.invert();

        agg::span_allocator<color_type> sa;

        typedef agg::span_interpolator_linear<> interpolator_type;
        interpolator_type interpolator(img_mtx);

        typedef agg::image_accessor_clip<pixfmt> img_source_type;

        pixfmt img_pixf(rbuf_img(0));
        img_source_type img_src(img_pixf, agg::rgba_pre(0, 0.4, 0, 0.5));

/*
        // Version without filtering (nearest neighbor)
        //------------------------------------------
        typedef agg::span_image_filter_rgb_nn<img_source_type,
                                              interpolator_type> span_gen_type;
        span_gen_type sg(img_src, interpolator);
        //------------------------------------------
*/

        // Version with "hardcoded" bilinear filter and without 
        // image_accessor (direct filter, the old variant)
        //------------------------------------------
        typedef agg::span_image_filter_rgb_bilinear_clip<pixfmt,
                                                         interpolator_type> span_gen_type;
        span_gen_type sg(img_pixf, agg::rgba_pre(0, 0.4, 0, 0.5), interpolator);
        //------------------------------------------

/*
        // Version with arbitrary 2x2 filter
        //------------------------------------------
        typedef agg::span_image_filter_rgb_2x2<img_source_type,
                                               interpolator_type> span_gen_type;
        agg::image_filter<agg::image_filter_kaiser> filter;
        span_gen_type sg(img_src, interpolator, filter);
        //------------------------------------------
*/
/*
        // Version with arbitrary filter
        //------------------------------------------
        typedef agg::span_image_filter_rgb<img_source_type,
                                           interpolator_type> span_gen_type;
        agg::image_filter<agg::image_filter_spline36> filter;
        span_gen_type sg(img_src, interpolator, filter);
        //------------------------------------------
*/

        agg::rasterizer_scanline_aa<> ras;
        ras.clip_box(0, 0, width(), height());
        agg::scanline_u8 sl;
        double r = initial_width();
        if(initial_height() - 60 < r) r = initial_height() - 60;
        agg::ellipse ell(initial_width()  / 2.0 + 10, 
                         initial_height() / 2.0 + 20 + 10, 
                         r / 2.0 + 16.0, 
                         r / 2.0 + 16.0, 200);


        agg::conv_transform<agg::ellipse> tr(ell, src_mtx);

        ras.add_path(tr);
        agg::render_scanlines_aa(ras, sl, rb_pre, sa, sg);

        agg::render_ctrl(ras, sl, rb, m_angle);
        agg::render_ctrl(ras, sl, rb, m_scale);
    }

};





int agg_main(int argc, char* argv[])
{
    the_application app(pix_format, flip_y);
    app.caption("Image Affine Transformations with filtering");

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

    if(app.init(app.rbuf_img(0).width() + 20, app.rbuf_img(0).height() + 40 + 20, agg::window_resize))
    {

// Test the plain/premultiplied issue
//-------------------
//typedef agg::pixfmt_bgra32         pixfmt; 
//typedef agg::renderer_base<pixfmt> renderer_base;
//pixfmt        pixf(app.rbuf_img(0));
//renderer_base rb(pixf);
//for(unsigned i = 0; i < app.rbuf_img(0).height(); i += 2)
//{
//    // Fully transparent
//    rb.copy_hline(0, i, app.rbuf_img(0).width(), agg::rgba(0, 0, 0, 0));  
//    if(i + 1 < app.rbuf_img(0).height())
//    {
//        // Fully opaque white
//        rb.copy_hline(0, i + 1, app.rbuf_img(0).width(), agg::rgba(1, 1, 1, 1));  
//    }
//}

        return app.run();
    }
    return 0;
}



/*
E:\agg23\examples\image1.cpp(111) : error C2664: 

  '__thiscall agg::span_image_filter_gray_bilinear<struct agg::sgray8,
                                                   struct agg::order_bgra,
                                                   class agg::span_interpolator_linear<class agg::trans_affine,8> >::agg::span_image_filter_gray_bilinear<struct agg::sgray8,struct agg::order_bgra,class agg::span_interpolator_linear<class agg::trans_affine,8> >(class agg::span_interpolator_linear<class agg::trans_affine,8> &,const class agg::row_ptr_cache<unsigned char> &,const struct agg::sgray8 &,struct agg::order_bgra &)' : 

cannot convert parameter 1 from 

'class agg::span_allocator<struct agg::sgray8>' to 
'class agg::span_interpolator_linear<class agg::trans_affine,8> &'
*/
