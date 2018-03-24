#include <stdio.h>
#include <stdlib.h>
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_conv_transform.h"
#include "agg_bspline.h"
#include "agg_ellipse.h"
#include "agg_gsv_text.h"
#include "agg_scanline_p.h"
#include "agg_renderer_scanline.h"
#include "ctrl/agg_slider_ctrl.h"
#include "ctrl/agg_scale_ctrl.h"
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
enum default_num_points_e { default_num_points = 10000 };

enum start_size_e
{
    start_width  = 400,
    start_height = 400
};

static double spline_r_x[] = { 0.000000, 0.200000, 0.400000, 0.910484, 0.957258, 1.000000 };
static double spline_r_y[] = { 1.000000, 0.800000, 0.600000, 0.066667, 0.169697, 0.600000 };

static double spline_g_x[] = { 0.000000, 0.292244, 0.485655, 0.564859, 0.795607, 1.000000 };
static double spline_g_y[] = { 0.000000, 0.607260, 0.964065, 0.892558, 0.435571, 0.000000 };

static double spline_b_x[] = { 0.000000, 0.055045, 0.143034, 0.433082, 0.764859, 1.000000 };
static double spline_b_y[] = { 0.385480, 0.128493, 0.021416, 0.271507, 0.713974, 1.000000 };


struct scatter_point
{
    double     x;
    double     y;
    double     z;
    agg::rgba  color;
};


double random_dbl(double start, double end)
{
    unsigned r = rand() & 0x7FFF;
    return double(r) * (end - start) / 32768.0 + start;
}


class the_application : public agg::platform_support
{
    unsigned       m_num_points;
    scatter_point* m_points;

    agg::scale_ctrl<agg::rgba8>  m_scale_ctrl_z;
    agg::slider_ctrl<agg::rgba8> m_slider_ctrl_sel;
    agg::slider_ctrl<agg::rgba8> m_slider_ctrl_size;

    agg::bspline m_spline_r;
    agg::bspline m_spline_g;
    agg::bspline m_spline_b;

public:
    virtual ~the_application()
    {
        delete [] m_points;
    }

    the_application(agg::pix_format_e format, bool flip_y, unsigned num_points) :
        agg::platform_support(format, flip_y),
        m_num_points(num_points),
        m_points(new scatter_point [num_points]),
        m_scale_ctrl_z    (5, 5,  start_width-5, 12, !flip_y),
        m_slider_ctrl_sel (5, 20, start_width-5, 27, !flip_y),
        m_slider_ctrl_size(5, 35, start_width-5, 42, !flip_y)
    {

        m_spline_r.init(6, spline_r_x, spline_r_y);
        m_spline_g.init(6, spline_g_x, spline_g_y);
        m_spline_b.init(6, spline_b_x, spline_b_y);

        add_ctrl(m_scale_ctrl_z);
        add_ctrl(m_slider_ctrl_sel);
        add_ctrl(m_slider_ctrl_size);

        m_slider_ctrl_size.label("Size");
        m_slider_ctrl_sel.label("Selectivity");
    }

    
    void generate()
    {
        unsigned i;

        double rx = initial_width()/3.5;
        double ry = initial_height()/3.5;

        for(i = 0; i < m_num_points; i++)
        {
            double z = m_points[i].z = random_dbl(0.0, 1.0);
            double x = cos(z * 2.0 * agg::pi) * rx;
            double y = sin(z * 2.0 * agg::pi) * ry;

            double dist  = random_dbl(0.0, rx/2.0);
            double angle = random_dbl(0.0, agg::pi * 2.0);

            m_points[i].x = initial_width()/2.0  + x + cos(angle) * dist;
            m_points[i].y = initial_height()/2.0 + y + sin(angle) * dist;
            m_points[i].color = agg::rgba(m_spline_r.get(z)*0.8, 
                                          m_spline_g.get(z)*0.8, 
                                          m_spline_b.get(z)*0.8, 
                                          1.0);
        }
    }


    virtual void on_init()
    {
        generate();
    }


    virtual void on_draw()
    {
        agg::rasterizer_scanline_aa<> pf;
        agg::scanline_p8 sl;

        typedef agg::renderer_base<pixfmt> renderer_base;

        pixfmt pixf(rbuf_window());
        renderer_base rb(pixf);

        rb.clear(agg::rgba(1,1,1));
      
        agg::ellipse e1;
        agg::conv_transform<agg::ellipse> t1(e1, trans_affine_resizing());


        unsigned i;
        unsigned n_drawn = 0;
        for(i = 0; i < m_num_points; i++)
        {
            double z = m_points[i].z;
            double alpha = 1.0;
            if(z < m_scale_ctrl_z.value1())
            {
                alpha = 
                    1.0 - 
                    (m_scale_ctrl_z.value1() - z) * 
                        m_slider_ctrl_sel.value() * 100.0;
            }

            if(z > m_scale_ctrl_z.value2())
            {
                alpha = 
                    1.0 - 
                    (z - m_scale_ctrl_z.value2()) * 
                        m_slider_ctrl_sel.value() * 100.0;
            }



            if(alpha > 1.0) alpha = 1.0;
            if(alpha < 0.0) alpha = 0.0;

            if(alpha > 0.0)
            {
                e1.init(m_points[i].x, 
                        m_points[i].y, 
                        m_slider_ctrl_size.value() * 5.0, 
                        m_slider_ctrl_size.value() * 5.0, 
                        8);
                pf.add_path(t1);

                agg::render_scanlines_aa_solid(
                    pf, sl, rb,
                    agg::rgba(m_points[i].color.r, 
                                   m_points[i].color.g,
                                   m_points[i].color.b,
                                   alpha));
                n_drawn++;
            }
        }

        agg::render_ctrl(pf, sl, rb, m_scale_ctrl_z);
        agg::render_ctrl(pf, sl, rb, m_slider_ctrl_sel);
        agg::render_ctrl(pf, sl, rb, m_slider_ctrl_size);

        char buf[10];
        sprintf(buf, "%08u", n_drawn);

        agg::gsv_text txt;
        txt.size(15.0);
        txt.text(buf);
        txt.start_point(10.0, initial_height() - 20.0);
        agg::gsv_text_outline<> txt_o(txt, trans_affine_resizing());
        pf.add_path(txt_o);
        agg::render_scanlines_aa_solid(pf, sl, rb, agg::rgba(0,0,0));

    }


    virtual void on_idle()
    {
        unsigned i;
        for(i = 0; i < m_num_points; i++)
        {
            m_points[i].x += random_dbl(0, m_slider_ctrl_sel.value()) - m_slider_ctrl_sel.value()*0.5;
            m_points[i].y += random_dbl(0, m_slider_ctrl_sel.value()) - m_slider_ctrl_sel.value()*0.5;
            m_points[i].z += random_dbl(0, m_slider_ctrl_sel.value()*0.01) - m_slider_ctrl_sel.value()*0.005;
            if(m_points[i].z < 0.0) m_points[i].z = 0.0;
            if(m_points[i].z > 1.0) m_points[i].z = 1.0;
        }
        force_redraw();
    }


    virtual void on_mouse_button_down(int x, int y, unsigned flags)
    {
        if(flags & agg::mouse_left)
        {
            generate();
            force_redraw();
        }

        if(flags & agg::mouse_right)
        {
            wait_mode(!wait_mode());
        }
    }

};



int agg_main(int argc, char* argv[])
{
    unsigned num_points = default_num_points;
    if(argc > 1)
    {
        num_points = atoi(argv[1]);
        if(num_points == 0)    num_points = default_num_points;
        if(num_points > 20000) num_points = 20000;
    }

    the_application app(pix_format, flip_y, num_points);
    app.caption("AGG Drawing random circles - A scatter plot prototype");

    if(app.init(start_width, start_height, agg::window_resize | agg::window_keep_aspect_ratio))
    {
        return app.run();
    }
    return 1;
}


