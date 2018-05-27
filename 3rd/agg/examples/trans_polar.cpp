#include "agg_basics.h"
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_u.h"
#include "agg_renderer_scanline.h"
#include "agg_pixfmt_rgb.h"
#include "agg_trans_affine.h"
#include "agg_conv_transform.h"
#include "agg_conv_segmentator.h"
#include "platform/agg_platform_support.h"
#include "ctrl/agg_slider_ctrl.h"
#include "ctrl/agg_cbox_ctrl.h"

#define AGG_BGR24
//#define AGG_BGR96
#include "pixel_formats.h"

enum flip_y_e { flip_y = true };


namespace agg
{


    template<class ColorT, class Ctrl, class Pipeline> class transformed_control
    {
    public:
        transformed_control(Ctrl& ctrl, Pipeline& pl) :
            m_ctrl(ctrl),
            m_pipeline(pl)
        {}
           
        unsigned num_paths() { return m_ctrl.num_paths(); }
        void     rewind(unsigned path_id) { m_pipeline.rewind(path_id); }
        unsigned vertex(double* x, double* y) { return m_pipeline.vertex(x, y); }
        const ColorT color(unsigned i) const { return m_ctrl.color(i); } 

    private:
        Ctrl&     m_ctrl;
        Pipeline& m_pipeline;
    };



    
    class trans_polar
    {
    public:
        trans_polar() :
          m_base_angle(1.0),
          m_base_scale(1.0),
          m_base_x(0.0),
          m_base_y(0.0),
          m_translation_x(0.0),
          m_translation_y(0.0),
          m_spiral(0.0)
        {}

        void base_scale(double v)  { m_base_scale = v; }
        void full_circle(double v) { m_base_angle = 2.0 * pi / v; }
        void base_offset(double dx, double dy) { m_base_x = dx; m_base_y = dy; }
        void translation(double dx, double dy) { m_translation_x = dx; m_translation_y = dy;}
        void spiral(double v) { m_spiral = v; }

        void transform(double* x, double* y) const
        {
            double x1 = (*x + m_base_x) * m_base_angle;
            double y1 = (*y + m_base_y) * m_base_scale + (*x * m_spiral);
            *x = cos(x1) * y1 + m_translation_x;
            *y = sin(x1) * y1 + m_translation_y;
        }

    private:
        double m_base_angle;
        double m_base_scale;
        double m_base_x;
        double m_base_y;
        double m_translation_x;
        double m_translation_y;
        double m_spiral;
    };








}















class the_application : public agg::platform_support
{
    agg::slider_ctrl<color_type> m_slider1;
    agg::slider_ctrl<color_type> m_slider_spiral;
    agg::slider_ctrl<color_type> m_slider_base_y;

public:
    the_application(agg::pix_format_e format, bool flip_y) :
        agg::platform_support(format, flip_y),
        m_slider1      (10, 10,    600-10, 17,    !flip_y),
        m_slider_spiral(10, 10+20, 600-10, 17+20, !flip_y),
        m_slider_base_y(10, 10+40, 600-10, 17+40, !flip_y)
    {
        add_ctrl(m_slider1);
        m_slider1.range(0.0, 100.0);
        m_slider1.num_steps(5);
        m_slider1.value(32.0);
        m_slider1.label("Some Value=%1.0f");

        add_ctrl(m_slider_spiral);
        m_slider_spiral.label("Spiral=%.3f");
        m_slider_spiral.range(-0.1, 0.1);
        m_slider_spiral.value(0.0);

        add_ctrl(m_slider_base_y);
        m_slider_base_y.label("Base Y=%.3f");
        m_slider_base_y.range(50.0, 200.0);
        m_slider_base_y.value(120.0);
    }


    virtual ~the_application()
    {
    }


    virtual void on_init()
    {
    }


    virtual void on_draw()
    {
        typedef agg::renderer_base<pixfmt> ren_base;
        typedef agg::renderer_scanline_aa_solid<ren_base> renderer;

        pixfmt pixf(rbuf_window());
        ren_base rb(pixf);
        renderer ren(rb);
        agg::scanline_u8 sl;

        rb.clear(agg::rgba(1,1,1));

        agg::rasterizer_scanline_aa<> ras; 
       
        agg::render_ctrl(ras, sl, rb, m_slider1);
        agg::render_ctrl(ras, sl, rb, m_slider_spiral);
        agg::render_ctrl(ras, sl, rb, m_slider_base_y);
 

        typedef agg::conv_segmentator<agg::slider_ctrl<color_type> > conv_segmentator_type;
        typedef agg::conv_transform<conv_segmentator_type, agg::trans_polar> conv_transform_type;

        agg::trans_polar trans;
        trans.full_circle(-600);
        trans.base_scale(-1.0);
        trans.base_offset(0.0, m_slider_base_y.value());
        trans.translation(width() / 2.0, height() / 2.0 + 30.0);
        trans.spiral(-m_slider_spiral.value());

        conv_segmentator_type segm(m_slider1);
        conv_transform_type pipeline(segm, trans);

        agg::transformed_control<agg::srgba8, 
                                 agg::slider_ctrl<color_type>, 
                                 conv_transform_type>  ctrl(m_slider1, pipeline);


        agg::render_ctrl(ras, sl, rb, ctrl);
    }


};


int agg_main(int argc, char* argv[])
{
    the_application app(pix_format, flip_y);
    app.caption("AGG Example. Polar Transformer");

    if(app.init(600, 400, agg::window_resize))
    {
        return app.run();
    }
    return 1;
}

