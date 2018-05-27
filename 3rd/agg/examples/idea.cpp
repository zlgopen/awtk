#include "agg_basics.h"
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_p.h"
#include "agg_renderer_scanline.h"
#include "agg_trans_affine.h"
#include "agg_conv_stroke.h"
#include "agg_conv_transform.h"
#include "ctrl/agg_cbox_ctrl.h"
#include "ctrl/agg_slider_ctrl.h"
#include "platform/agg_platform_support.h"

#define AGG_BGR24
//#define AGG_RGB24
//#define AGG_BGRA32 
//#define AGG_RGBA32 
//#define AGG_ARGB32 
//#define AGG_ABGR32
//#define AGG_BGR96
//#define AGG_BGRA128
//#define AGG_RGB565
//#define AGG_RGB555
#include "pixel_formats.h"

enum flip_y_e { flip_y = false };


struct path_attributes
{
    unsigned   index;
    agg::srgba8 fill_color;
    agg::srgba8 stroke_color;
    double     stroke_width;

    path_attributes() {}
    path_attributes(unsigned idx, 
                    const agg::srgba8& fill,
                    const agg::srgba8& stroke,
                    double width) :
      index(idx),
      fill_color(fill),
      stroke_color(stroke),
      stroke_width(width)
    {
    }
};



static double g_poly_bulb[] = 
{
    -6,-67,    -6,-71,   -7,-74,    -8,-76,    -10,-79,
    -10,-82,   -9,-84,   -6,-86,    -4,-87,    -2,-86,
    -1,-86,     1,-84,    2,-82,     2,-79,     0,-77,
    -2,-73,    -2,-71,   -2,-69,    -3,-67,    -4,-65
};

static double g_poly_beam1[] = 
{
    -14,-84,-22,-85,-23,-87,-22,-88,-21,-88
};

static double g_poly_beam2[] = 
{
   -10,-92,   -14,-96,   -14,-98,   -12,-99,   -11,-97
};

static double g_poly_beam3[] = 
{
   -1,-92,     -2,-98,    0,-100,    2,-100,    1,-98
};

static double g_poly_beam4[] = 
{
    5,-89,      11,-94,   13,-93,    13,-92,    12,-91
};


static double g_poly_fig1[] = 
{
    1,-48,-3,-54,-7,-58,-12,-58,-17,-55,-20,-52,-21,-47,
    -20,-40,-17,-33,-11,-28,-6,-26,-2,-25,2,-26,4,-28,5,
    -33,5,-39,3,-44,12,-48,12,-50,12,-51,3,-46
};


static double g_poly_fig2[] = 
{
    11,-27,6,-23,4,-22,3,-19,5,
    -16,6,-15,11,-17,19,-23,25,-30,32,-38,32,-41,32,-50,30,-64,32,-72,
    32,-75,31,-77,28,-78,26,-80,28,-87,27,-89,25,-88,24,-79,24,-76,23,
    -75,20,-76,17,-76,17,-74,19,-73,22,-73,24,-71,26,-69,27,-64,28,-55,
    28,-47,28,-40,26,-38,20,-33,14,-30
};


static double g_poly_fig3[] = 
{
    -6,-20,-9,-21,-15,-21,-20,-17,
    -28,-8,-32,-1,-32,1,-30,6,-26,8,-20,10,-16,12,-14,14,-15,16,-18,20,
    -22,20,-25,19,-27,20,-26,22,-23,23,-18,23,-14,22,-11,20,-10,17,-9,14,
    -11,11,-16,9,-22,8,-26,5,-28,2,-27,-2,-23,-8,-19,-11,-12,-14,-6,-15,
    -6,-18
};


static double g_poly_fig4[] = 
{
    11,-6,8,-16,5,-21,-1,-23,-7,
   -22,-10,-17,-9,-10,-8,0,-8,10,-10,18,-11,22,-10,26,-7,28,-3,30,0,31,
    5,31,10,27,14,18,14,11,11,2
};


static double g_poly_fig5[] = 
{
    0,22,-5,21,-8,22,-9,26,-8,49,
    -8,54,-10,64,-10,75,-9,81,-10,84,-16,89,-18,95,-18,97,-13,100,-12,99,
    -12,95,-10,90,-8,87,-6,86,-4,83,-3,82,-5,80,-6,79,-7,74,-6,63,-3,52,
    0,42,1,31
};


static double g_poly_fig6[] = 
{
    12,31,12,24,8,21,3,21,2,24,3,
    30,5,40,8,47,10,56,11,64,11,71,10,76,8,77,8,79,10,81,13,82,17,82,26,
    84,28,87,32,86,33,81,32,80,25,79,17,79,14,79,13,76,14,72,14,64,13,55,
    12,44,12,34
};



static path_attributes g_attr[3];
static agg::path_storage g_path;
static unsigned g_npaths = 0;
static agg::filling_rule_e g_pflag = agg::fill_non_zero;
static agg::rasterizer_scanline_aa<> g_rasterizer;
static agg::scanline_p8              g_scanline;
static double g_angle = 0.0;

#define AGG_POLY_SIZE(p) (sizeof(p) / (sizeof(*p) * 2))



struct trans_roundoff
{
    static void transform(double* x, double* y)
    {
        *x = floor(*x + 0.5);
        *y = floor(*y + 0.5);
    }
};


class the_application : public agg::platform_support
{
    double m_dx;
    double m_dy;
    agg::cbox_ctrl<color_type> m_rotate;
    agg::cbox_ctrl<color_type> m_even_odd;
    agg::cbox_ctrl<color_type> m_draft;
    agg::cbox_ctrl<color_type> m_roundoff;
    agg::slider_ctrl<color_type> m_angle_delta;
    bool m_redraw_flag;

public:
    the_application(agg::pix_format_e format, bool flip_y) :
        agg::platform_support(format, flip_y),
        m_rotate(10,  3, "Rotate", !flip_y),
        m_even_odd(60, 3, "Even-Odd", !flip_y),
        m_draft(130, 3, "Draft", !flip_y),
        m_roundoff(175, 3, "Roundoff", !flip_y),
        m_angle_delta(10, 21, 250-10, 27, !flip_y),
        m_redraw_flag(true)
    {
        m_angle_delta.label("Step=%4.3f degree");

        g_attr[g_npaths++] = path_attributes(g_path.start_new_path(),
                                             agg::srgba8(255, 255, 0),
                                             agg::srgba8(0,   0,   0),
                                             1.0);

        g_path.concat_poly(g_poly_bulb, AGG_POLY_SIZE(g_poly_bulb), true);

        g_attr[g_npaths++] = path_attributes(g_path.start_new_path(),
                                             agg::srgba8(255,  255, 200),
                                             agg::srgba8(90,   0,   0),
                                             0.7);

        g_path.concat_poly(g_poly_beam1, AGG_POLY_SIZE(g_poly_beam1), true); 
        g_path.concat_poly(g_poly_beam2, AGG_POLY_SIZE(g_poly_beam2), true); 
        g_path.concat_poly(g_poly_beam3, AGG_POLY_SIZE(g_poly_beam3), true); 
        g_path.concat_poly(g_poly_beam4, AGG_POLY_SIZE(g_poly_beam4), true); 

        g_attr[g_npaths++] = path_attributes(g_path.start_new_path(),
                                             agg::srgba8(0, 0, 0),
                                             agg::srgba8(0, 0, 0),
                                             0.0);

        g_path.concat_poly(g_poly_fig1, AGG_POLY_SIZE(g_poly_fig1), true); 
        g_path.concat_poly(g_poly_fig2, AGG_POLY_SIZE(g_poly_fig2), true); 
        g_path.concat_poly(g_poly_fig3, AGG_POLY_SIZE(g_poly_fig3), true); 
        g_path.concat_poly(g_poly_fig4, AGG_POLY_SIZE(g_poly_fig4), true); 
        g_path.concat_poly(g_poly_fig5, AGG_POLY_SIZE(g_poly_fig5), true); 
        g_path.concat_poly(g_poly_fig6, AGG_POLY_SIZE(g_poly_fig6), true); 

        m_rotate.text_size(7);
        m_even_odd.text_size(7);
        m_draft.text_size(7);
        m_roundoff.text_size(7);
        add_ctrl(m_rotate);
        add_ctrl(m_even_odd);
        add_ctrl(m_draft);
        add_ctrl(m_roundoff);
        add_ctrl(m_angle_delta);
        m_angle_delta.value(0.01);
    }


    virtual void on_init()
    {
        m_dx = rbuf_window().width();
        m_dy = rbuf_window().height();
    }

    virtual void on_resize(int, int)
    {
        m_redraw_flag = true;
    }

    virtual void on_draw()
    {
        typedef agg::renderer_base<pixfmt> ren_base;

        pixfmt pixf(rbuf_window());
        ren_base rbase(pixf);
        trans_roundoff roundoff;

        if(m_redraw_flag)
        {
            g_rasterizer.gamma(agg::gamma_none());
            rbase.clear(agg::srgba8(255,255,255));
            g_rasterizer.filling_rule(agg::fill_non_zero);
            agg::render_ctrl(g_rasterizer, g_scanline, rbase, m_rotate);
            agg::render_ctrl(g_rasterizer, g_scanline, rbase, m_even_odd);
            agg::render_ctrl(g_rasterizer, g_scanline, rbase, m_draft);
            agg::render_ctrl(g_rasterizer, g_scanline, rbase, m_roundoff);
            agg::render_ctrl(g_rasterizer, g_scanline, rbase, m_angle_delta);
            m_redraw_flag = false;
        }
        else
        {
            rbase.copy_bar(0, 
                           int(32.0 * rbuf_window().height() / m_dy), 
                           rbuf_window().width(), 
                           rbuf_window().height(), 
                           agg::srgba8(255,255,255));
        }


        if(m_draft.status())
        {
            g_rasterizer.gamma(agg::gamma_threshold(0.4));
        }

        agg::trans_affine mtx;
        mtx.reset();
        mtx *= agg::trans_affine_rotation(g_angle * agg::pi / 180.0);
        mtx *= agg::trans_affine_translation(m_dx / 2, m_dy / 2 + 10);
        mtx *= agg::trans_affine_scaling(rbuf_window().width() / m_dx, 
                                         rbuf_window().height() / m_dy);

        agg::conv_transform<agg::path_storage> fill(g_path, mtx);
        agg::conv_transform
        <
            agg::conv_transform<agg::path_storage>, 
            trans_roundoff
        >
        fill_roundoff(fill, roundoff);

        agg::conv_stroke
        <
            agg::conv_transform<agg::path_storage>
        > 
        stroke(fill);

        agg::conv_stroke
        <
            agg::conv_transform
            <
                agg::conv_transform<agg::path_storage>, 
                trans_roundoff
            >
        > 
        stroke_roundoff(fill_roundoff);

        g_pflag = m_even_odd.status() ? agg::fill_even_odd : agg::fill_non_zero;

        unsigned i;
        for(i = 0; i < g_npaths; i++)
        {
            g_rasterizer.filling_rule(g_pflag);
            if(m_roundoff.status()) g_rasterizer.add_path(fill_roundoff, g_attr[i].index);
            else                    g_rasterizer.add_path(fill,          g_attr[i].index);

            if(m_draft.status())
            {
                agg::render_scanlines_bin_solid(g_rasterizer, g_scanline, rbase, g_attr[i].fill_color);
            }
            else
            {
                agg::render_scanlines_aa_solid(g_rasterizer, g_scanline, rbase, g_attr[i].fill_color);
            }

            if(g_attr[i].stroke_width > 0.001)
            {
                stroke.width(g_attr[i].stroke_width * mtx.scale());
                stroke_roundoff.width(g_attr[i].stroke_width * mtx.scale());
                if(m_roundoff.status()) g_rasterizer.add_path(stroke_roundoff, g_attr[i].index);
                else                    g_rasterizer.add_path(stroke,          g_attr[i].index);
                if(m_draft.status())
                {
                    agg::render_scanlines_bin_solid(g_rasterizer, g_scanline, rbase, g_attr[i].stroke_color);
                }
                else
                {
                    agg::render_scanlines_aa_solid(g_rasterizer, g_scanline, rbase, g_attr[i].stroke_color);
                }
            }
        }
    }

    virtual void on_idle()
    {
        g_angle += m_angle_delta.value();
        if(g_angle > 360.0) g_angle -= 360.0;
        force_redraw();
    }

    virtual void on_ctrl_change()
    {
        wait_mode(!m_rotate.status());
        m_redraw_flag = true;
    }

};




int agg_main(int argc, char* argv[])
{
    the_application app(pix_format, flip_y);
    app.caption("AGG Example. Idea");

    if(app.init(250, 280, agg::window_resize))
    {
        return app.run();
    }
    return 1;
}


