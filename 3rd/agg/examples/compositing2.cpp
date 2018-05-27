#include <stdio.h>
#include "agg_rendering_buffer.h"
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

#define AGG_BGRA32
//#define AGG_BGRA128
#include "pixel_formats.h"

enum flip_y_e { flip_y = true };

typedef color_type color;
typedef component_order order;
typedef agg::blender_rgba<color, order> prim_blender_type; 
typedef agg::pixfmt_alpha_blend_rgba<prim_blender_type, agg::rendering_buffer> prim_pixfmt_type;
typedef agg::renderer_base<prim_pixfmt_type> prim_ren_base_type;


void force_comp_op_link()
{
    // For unknown reason Digital Mars C++ doesn't want to link these 
    // functions if they are not specified explicitly. 
    color::value_type p[4] = {0};
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


template<class Container, class ColorT> 
void generate_color_ramp(Container& c, 
                         ColorT c1, ColorT c2, ColorT c3, ColorT c4)
{
    unsigned i;
    for(i = 0; i < 85; i++)
    {
        c[i] = c1.gradient(c2, i/85.0);
    }
    for(; i < 170; i++)
    {
        c[i] = c2.gradient(c3, (i - 85)/85.0);
    }
    for(; i < 256; i++)
    {
        c[i] = c3.gradient(c4, (i - 170)/85.0);
    }
}


class the_application : public agg::platform_support
{
    agg::slider_ctrl<color>    m_alpha_dst;
    agg::slider_ctrl<color>    m_alpha_src;
    agg::rbox_ctrl<color_type> m_comp_op;

    agg::pod_auto_array<color, 256> m_ramp1;
    agg::pod_auto_array<color, 256> m_ramp2;

    agg::rasterizer_scanline_aa<> m_ras;
    agg::scanline_u8 m_sl;

public:
    the_application(agg::pix_format_e format, bool flip_y) :
        agg::platform_support(format, flip_y),
        m_alpha_dst(5, 5,    400, 11,    !flip_y),
        m_alpha_src(5, 5+15, 400, 11+15, !flip_y),
        m_comp_op(420, 5.0, 420+170.0, 340.0, !flip_y)
    {
        m_alpha_dst.label("Dst Alpha=%.2f");
        m_alpha_dst.value(1.0);
        add_ctrl(m_alpha_dst);

        m_alpha_src.label("Src Alpha=%.2f");
        m_alpha_src.value(1.0);
        add_ctrl(m_alpha_src);

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


    template<class RenBase, class ColorRamp> 
    void radial_shape(RenBase& rbase, ColorRamp& colors,
                      double x1, double y1, double x2, double y2)
    {
        typedef RenBase renderer_base_type;
        typedef agg::gradient_radial gradient_func_type;
        typedef ColorRamp color_func_type;
        typedef agg::span_interpolator_linear<> interpolator_type;
        typedef agg::span_allocator<color> span_allocator_type;
        typedef agg::span_gradient<color, 
                                   interpolator_type, 
                                   gradient_func_type, 
                                   color_func_type> span_gradient_type;

        gradient_func_type  gradient_func;                   // The gradient function
        agg::trans_affine   gradient_mtx;
        interpolator_type   span_interpolator(gradient_mtx); // Span interpolator
        span_allocator_type span_allocator;                  // Span Allocator
        span_gradient_type  span_gradient(span_interpolator, 
                                          gradient_func, 
                                          colors, 
                                          0, 100);

        double cx = (x1 + x2) / 2.0;
        double cy = (y1 + y2) / 2.0;
        double r  = 0.5 * (((x2 - x1) < (y2 - y1)) ? (x2 - x1) : (y2 - y1));

        gradient_mtx *= agg::trans_affine_scaling(r / 100.0);
        gradient_mtx *= agg::trans_affine_translation(cx, cy);
        gradient_mtx *= trans_affine_resizing();
        gradient_mtx.invert();

        agg::ellipse ell(cx, cy, r, r, 100);
        agg::conv_transform<agg::ellipse> trans(ell, trans_affine_resizing());
        m_ras.add_path(trans);

        agg::render_scanlines_aa(m_ras, m_sl, rbase, span_allocator, span_gradient);
    }


    template<class RenBase> void render_scene(RenBase& rb)
    {
        typedef agg::comp_op_adaptor_rgba<color, order> blender_type;
        typedef agg::pixfmt_custom_blend_rgba<blender_type, agg::rendering_buffer> pixfmt_type;
        typedef agg::renderer_base<pixfmt_type> renderer_type;

        pixfmt_type pixf(rbuf_window());
        renderer_type ren(pixf);


        pixf.comp_op(agg::comp_op_difference);
        radial_shape(ren, m_ramp1, 50, 50, 50+320, 50+320);

        pixf.comp_op(m_comp_op.cur_item());
        double cx = 50;
        double cy = 50;
        radial_shape(ren, m_ramp2, cx+120-70, cy+120-70, cx+120+70, cy+120+70);
        radial_shape(ren, m_ramp2, cx+200-70, cy+120-70, cx+200+70, cy+120+70);
        radial_shape(ren, m_ramp2, cx+120-70, cy+200-70, cx+120+70, cy+200+70); 
        radial_shape(ren, m_ramp2, cx+200-70, cy+200-70, cx+200+70, cy+200+70);
    }


    virtual void on_draw()
    {
        prim_pixfmt_type pixf(rbuf_window());
        prim_ren_base_type rb(pixf);
        rb.clear(agg::srgba8(255, 255, 255));

        generate_color_ramp(m_ramp1, 
                            agg::rgba(0, 0, 0, m_alpha_dst.value()),
                            agg::rgba(0, 0, 1, m_alpha_dst.value()),
                            agg::rgba(0, 1, 0, m_alpha_dst.value()),
                            agg::rgba(1, 0, 0, 0));

        generate_color_ramp(m_ramp2, 
                            agg::rgba(0, 0, 0, m_alpha_src.value()),
                            agg::rgba(0, 0, 1, m_alpha_src.value()),
                            agg::rgba(0, 1, 0, m_alpha_src.value()),
                            agg::rgba(1, 0, 0, 0));

        render_scene(rb);
        agg::renderer_scanline_aa_solid<prim_ren_base_type> ren(rb);

        agg::render_ctrl_rs(m_ras, m_sl, ren, m_alpha_dst);
        agg::render_ctrl_rs(m_ras, m_sl, ren, m_alpha_src);
        agg::render_ctrl_rs(m_ras, m_sl, ren, m_comp_op);
    }

};


int agg_main(int argc, char* argv[])
{
    force_comp_op_link();
    the_application app(pix_format, flip_y);
    app.caption("AGG Example. Compositing Modes");

    if(app.init(600, 400, agg::window_resize|agg::window_keep_aspect_ratio))
    {
        return app.run();
    }
    return 1;
}


