#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "agg_rendering_buffer.h"
#include "agg_trans_viewport.h"
#include "agg_path_storage.h"
#include "agg_conv_transform.h"
#include "agg_conv_curve.h"
#include "agg_conv_stroke.h"
#include "agg_gsv_text.h"
#include "agg_scanline_u.h"
#include "agg_scanline_bin.h"
#include "agg_renderer_scanline.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_rasterizer_compound_aa.h"
#include "agg_span_allocator.h"
#include "agg_gamma_lut.h"
#include "agg_pixfmt_rgba.h"
#include "agg_bounding_rect.h"
#include "agg_color_gray.h"
#include "platform/agg_platform_support.h"



enum { flip_y = false };

typedef agg::pixfmt_bgra32_pre pixfmt;



namespace agg
{
    struct path_style
    {
        unsigned path_id;
        int left_fill;
        int right_fill;
        int line;
    };

    class compound_shape
    {
    public:
        ~compound_shape() 
        { 
            if(m_fd)
            {
                fclose(m_fd);
            }
        }

        compound_shape() :
            m_path(),
            m_affine(),
            m_curve(m_path),
            m_trans(m_curve, m_affine),
            m_styles()
        {}

        bool open(const char* fname)
        {
            m_fd = fopen(fname, "r");
            return m_fd != 0;
        }

        bool read_next()
        {
            m_path.remove_all();
            m_styles.remove_all();
            const char space[] = " \t\n\r";
            double ax, ay, cx, cy;
            if(m_fd)
            {
                char buf[1024];
                char* ts;

                for(;;)
                {
                    if(fgets(buf, 1022, m_fd) == 0) return false;
                    if(buf[0] == '=') break;
                }

                while(fgets(buf, 1022, m_fd))
                {
                    if(buf[0] == '!') break;
                    if(buf[0] == 'P')
                    {
                        // BeginPath
                        path_style style;
                        style.path_id = m_path.start_new_path();
                        ts = strtok(buf, space); // Path;
                        ts = strtok(0, space);  // left_style
                        style.left_fill = atoi(ts);
                        ts = strtok(0, space);  // right_style
                        style.right_fill = atoi(ts);
                        ts = strtok(0, space);  // line_style
                        style.line = atoi(ts);
                        ts = strtok(0, space);  // ax
                        ax = atof(ts);
                        ts = strtok(0, space);  // ay
                        ay = atof(ts);
                        m_path.move_to(ax, ay);
                        m_styles.add(style);
                    }


                    if(buf[0] == 'C')
                    {
                        ts = strtok(buf, space); // Curve;
                        ts = strtok(0, space);  // cx
                        cx = atof(ts);
                        ts = strtok(0, space);  // cy
                        cy = atof(ts);
                        ts = strtok(0, space);  // ax
                        ax = atof(ts);
                        ts = strtok(0, space);  // ay
                        ay = atof(ts);
                        m_path.curve3(cx, cy, ax, ay);
                    }

                    if(buf[0] == 'L')
                    {
                        ts = strtok(buf, space); // Line;
                        ts = strtok(0, space);  // ax
                        ax = atof(ts);
                        ts = strtok(0, space);  // ay
                        ay = atof(ts);
                        m_path.line_to(ax, ay);
                    }


                    if(buf[0] == '<')
                    {
                        // EndPath
                    }
                }
                return true;
            }
            return false;
        }


        unsigned operator [] (unsigned i) const 
        {
            return m_styles[i].path_id;
        }

        unsigned paths() const { return m_styles.size(); }
        const path_style& style(unsigned i) const
        {
            return m_styles[i];
        }

        void rewind(unsigned path_id)
        {
            m_trans.rewind(path_id);
        }

        unsigned vertex(double* x, double* y)
        {
            return m_trans.vertex(x, y);
        }

        double scale() const
        {
            return m_affine.scale();
        }

        void scale(double w, double h)
        {
            m_affine.reset();
            double x1, y1, x2, y2;
            bounding_rect(m_path, *this, 0, m_styles.size(), 
                          &x1, &y1, &x2, &y2);
            if(x1 < x2 && y1 < y2)
            {
                trans_viewport vp;
                vp.preserve_aspect_ratio(0.5, 0.5, aspect_ratio_meet);
                vp.world_viewport(x1, y1, x2, y2);
                vp.device_viewport(0, 0, w, h);
                m_affine = vp.to_affine();
            }
            m_curve.approximation_scale(m_affine.scale());
        }

        void approximation_scale(double s)
        {
            m_curve.approximation_scale(m_affine.scale() * s);
        }

        int hit_test(double x, double y, double r)
        {
            m_affine.inverse_transform(&x, &y);
            r /= m_affine.scale();
            unsigned i;
            for(i = 0; i < m_path.total_vertices(); i++)
            {
                double vx, vy;
                unsigned cmd = m_path.vertex(i, &vx, &vy);
                if(is_vertex(cmd))
                {
                    if(calc_distance(x, y, vx, vy) <= r)
                    {
                        return i;
                    }
                }
            }
            return -1;
        }

        void modify_vertex(unsigned i, double x, double y)
        {
            m_affine.inverse_transform(&x, &y);
            m_path.modify_vertex(i, x, y);
        }

    private:
        path_storage                              m_path;
        trans_affine                              m_affine;
        conv_curve<path_storage>                  m_curve;
        conv_transform<conv_curve<path_storage> > m_trans;
        pod_bvector<path_style>                   m_styles;
        double                                    m_x1, m_y1, m_x2, m_y2;

        FILE* m_fd;
    };



    // Testing class, color provider and span generator
    //-------------------------------------------------
    class test_styles
    {
    public:
        test_styles(const rgba8* solid_colors, 
                    const rgba8* gradient) : 
            m_solid_colors(solid_colors),
            m_gradient(gradient)
        {}

        // Suppose that style=1 is a gradient
        //---------------------------------------------
        bool is_solid(unsigned style) const 
        { 
            return true;//style != 1; 
        }

        // Just returns a color
        //---------------------------------------------
        const rgba8& color(unsigned style) const 
        { 
            return m_solid_colors[style]; 
        }

        // Generate span. In our test case only one style (style=1)
        // can be a span generator, so that, parameter "style"
        // isn't used here.
        //---------------------------------------------
        void generate_span(rgba8* span, int x, int y, unsigned len, unsigned style)
        {
            memcpy(span, m_gradient + x, sizeof(rgba8) * len);
        }

    private:
        const rgba8* m_solid_colors;
        const rgba8* m_gradient;
    };




}






class the_application : public agg::platform_support
{

public:
    agg::compound_shape        m_shape;
    agg::rgba8                 m_colors[100];
    agg::trans_affine          m_scale;
    agg::gamma_lut<>           m_gamma;
    agg::pod_array<agg::rgba8> m_gradient;
    int                        m_point_idx;
    int                        m_hit_x;
    int                        m_hit_y;

    the_application(agg::pix_format_e format, bool flip_y) :
        agg::platform_support(format, flip_y),
        m_point_idx(-1),
        m_hit_x(-1),
        m_hit_y(-1)
    {
        m_gamma.gamma(2.0);

        for(unsigned i = 0; i < 100; i++)
        {
            m_colors[i] = agg::rgba8(
                (rand() & 0xFF), 
                (rand() & 0xFF), 
                (rand() & 0xFF), 
                230);

            m_colors[i].apply_gamma_dir(m_gamma);
            m_colors[i].premultiply();
        }
    }



    bool open(const char* fname)
    {
        return m_shape.open(full_file_name(fname));
    }

    void read_next()
    {
        m_shape.read_next();
        m_shape.scale(width(), height());
    }

    virtual void on_draw()
    {
        typedef agg::renderer_base<pixfmt> renderer_base;
        typedef agg::renderer_scanline_aa_solid<renderer_base> renderer_scanline;
        typedef agg::scanline_u8 scanline;

        pixfmt pixf(rbuf_window());
        renderer_base ren_base(pixf);
        ren_base.clear(agg::rgba(1.0, 1.0, 0.95));
        renderer_scanline ren(ren_base);

        unsigned i;
        unsigned w = unsigned(width());
        m_gradient.resize(w);
        agg::rgba8 c1(255, 0, 0, 180);
        agg::rgba8 c2(0, 0, 255, 180);
        for(i = 0; i < w; i++)
        {
            m_gradient[i] = c1.gradient(c2, i / width());
            m_gradient[i].premultiply();
        }

        agg::rasterizer_scanline_aa<agg::rasterizer_sl_clip_dbl> ras;
        agg::rasterizer_compound_aa<agg::rasterizer_sl_clip_dbl> rasc;
        agg::scanline_u8 sl;
        agg::scanline_bin sl_bin;
        agg::conv_transform<agg::compound_shape> shape(m_shape, m_scale);
        agg::conv_stroke<agg::conv_transform<agg::compound_shape> > stroke(shape);

        agg::test_styles style_handler(m_colors, m_gradient.data());
        agg::span_allocator<agg::rgba8> alloc;

        m_shape.approximation_scale(m_scale.scale());

        // Fill shape
        //----------------------
        rasc.clip_box(0, 0, width(), height());
        rasc.reset();
        //rasc.filling_rule(agg::fill_even_odd);
        start_timer();
        for(i = 0; i < m_shape.paths(); i++)
        {
            
            if(m_shape.style(i).left_fill >= 0 || 
               m_shape.style(i).right_fill >= 0)
            {
                rasc.styles(m_shape.style(i).left_fill, 
                            m_shape.style(i).right_fill);
                rasc.add_path(shape, m_shape.style(i).path_id);
            }
        }
        agg::render_scanlines_compound(rasc, sl, sl_bin, ren_base, alloc, style_handler);
        double tfill = elapsed_time();

        // Hit-test test
        bool draw_strokes = true;
        if(m_hit_x >= 0 && m_hit_y >= 0)
        {
            if(rasc.hit_test(m_hit_x, m_hit_y))
            {
                draw_strokes = false;
            }
        }

        // Draw strokes
        //----------------------
        start_timer();
        if(draw_strokes)
        {
            ras.clip_box(0, 0, width(), height());
            stroke.width(sqrt(m_scale.scale()));
            stroke.line_join(agg::round_join);
            stroke.line_cap(agg::round_cap);
            for(i = 0; i < m_shape.paths(); i++)
            {
                ras.reset();
                if(m_shape.style(i).line >= 0)
                {
                    ras.add_path(stroke, m_shape.style(i).path_id);
                    ren.color(agg::rgba8(0,0,0, 128));
                    agg::render_scanlines(ras, sl, ren);
                }
            }
        }
        double tstroke = elapsed_time();


        char buf[256]; 
        agg::gsv_text t;
        t.size(8.0);
        t.flip(true);

        agg::conv_stroke<agg::gsv_text> ts(t);
        ts.width(1.6);
        ts.line_cap(agg::round_cap);

        sprintf(buf, "Fill=%.2fms (%dFPS) Stroke=%.2fms (%dFPS) Total=%.2fms (%dFPS)\n\n"
                     "Space: Next Shape\n\n"
                     "+/- : ZoomIn/ZoomOut (with respect to the mouse pointer)",
                     tfill, int(1000.0 / tfill),
                     tstroke, int(1000.0 / tstroke),
                     tfill+tstroke, int(1000.0 / (tfill+tstroke)));

        t.start_point(10.0, 20.0);
        t.text(buf);

        ras.add_path(ts);
        ren.color(agg::rgba(0,0,0));
        agg::render_scanlines(ras, sl, ren);

        if(m_gamma.gamma() != 1.0)
        {
            pixf.apply_gamma_inv(m_gamma);
        }
    }

    
    virtual void on_key(int x, int y, unsigned key, unsigned flags)
    {
        if(key == ' ')
        {
            m_shape.read_next();
            m_shape.scale(width(), height());
            force_redraw();
        }

        if(key == '+' || key == agg::key_kp_plus)
        {
            m_scale *= agg::trans_affine_translation(-x, -y);
            m_scale *= agg::trans_affine_scaling(1.1);
            m_scale *= agg::trans_affine_translation(x, y);
            force_redraw();
        }

        if(key == '-' || key == agg::key_kp_minus)
        {
            m_scale *= agg::trans_affine_translation(-x, -y);
            m_scale *= agg::trans_affine_scaling(1/1.1);
            m_scale *= agg::trans_affine_translation(x, y);
            force_redraw();
        }

        if(key == agg::key_left)
        {
            m_scale *= agg::trans_affine_translation(-x, -y);
            m_scale *= agg::trans_affine_rotation(-agg::pi / 20.0);
            m_scale *= agg::trans_affine_translation(x, y);
            force_redraw();
        }

        if(key == agg::key_right)
        {
            m_scale *= agg::trans_affine_translation(-x, -y);
            m_scale *= agg::trans_affine_rotation(agg::pi / 20.0);
            m_scale *= agg::trans_affine_translation(x, y);
            force_redraw();
        }
    }

    void on_mouse_move(int x, int y, unsigned flags) 
    {
        if((flags & 3) == 0)
        {
            on_mouse_button_up(x, y, flags);
        }
        else
        {
            if(m_point_idx >= 0)
            {
                double xd = x;
                double yd = y;
                m_scale.inverse_transform(&xd, &yd);
                m_shape.modify_vertex(m_point_idx, xd, yd);
                force_redraw();
            }
        }
    }

    void on_mouse_button_down(int x, int y, unsigned flags) 
    {
        if(flags & 1)
        {
            double xd = x;
            double yd = y;
            double r = 4.0 / m_scale.scale();
            m_scale.inverse_transform(&xd, &yd);
            m_point_idx = m_shape.hit_test(xd, yd, r);
            force_redraw();
        }
        if(flags & 2)
        {
            m_hit_x = x;
            m_hit_y = y;
            force_redraw();
        }
    }

    void on_mouse_button_up(int x, int y, unsigned flags) 
    {
        m_point_idx = -1;
        m_hit_x = -1;
        m_hit_y = -1;
        force_redraw();
    }


};



int agg_main(int argc, char* argv[])
{
    the_application app(agg::pix_format_bgra32, flip_y);
    app.caption("AGG Example - Flash Rasterizer");
    const char* fname = "shapes.txt";
    if(argc > 1) fname = argv[1];
    if(!app.open(fname))
    {
        char buf[256];
        if(strcmp(fname, "shapes.txt") == 0)
        {
            sprintf(buf, "File not found: %s. Download http://www.antigrain.com/%s\n"
                         "or copy it from another directory if available.",
                    fname, fname);
        }
        else
        {
            sprintf(buf, "File not found: %s", fname);
        }
        app.message(buf);
        return 1;
    }

    if(app.init(655, 520, agg::window_resize))
    {
        app.read_next();
        return app.run();
    }
    return 1;
}





