#include <math.h>
#include <stdio.h>
#include <time.h>
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
#include "agg_rasterizer_outline_aa.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_span_allocator.h"
#include "agg_gamma_lut.h"
#include "agg_pixfmt_rgba.h"
#include "agg_bounding_rect.h"
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
            m_styles(),
            m_min_style(0x7FFFFFFF),
            m_max_style(-0x7FFFFFFF)
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
            m_min_style = 0x7FFFFFFF;
            m_max_style = -0x7FFFFFFF;

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
                        if(style.left_fill >= 0)
                        {
                            if(style.left_fill < m_min_style) m_min_style = style.left_fill;
                            if(style.left_fill > m_max_style) m_max_style = style.left_fill;
                        }
                        if(style.right_fill >= 0)
                        {
                            if(style.right_fill < m_min_style) m_min_style = style.right_fill;
                            if(style.right_fill > m_max_style) m_max_style = style.right_fill;
                        }
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

        int min_style() const { return m_min_style; }
        int max_style() const { return m_max_style; }

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
        int                                       m_min_style;
        int                                       m_max_style;

        FILE* m_fd;
    };

}






class the_application : public agg::platform_support
{

public:
    agg::compound_shape        m_shape;
    agg::rgba8                 m_colors[100];
    agg::trans_affine          m_scale;
    agg::gamma_lut<>           m_gamma;
    int                        m_point_idx;


    the_application(agg::pix_format_e format, bool flip_y) :
        agg::platform_support(format, flip_y),
        m_point_idx(-1)
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

        agg::rasterizer_scanline_aa<agg::rasterizer_sl_clip_dbl> ras;
        agg::scanline_u8 sl;
        agg::conv_transform<agg::compound_shape> shape(m_shape, m_scale);
        agg::conv_stroke<agg::conv_transform<agg::compound_shape> > stroke(shape);

        m_shape.approximation_scale(m_scale.scale());

        unsigned i;
        agg::path_storage tmp_path;

        ras.clip_box(0, 0, width(), height());

        // This is an alternative method of Flash rasterization. 
        // We decompose the compound shape into separate paths
        // and select the ones that fit the given style (left or right).
        // So that, we form a sub-shape and draw it as a whole.
        // 
        // Here the regular scanline rasterizer is used, but it doesn't
        // automatically close the polygons. So that, the rasterizer 
        // actually works with a set of polylines instead of polygons.
        // Of course, the data integrity must be preserved, that is, 
        // the polylines must eventually form a closed contour
        // (or a set of closed contours). So that, first we set 
        // auto_close(false);
        // 
        // The second important thing is that one path can be rasterized 
        // twice, if it has both, left and right fill. Sometimes the 
        // path has equal left and right fill, so that, the same path
        // will be added twice even for a single sub-shape. If the 
        // rasterizer can tolerate these degenerates you can add them, 
        // but it's also fine just to omit them.
        //
        // The third thing is that for one side (left or right)
        // you should invert the direction of the paths.
        //
        // The main disadvantage of this method is imperfect stitching
        // of the adjacent polygons. The problem can be solved if we use
        // compositing operation "plus" instead of alpha-blend. But
        // in this case we are forced to use an RGBA buffer, clean it with 
        // zero, rasterize using "plus" operation, and then alpha-blend
        // the result over the final scene. It can be too expensive.
        //------------------------------------------------------------
        ras.auto_close(false);
        //ras.filling_rule(agg::fill_even_odd);
        start_timer();
        for(int s = m_shape.min_style(); s <= m_shape.max_style(); s++)
        {
            ras.reset();
            for(i = 0; i < m_shape.paths(); i++)
            {
                const agg::path_style& style = m_shape.style(i);
                if(style.left_fill != style.right_fill)
                {
                    if(style.left_fill == s)
                    {
                        ras.add_path(shape, style.path_id);
                    }
                    if(style.right_fill == s)
                    {
                        tmp_path.remove_all();
                        tmp_path.concat_path(shape, style.path_id);
                        tmp_path.invert_polygon(0);
                        ras.add_path(tmp_path);
                    }
                }
            }
            agg::render_scanlines_aa_solid(ras, sl, ren_base, m_colors[s]);
        }
        double tfill = elapsed_time();
        ras.auto_close(true);

        // Draw strokes
        //----------------------
        start_timer();
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
        if((flags & 1) == 0)
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
    }

    void on_mouse_button_up(int x, int y, unsigned flags) 
    {
        m_point_idx = -1;
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





