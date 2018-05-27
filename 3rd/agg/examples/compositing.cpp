#include <stdio.h>
#include <cassert>
#include "agg_rendering_buffer.h"
#include "agg_renderer_base.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_u.h"
#include "agg_renderer_scanline.h"
#include "agg_rounded_rect.h"
#include "agg_pixfmt_rgba.h"
#include "agg_span_allocator.h"
#include "agg_span_gradient.h"
#include "agg_gsv_text.h"
#include "agg_span_interpolator_linear.h"
#include "platform/agg_platform_support.h"
#include "ctrl/agg_slider_ctrl.h"
#include "ctrl/agg_rbox_ctrl.h"

//#define AGG_BGRA32
#define AGG_BGRA128
#include "pixel_formats.h"

enum flip_y_e { flip_y = true };

typedef color_type color;
typedef component_order order;
typedef agg::rendering_buffer rbuf_type;

typedef agg::blender_rgba<color, order> prim_blender_type; 
typedef agg::pixfmt_alpha_blend_rgba<prim_blender_type, rbuf_type> prim_pixfmt_type;
typedef agg::renderer_base<prim_pixfmt_type> prim_ren_base_type;

void force_comp_op_link()
{
    // For unknown reason Digital Mars C++ doesn't want to link these 
    // functions if they are not specified explicitly. 
    color_type::value_type p[4] = {0};
    //agg::comp_op_rgba_invert_rgb <color, order>::blend_pix(p,0,0,0,0,0);
    //agg::comp_op_rgba_invert     <color, order>::blend_pix(p,0,0,0,0,0);
    //agg::comp_op_rgba_contrast   <color, order>::blend_pix(p,0,0,0,0,0);
    agg::comp_op_rgba_darken     <color, order>::blend_pix(p,0,0,0,0,0);
    agg::comp_op_rgba_lighten    <color, order>::blend_pix(p,0,0,0,0,0);
    agg::comp_op_rgba_color_dodge<color, order>::blend_pix(p,0,0,0,0,0);
    agg::comp_op_rgba_color_burn <color, order>::blend_pix(p,0,0,0,0,0);
    agg::comp_op_rgba_hard_light <color, order>::blend_pix(p,0,0,0,0,0);
    agg::comp_op_rgba_soft_light <color, order>::blend_pix(p,0,0,0,0,0);
    agg::comp_op_rgba_difference <color, order>::blend_pix(p,0,0,0,0,0);
    agg::comp_op_rgba_exclusion  <color, order>::blend_pix(p,0,0,0,0,0);
    agg::comp_op_rgba_src_atop   <color, order>::blend_pix(p,0,0,0,0,0);
    agg::comp_op_rgba_dst_atop   <color, order>::blend_pix(p,0,0,0,0,0);
    agg::comp_op_rgba_xor        <color, order>::blend_pix(p,0,0,0,0,0);
    agg::comp_op_rgba_plus       <color, order>::blend_pix(p,0,0,0,0,0);
    //agg::comp_op_rgba_minus      <color, order>::blend_pix(p,0,0,0,0,0);
    agg::comp_op_rgba_multiply   <color, order>::blend_pix(p,0,0,0,0,0);
    agg::comp_op_rgba_screen     <color, order>::blend_pix(p,0,0,0,0,0);
    agg::comp_op_rgba_overlay    <color, order>::blend_pix(p,0,0,0,0,0);
    agg::comp_op_rgba_src        <color, order>::blend_pix(p,0,0,0,0,0);
    agg::comp_op_rgba_dst        <color, order>::blend_pix(p,0,0,0,0,0);
    agg::comp_op_rgba_src_over   <color, order>::blend_pix(p,0,0,0,0,0);
    agg::comp_op_rgba_dst_over   <color, order>::blend_pix(p,0,0,0,0,0);
    agg::comp_op_rgba_src_in     <color, order>::blend_pix(p,0,0,0,0,0);
    agg::comp_op_rgba_dst_in     <color, order>::blend_pix(p,0,0,0,0,0);
    agg::comp_op_rgba_src_out    <color, order>::blend_pix(p,0,0,0,0,0);
    agg::comp_op_rgba_dst_out    <color, order>::blend_pix(p,0,0,0,0,0);
    agg::comp_op_rgba_clear      <color, order>::blend_pix(p,0,0,0,0,0);
}



agg::trans_affine gradient_affine(double x1, double y1, double x2, double y2, 
                                  double gradient_d2 = 100.0)
{
    agg::trans_affine mtx;
    double dx = x2 - x1;
    double dy = y2 - y1;
    mtx.reset();
    mtx *= agg::trans_affine_scaling(sqrt(dx * dx + dy * dy) / gradient_d2);
    mtx *= agg::trans_affine_rotation(atan2(dy, dx));
    mtx *= agg::trans_affine_translation(x1, y1);
    mtx.invert();
    return mtx;
}



template<class RenBase> 
void circle(RenBase& rbase, color c1, color c2, 
            double x1, double y1, double x2, double y2,
            double shadow_alpha)
{
    typedef RenBase renderer_base_type;
    typedef agg::gradient_x gradient_func_type;
    typedef agg::gradient_linear_color<color> color_func_type;
    typedef agg::span_interpolator_linear<> interpolator_type;
    typedef agg::span_allocator<color> span_allocator_type;
    typedef agg::span_gradient<color, 
                               interpolator_type, 
                               gradient_func_type, 
                               color_func_type> span_gradient_type;

    gradient_func_type  gradient_func;                   // The gradient function
    agg::trans_affine   gradient_mtx = gradient_affine(x1, y1, x2, y2, 100);
    interpolator_type   span_interpolator(gradient_mtx); // Span interpolator
    span_allocator_type span_allocator;                  // Span Allocator
    color_func_type     color_func(c1, c2);
    span_gradient_type  span_gradient(span_interpolator, 
                                      gradient_func, 
                                      color_func, 
                                      0, 100);
    agg::rasterizer_scanline_aa<> ras;
    agg::scanline_u8 sl;

    double r = agg::calc_distance(x1, y1, x2, y2) / 2;
    agg::ellipse ell((x1+x2)/2+5, (y1+y2)/2-3, r, r, 100);

    ras.add_path(ell);
    agg::render_scanlines_aa_solid(ras, sl, rbase, 
                                   agg::rgba(0.6, 0.6, 0.6, 0.7*shadow_alpha));

    ell.init((x1+x2)/2, (y1+y2)/2, r, r, 100);
    ras.add_path(ell);
    agg::render_scanlines_aa(ras, sl, rbase, span_allocator, span_gradient);
}



template<class RenBase> 
void src_shape(RenBase& rbase, color c1, color c2, 
               double x1, double y1, double x2, double y2)
{
    typedef RenBase renderer_base_type;
    typedef agg::gradient_x gradient_func_type;
    typedef agg::gradient_linear_color<color> color_func_type;
    typedef agg::span_interpolator_linear<> interpolator_type;
    typedef agg::span_allocator<color> span_allocator_type;
    typedef agg::span_gradient<color, 
                               interpolator_type, 
                               gradient_func_type, 
                               color_func_type> span_gradient_type;

    gradient_func_type  gradient_func;                   // The gradient function
    agg::trans_affine   gradient_mtx = gradient_affine(x1, y1, x2, y2, 100);
    interpolator_type   span_interpolator(gradient_mtx); // Span interpolator
    span_allocator_type span_allocator;                  // Span Allocator
    color_func_type     color_func(c1, c2);
    span_gradient_type  span_gradient(span_interpolator, 
                                      gradient_func, 
                                      color_func, 
                                      0, 100);
    agg::rasterizer_scanline_aa<> ras;
    agg::scanline_u8 sl;

    agg::rounded_rect shape(x1, y1, x2, y2, 40);
//    agg::ellipse shape((x1+x2)/2, (y1+y2)/2, fabs(x2-x1)/2, fabs(y2-y1)/2, 100);

    ras.add_path(shape);
    agg::render_scanlines_aa(ras, sl, rbase, span_allocator, span_gradient);
}







class the_application : public agg::platform_support
{
    agg::slider_ctrl<color>    m_alpha_src;
    agg::slider_ctrl<color>    m_alpha_dst;
    agg::rbox_ctrl<color_type> m_comp_op;

public:
    the_application(agg::pix_format_e format, bool flip_y) :
        agg::platform_support(format, flip_y),
        m_alpha_src(5, 5,    400, 11,    !flip_y),
        m_alpha_dst(5, 5+15, 400, 11+15, !flip_y),
        m_comp_op(420, 5.0, 420+170.0, 340.0, !flip_y)
    {
        m_alpha_src.label("Src Alpha=%.2f");
        m_alpha_src.value(0.75);
        add_ctrl(m_alpha_src);

        m_alpha_dst.label("Dst Alpha=%.2f");
        m_alpha_dst.value(1.0);
        add_ctrl(m_alpha_dst);

        m_comp_op.text_size(6.8);
        m_comp_op.add_item("clear");
        m_comp_op.add_item("src");
        m_comp_op.add_item("dst");
        m_comp_op.add_item("src-over");
        m_comp_op.add_item("dst-over");
        m_comp_op.add_item("src-in");
        m_comp_op.add_item("dst-in");
        m_comp_op.add_item("src-out");
        m_comp_op.add_item("dst-out");
        m_comp_op.add_item("src-atop");
        m_comp_op.add_item("dst-atop");
        m_comp_op.add_item("xor");
        m_comp_op.add_item("plus");
        //m_comp_op.add_item("minus");
        m_comp_op.add_item("multiply");
        m_comp_op.add_item("screen");
        m_comp_op.add_item("overlay");
        m_comp_op.add_item("darken");
        m_comp_op.add_item("lighten");
        m_comp_op.add_item("color-dodge");
        m_comp_op.add_item("color-burn");
        m_comp_op.add_item("hard-light");
        m_comp_op.add_item("soft-light");
        m_comp_op.add_item("difference");
        m_comp_op.add_item("exclusion");
        //m_comp_op.add_item("contrast");
        //m_comp_op.add_item("invert");
        //m_comp_op.add_item("invert-rgb");
        m_comp_op.cur_item(3);
        add_ctrl(m_comp_op);
    }

    virtual ~the_application()
    {
    }

    virtual void on_init()
    {
    }


    void render_scene(rbuf_type& rbuf, prim_pixfmt_type& pixf)
    {
        typedef agg::comp_op_adaptor_rgba<color, order> blender_type;
        typedef agg::pixfmt_custom_blend_rgba<blender_type, rbuf_type> pixfmt_type;
        typedef agg::renderer_base<pixfmt_type> renderer_type;

        pixfmt_type ren_pixf(rbuf);
        renderer_type renderer(ren_pixf);

        agg::renderer_base<prim_pixfmt_type> rb(pixf);

        rb.blend_from(prim_pixfmt_type(rbuf_img(1)), 
                      0, 250, 180, 
                      agg::cover_type(m_alpha_dst.value() * agg::cover_full));

        circle(rb, 
               agg::srgba8(0xFD, 0xF0, 0x6F, unsigned(m_alpha_dst.value() * 255)), 
               agg::srgba8(0xFE, 0x9F, 0x34, unsigned(m_alpha_dst.value() * 255)),
               70*3, 100+24*3, 37*3, 100+79*3,
               m_alpha_dst.value());

        ren_pixf.comp_op(m_comp_op.cur_item());

        src_shape(renderer, 
                    agg::srgba8(0x7F, 0xC1, 0xFF, unsigned(m_alpha_src.value() * 255)), 
                    agg::srgba8(0x05, 0x00, 0x5F, unsigned(m_alpha_src.value() * 255)),
                    300+50, 100+24*3, 107+50, 100+79*3);
/*
        src_shape(renderer, 
                    agg::srgba8(0xFF, 0xFF, 0xFF, unsigned(m_alpha_src.value() * 255)), 
                    agg::srgba8(0xFF, 0xFF, 0xFF, unsigned(m_alpha_src.value() * 255)),
                    300+50, 100+24*3, 107+50, 100+79*3);
*/
    }


    virtual void on_draw()
    {
        prim_pixfmt_type pixf(rbuf_window());
        prim_ren_base_type rb(pixf);
        rb.clear(agg::srgba8(255, 255, 255));

        unsigned y;
        for(y = 0; y < rb.height(); y += 8)
        {
            unsigned x;
            for(x = ((y >> 3) & 1) << 3; x < rb.width(); x += 16)
            {
                rb.copy_bar(x, y, x+7, y+7, agg::srgba8(0xdf, 0xdf, 0xdf));
            }
        }

        create_img(0, rbuf_window().width(), rbuf_window().height()); // agg_platform_support functionality

        prim_pixfmt_type pixf2(rbuf_img(0));
        prim_ren_base_type rb2(pixf2);
        rb2.clear(agg::srgba8(0,0,0,0));
        //rb2.clear(agg::srgba8(255,255,255,255));

        typedef agg::blender_rgba_pre<color, order> blender_type_pre; 
        typedef agg::pixfmt_alpha_blend_rgba<blender_type_pre, rbuf_type> pixfmt_pre;
        typedef agg::renderer_base<pixfmt_pre> ren_base_pre;

        pixfmt_pre pixf_pre(rbuf_window());
        ren_base_pre rb_pre(pixf_pre);


        start_timer();
        render_scene(rbuf_img(0), pixf2);
        double tm = elapsed_time();

        rb_pre.blend_from(pixf2);

        agg::rasterizer_scanline_aa<> ras;
        agg::scanline_u8 sl;
        agg::renderer_scanline_aa_solid<prim_ren_base_type> ren(rb);

        char buf[64]; 
        agg::gsv_text t;
        t.size(10.0);

        agg::conv_stroke<agg::gsv_text> pt(t);
        pt.width(1.5);

        sprintf(buf, "%3.2f ms", tm);
        t.start_point(10.0, 35.0);
        t.text(buf);

        ras.add_path(pt);
        ren.color(agg::rgba(0,0,0));
        agg::render_scanlines(ras, sl, ren);


        agg::render_ctrl_rs(ras, sl, ren, m_alpha_src);
        agg::render_ctrl_rs(ras, sl, ren, m_alpha_dst);
        agg::render_ctrl_rs(ras, sl, ren, m_comp_op);
    }


    virtual void on_mouse_button_down(int x, int y, unsigned flags)
    {
    }

    virtual void on_mouse_move(int x, int y, unsigned flags)
    {
    }

    virtual void on_mouse_button_up(int x, int y, unsigned flags)
    {
    }
};


int agg_main(int argc, char* argv[])
{
    force_comp_op_link();
    the_application app(pix_format, flip_y);
    app.caption("AGG Example. Compositing Modes");

    const char* img_name = "compositing";
    if(argc >= 2) img_name = argv[1];
    if(!app.load_img(1, img_name)) 
    {
        char buf[256];
        if(strcmp(img_name, "compositing") == 0)
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


    if(app.init(600, 400, agg::window_resize))
    {
        return app.run();
    }
    return 1;
}


