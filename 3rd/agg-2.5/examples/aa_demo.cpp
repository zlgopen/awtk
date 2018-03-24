#include "agg_basics.h"
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_u.h"
#include "agg_renderer_scanline.h"
#include "agg_pixfmt_rgb.h"
#include "platform/agg_platform_support.h"
#include "ctrl/agg_slider_ctrl.h"
#include "ctrl/agg_cbox_ctrl.h"


enum flip_y_e { flip_y = true };


namespace agg
{

    class square
    {
    public:
        square(double size) : m_size(size) {}

        template<class Rasterizer, class Scanline, class Renderer, class ColorT> 
        void draw(Rasterizer& ras, Scanline& sl, Renderer& ren, ColorT color, 
                  double x, double y)
        {
            ras.reset();
            ras.move_to_d(x*m_size,        y*m_size);
            ras.line_to_d(x*m_size+m_size, y*m_size);
            ras.line_to_d(x*m_size+m_size, y*m_size+m_size);
            ras.line_to_d(x*m_size,        y*m_size+m_size);
            agg::render_scanlines_aa_solid(ras, sl, ren, color);
        }

    private:
        double m_size;
    };



    template<class Renderer> class renderer_enlarged
    {
    public:
        renderer_enlarged(Renderer& ren, double size) : 
            m_ren(ren),
            m_square(size), 
            m_size(size) {}

        //--------------------------------------------------------------------
        void color(rgba8 c) { m_color = c; }

        //--------------------------------------------------------------------
        void prepare() {}

        //--------------------------------------------------------------------
        template<class Scanline> void render(const Scanline& sl)
        {
            int y = sl.y();

            unsigned num_spans = sl.num_spans();
            typename Scanline::const_iterator span = sl.begin();

            do
            {
                int x = span->x;
                const typename Scanline::cover_type* covers = span->covers;
                int num_pix = span->len;
                
                do 
                {
                    int a = (*covers++ * m_color.a) >> 8;
                    m_square.draw(m_ras, m_sl, m_ren, 
                                  rgba8(m_color.r, m_color.g, m_color.b, a),
                                  x, y);
                    ++x;
                }
                while(--num_pix);
            }
            while(--num_spans);
        }

    private:
        rasterizer_scanline_aa<> m_ras;
        scanline_u8 m_sl;
        Renderer&   m_ren;
        square      m_square;
        rgba8       m_color;
        double      m_size;
    };



};



















class the_application : public agg::platform_support
{
    double m_x[3];
    double m_y[3];
    double m_dx;
    double m_dy;
    int    m_idx;

    agg::slider_ctrl<agg::rgba8> m_slider1;
    agg::slider_ctrl<agg::rgba8> m_slider2;

public:
    the_application(agg::pix_format_e format, bool flip_y) :
        agg::platform_support(format, flip_y),
        m_slider1(80, 10,    600-10, 19,    !flip_y),
        m_slider2(80, 10+20, 600-10, 19+20, !flip_y)
    {
        m_idx = -1;
        m_x[0] = 57;    m_y[0] = 100;
        m_x[1] = 369;   m_y[1] = 170;
        m_x[2] = 143;   m_y[2] = 310;

        add_ctrl(m_slider1);
        add_ctrl(m_slider2);

        m_slider1.range(8.0, 100.0);
        m_slider1.num_steps(23);
        m_slider1.value(32.0);

        m_slider2.range(0.1, 3.0);
        m_slider2.value(1.0);

        m_slider1.label("Pixel size=%1.0f");
        m_slider2.label("Gamma=%4.3f");

        m_slider1.no_transform();
        m_slider2.no_transform();
    }


    virtual ~the_application()
    {
    }


    virtual void on_init()
    {
    }


    virtual void on_draw()
    {
        typedef agg::renderer_base<agg::pixfmt_bgr24> ren_base;

        agg::pixfmt_bgr24 pixf(rbuf_window());
        ren_base ren(pixf);
        agg::scanline_u8 sl;

        ren.clear(agg::rgba(1,1,1));

        agg::rasterizer_scanline_aa<> ras;

        int size_mul = int(m_slider1.value());

        ras.gamma(agg::gamma_power(m_slider2.value()));


        agg::renderer_enlarged<ren_base> ren_en(ren, size_mul);

        ras.reset();
        ras.move_to_d(m_x[0]/size_mul, m_y[0]/size_mul);
        ras.line_to_d(m_x[1]/size_mul, m_y[1]/size_mul);
        ras.line_to_d(m_x[2]/size_mul, m_y[2]/size_mul);
        ren_en.color(agg::rgba8(0,0,0, 255));
        agg::render_scanlines(ras, sl, ren_en);


        agg::render_scanlines_aa_solid(ras, sl, ren, agg::rgba8(0,0,0));

        ras.gamma(agg::gamma_none());

        agg::path_storage ps;
        agg::conv_stroke<agg::path_storage> pg(ps);
        pg.width(2.0);

        ps.remove_all();
        ps.move_to(m_x[0], m_y[0]);
        ps.line_to(m_x[1], m_y[1]);
        ras.add_path(pg);
        agg::render_scanlines_aa_solid(ras, sl, ren, agg::rgba8(0,150,160, 200));

        ps.remove_all();
        ps.move_to(m_x[1], m_y[1]);
        ps.line_to(m_x[2], m_y[2]);
        ras.add_path(pg);
        agg::render_scanlines_aa_solid(ras, sl, ren, agg::rgba8(0,150,160, 200));

        ps.remove_all();
        ps.move_to(m_x[2], m_y[2]);
        ps.line_to(m_x[0], m_y[0]);
        ras.add_path(pg);
        agg::render_scanlines_aa_solid(ras, sl, ren, agg::rgba8(0,150,160, 200));

        // Render the controls
        agg::render_ctrl(ras, sl, ren, m_slider1);
        agg::render_ctrl(ras, sl, ren, m_slider2);
    }




    virtual void on_mouse_button_down(int x, int y, unsigned flags)
    {
        if(flags & agg::mouse_left)
        {
            unsigned i;
            for (i = 0; i < 3; i++)
            {
                if(sqrt( (x-m_x[i]) * (x-m_x[i]) + (y-m_y[i]) * (y-m_y[i]) ) < 10.0)
                {
                    m_dx = x - m_x[i];
                    m_dy = y - m_y[i];
                    m_idx = i;
                    break;
                }
            }
            if(i == 3)
            {
                if(agg::point_in_triangle(m_x[0], m_y[0], 
                                          m_x[1], m_y[1],
                                          m_x[2], m_y[2],
                                          x, y))
                {
                    m_dx = x - m_x[0];
                    m_dy = y - m_y[0];
                    m_idx = 3;
                }
            }
        }
    }


    virtual void on_mouse_move(int x, int y, unsigned flags)
    {
        if(flags & agg::mouse_left)
        {
            if(m_idx == 3)
            {
                double dx = x - m_dx;
                double dy = y - m_dy;
                m_x[1] -= m_x[0] - dx;
                m_y[1] -= m_y[0] - dy;
                m_x[2] -= m_x[0] - dx;
                m_y[2] -= m_y[0] - dy;
                m_x[0] = dx;
                m_y[0] = dy;
                force_redraw();
                return;
            }

            if(m_idx >= 0)
            {
                m_x[m_idx] = x - m_dx;
                m_y[m_idx] = y - m_dy;
                force_redraw();
            }
        }
        else
        {
            on_mouse_button_up(x, y, flags);
        }
    }

    virtual void on_mouse_button_up(int x, int y, unsigned flags)
    {
        m_idx = -1;
    }
};


int agg_main(int argc, char* argv[])
{
    the_application app(agg::pix_format_bgr24, flip_y);
    app.caption("AGG Example. Anti-Aliasing Demo");

    if(app.init(600, 400, agg::window_resize))
    {
        return app.run();
    }
    return 1;
}


