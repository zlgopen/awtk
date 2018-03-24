#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_u.h"
#include "agg_renderer_scanline.h"
#include "agg_span_allocator.h"
#include "agg_span_gradient.h"
#include "agg_gradient_lut.h"
#include "agg_gamma_lut.h"
#include "agg_span_interpolator_linear.h"
#include "ctrl/agg_slider_ctrl.h"
#include "platform/agg_platform_support.h"


#include "agg_pixfmt_rgb.h"
#define pix_format agg::pix_format_bgr24
typedef agg::pixfmt_bgr24 pixfmt;
typedef agg::rgba8 color_type;
typedef agg::order_bgr component_order;


enum { flip_y = true };


class the_application : public agg::platform_support
{
    typedef agg::renderer_base<pixfmt> renderer_base;
    typedef agg::renderer_scanline_aa_solid<renderer_base> renderer_solid;
    typedef agg::gamma_lut<agg::int8u, agg::int8u> gamma_lut_type;
    typedef agg::gradient_radial_focus gradient_func_type;
    typedef agg::gradient_reflect_adaptor<gradient_func_type> gradient_adaptor_type;
    typedef agg::gradient_lut<agg::color_interpolator<agg::rgba8>, 1024> color_func_type;
    typedef agg::span_interpolator_linear<> interpolator_type;
    typedef agg::span_allocator<color_type> span_allocator_type;
    typedef agg::span_gradient<color_type, 
                               interpolator_type, 
                               gradient_adaptor_type, 
                               color_func_type> span_gradient_type;
    
    agg::slider_ctrl<agg::rgba8>    m_gamma;

    agg::scanline_u8                m_scanline;
    agg::rasterizer_scanline_aa<>   m_rasterizer;
    span_allocator_type             m_alloc;
    color_func_type                 m_gradient_lut;
    gamma_lut_type                  m_gamma_lut;

    double m_mouse_x, m_mouse_y;
    double m_old_gamma;


public:
    the_application(agg::pix_format_e format, bool flip_y) :
        agg::platform_support(format, flip_y),
        m_gamma(5.0, 5.0, 340.0, 12.0, !flip_y),
        m_mouse_x(200), m_mouse_y(200)
    {
        m_gamma.range(0.5, 2.5);
        m_gamma.value(1.8);
        m_gamma.label("Gamma = %.3f");
        add_ctrl(m_gamma);
        m_gamma.no_transform();

        m_gamma_lut.gamma(m_gamma.value());
        m_old_gamma = m_gamma.value();

        build_gradient_lut();
    }

    virtual void on_init()
    {
        m_mouse_y = initial_height() / 2;
        m_mouse_x = initial_width() / 2;
    }

    void build_gradient_lut()
    {
        m_gradient_lut.remove_all();

        m_gradient_lut.add_color(0.0, agg::rgba8_gamma_dir(agg::rgba8(0, 255, 0),   m_gamma_lut));
        m_gradient_lut.add_color(0.2, agg::rgba8_gamma_dir(agg::rgba8(120, 0, 0),   m_gamma_lut));
        m_gradient_lut.add_color(0.7, agg::rgba8_gamma_dir(agg::rgba8(120, 120, 0), m_gamma_lut));
        m_gradient_lut.add_color(1.0, agg::rgba8_gamma_dir(agg::rgba8(0, 0, 255),   m_gamma_lut));

        //m_gradient_lut.add_color(0.0, agg::rgba8::from_wavelength(380, m_gamma.value()));
        //m_gradient_lut.add_color(0.1, agg::rgba8::from_wavelength(420, m_gamma.value()));
        //m_gradient_lut.add_color(0.2, agg::rgba8::from_wavelength(460, m_gamma.value()));
        //m_gradient_lut.add_color(0.3, agg::rgba8::from_wavelength(500, m_gamma.value()));
        //m_gradient_lut.add_color(0.4, agg::rgba8::from_wavelength(540, m_gamma.value()));
        //m_gradient_lut.add_color(0.5, agg::rgba8::from_wavelength(580, m_gamma.value()));
        //m_gradient_lut.add_color(0.6, agg::rgba8::from_wavelength(620, m_gamma.value()));
        //m_gradient_lut.add_color(0.7, agg::rgba8::from_wavelength(660, m_gamma.value()));
        //m_gradient_lut.add_color(0.8, agg::rgba8::from_wavelength(700, m_gamma.value()));
        //m_gradient_lut.add_color(0.9, agg::rgba8::from_wavelength(740, m_gamma.value()));
        //m_gradient_lut.add_color(1.0, agg::rgba8::from_wavelength(780, m_gamma.value()));

        m_gradient_lut.build_lut();
    }

    virtual void on_draw()
    {
        pixfmt pixf(rbuf_window());
        renderer_base rb(pixf);
        renderer_solid rs(rb);
        rb.clear(agg::rgba(1, 1, 1));

        // When Gamma changes rebuild the gamma and gradient LUTs 
        //------------------
        if(m_old_gamma != m_gamma.value())
        {
            m_gamma_lut.gamma(m_gamma.value());
            build_gradient_lut();
            m_old_gamma = m_gamma.value();
        }


        // Gradient center. All gradient functions assume the 
        // center being in the origin (0,0) and you can't 
        // change it. But you can apply arbitrary transformations
        // to the gradient (see below).
        //------------------
        double cx = initial_width()  / 2;
        double cy = initial_height() / 2;
        double r = 100;

        // Focal center. Defined in the gradient coordinates, 
        // that is, with respect to the origin (0,0)
        //------------------
        double fx = m_mouse_x - cx;
        double fy = m_mouse_y - cy;

        gradient_func_type    gradient_func(r, fx, fy);
        gradient_adaptor_type gradient_adaptor(gradient_func);
        agg::trans_affine     gradient_mtx;
        
        // Making the affine matrix. Move to (cx,cy), 
        // apply the resizing transformations and invert
        // the matrix. Gradients and images always assume the
        // inverse transformations.
        //------------------
        gradient_mtx.translate(cx, cy);
        gradient_mtx *= trans_affine_resizing();
        gradient_mtx.invert();

        interpolator_type     span_interpolator(gradient_mtx);
        span_gradient_type    span_gradient(span_interpolator, 
                                          gradient_adaptor, 
                                          m_gradient_lut, 
                                          0, r);

        // Form the simple rectangle 
        //------------------
        m_rasterizer.reset();
        m_rasterizer.move_to_d(0,0);
        m_rasterizer.line_to_d(width(), 0);
        m_rasterizer.line_to_d(width(), height());
        m_rasterizer.line_to_d(0, height());

        // Render the gradient to the whole screen and measure the time
        //------------------
        start_timer();
        agg::render_scanlines_aa(m_rasterizer, m_scanline, rb, m_alloc, span_gradient);
        double tm = elapsed_time();

        // Draw the transformed circle that shows the gradient boundary
        //------------------
        agg::ellipse e(cx, cy, r, r);
        agg::conv_stroke<agg::ellipse> estr(e);
        agg::conv_transform<
            agg::conv_stroke<
                agg::ellipse> > etrans(estr, trans_affine_resizing());

        m_rasterizer.add_path(etrans);
        agg::render_scanlines_aa_solid(m_rasterizer, m_scanline, rb, agg::rgba(1,1,1));

        // Show the gradient time
        //------------------
        char buf[64]; 
        agg::gsv_text t;
        t.size(10.0);
        agg::conv_stroke<agg::gsv_text> pt(t);
        pt.width(1.5);
        sprintf(buf, "%3.2f ms", tm);
        t.start_point(10.0, 35.0);
        t.text(buf);
        m_rasterizer.add_path(pt);
        agg::render_scanlines_aa_solid(m_rasterizer, m_scanline, rb, agg::rgba(0,0,0));

        // Show the controls
        //------------------
        agg::render_ctrl(m_rasterizer, m_scanline, rb, m_gamma);

        // Apply the inverse gamma to the whole buffer 
        // (transform the colors to the perceptually uniform space)
        //------------------
        pixf.apply_gamma_inv(m_gamma_lut);
    }




    virtual void on_mouse_move(int x, int y, unsigned flags)
    {
        if(flags & agg::mouse_left)
        {
            m_mouse_x = x;
            m_mouse_y = y;
            trans_affine_resizing().inverse_transform(&m_mouse_x, &m_mouse_y);
            force_redraw();
        }
    }


    virtual void on_mouse_button_down(int x, int y, unsigned flags)
    {
        if(flags & agg::mouse_left)
        {
            m_mouse_x = x;
            m_mouse_y = y;
            trans_affine_resizing().inverse_transform(&m_mouse_x, &m_mouse_y);
            force_redraw();
        }
    }







};











int agg_main(int argc, char* argv[])
{
    the_application app(pix_format, flip_y);
    app.caption("AGG Example. PDF linear and radial gradients");

    if(app.init(600, 400, agg::window_resize))
    {
        return app.run();
    }
    return 1;
}


