#include <stdio.h>
#include "agg_basics.h"
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_u.h"
#include "agg_renderer_scanline.h"
#include "agg_blur.h"
#include "util/agg_color_conv.h"
#include "platform/agg_platform_support.h"
#include "ctrl/agg_slider_ctrl.h"
#include "ctrl/agg_cbox_ctrl.h"

//#define AGG_GRAY8
//#define AGG_GRAY32
//#define AGG_BGR24
#define AGG_BGR96
#include "pixel_formats.h"

enum flip_y_e { flip_y = true };

class the_application : public agg::platform_support
{
    agg::slider_ctrl<color_type> m_slider1;
    agg::slider_ctrl<color_type> m_slider2;
    agg::cbox_ctrl<color_type> m_cbox1;
    agg::cbox_ctrl<color_type> m_cbox2;

public:
    the_application(agg::pix_format_e format, bool flip_y) :
        agg::platform_support(format, flip_y),
        m_slider1(10, 10,    640-10, 19,    !flip_y),
        m_slider2(10, 10+20, 640-10, 19+20, !flip_y),
        m_cbox1(10, 10+40, "Monochrome", !flip_y),
        m_cbox2(10, 10+60, "Invert", !flip_y)
    {
        add_ctrl(m_slider1);
        add_ctrl(m_slider2);
        add_ctrl(m_cbox1);
        add_ctrl(m_cbox2);

        m_slider1.label("Line thickness=%1.2f");
        m_slider1.range(0.0, 5.0);
        m_slider1.value(1.0);

        m_slider2.label("Blur radius=%1.2f");
        m_slider2.range(0.0, 2.0);
        m_slider2.value(1.5);

        m_cbox1.status(true);
        m_cbox2.status(false);

        m_slider1.no_transform();
        m_slider2.no_transform();
        m_cbox1.no_transform();
        m_cbox2.no_transform();
    }

    template<class T>
    void SetCtrlClr(T & ctrl, agg::rgba const & clr)
    {
        ctrl.text_color(clr);
        ctrl.inactive_color(clr);
        ctrl.active_color(clr);
    }

    virtual void on_draw()
    {
        pixfmt pf(rbuf_window());
        agg::renderer_base<pixfmt> ren(pf);
        agg::scanline_u8 sl;
        agg::rasterizer_scanline_aa<> ras;
        agg::path_storage ps;
        agg::conv_stroke<agg::path_storage> pg(ps);

        agg::rgba clr1 = m_cbox1.status() ? agg::rgba(1, 1, 1) : agg::rgba(1, 0, 1);
        agg::rgba clr2 = m_cbox1.status() ? agg::rgba(0, 0, 0) : agg::rgba(0, 1, 0);
        agg::rgba foreground = m_cbox2.status() ? clr1 : clr2;
        agg::rgba background = m_cbox2.status() ? clr2 : clr1;

        SetCtrlClr(m_cbox1, foreground);
        SetCtrlClr(m_cbox2, foreground);

        ren.clear(background);

        // Draw row of straight lines.
        for (int i = 0; i < 20; ++i)
        {
            pg.width(m_slider1.value() * 0.3 * (i + 1));
            ps.remove_all();
            ps.move_to(20 + 30 * i, 310);
            ps.line_to(40 + 30 * i, 460);
            ras.add_path(pg);
            agg::render_scanlines_aa_solid(ras, sl, ren, foreground);
        }

        // Draw wheel of lines.
        for (int i = 0; i < 40; ++i)
        {
            pg.width(m_slider1.value());
            ps.remove_all();
            ps.move_to(320 + 20 * sin(i * agg::pi / 20), 180 + 20 * cos(i * agg::pi / 20));
            ps.line_to(320 + 100 * sin(i * agg::pi / 20), 180 + 100 * cos(i * agg::pi / 20));
            ras.add_path(pg);
            agg::render_scanlines_aa_solid(ras, sl, ren, foreground);
        }

        // Apply blur.
        start_timer();
        agg::apply_slight_blur(ren, m_slider2.value());
        double tm = elapsed_time();

        // Display the blur time.
        char buf[64]; 
        agg::gsv_text t;
        t.size(10.0);
        agg::conv_stroke<agg::gsv_text> st(t);
        st.width(1.5);
        sprintf(buf, "Blur: %3.2f ms", tm);
        t.start_point(10.0, 90.0);
        t.text(buf);
        ras.add_path(st);
        agg::render_scanlines_aa_solid(ras, sl, ren, foreground);

        // Render the controls
        render_ctrl(ras, sl, ren, m_slider1);
        render_ctrl(ras, sl, ren, m_slider2);
        render_ctrl(ras, sl, ren, m_cbox1);
        render_ctrl(ras, sl, ren, m_cbox2);
    }
};


int agg_main(int argc, char* argv[])
{
    the_application app(pix_format, flip_y);
    app.caption("Anti-aliased lines with blurring");

    if(app.init(640, 480, agg::window_resize))
    {
        return app.run();
    }
    return 1;
}


