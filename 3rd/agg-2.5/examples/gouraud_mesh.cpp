#include <math.h>
#include <stdio.h>
#include <time.h>
#include "agg_rendering_buffer.h"
#include "agg_conv_transform.h"
#include "agg_conv_stroke.h"
#include "agg_conv_clip_polyline.h"
#include "agg_scanline_u.h"
#include "agg_scanline_bin.h"
#include "agg_renderer_scanline.h"
#include "agg_rasterizer_outline_aa.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_span_allocator.h"
#include "agg_span_gouraud_rgba.h"
#include "agg_gamma_lut.h"
#include "agg_arc.h"
#include "agg_bezier_arc.h"
#include "agg_pixfmt_rgb.h"
#include "agg_pixfmt_rgba.h"
#include "agg_bounding_rect.h"
#include "agg_vpgen_clip_polygon.h"
#include "ctrl/agg_slider_ctrl.h"
#include "ctrl/agg_bezier_ctrl.h"
#include "ctrl/agg_rbox_ctrl.h"
#include "ctrl/agg_cbox_ctrl.h"
#include "platform/agg_platform_support.h"

#include "agg_rasterizer_compound_aa.h"


enum { flip_y = true };

typedef agg::pixfmt_bgra32_pre pixfmt;


namespace agg
{

    struct mesh_point
    {
        double x,y;
        double dx,dy;
        rgba8 color;
        rgba8 dc;

        mesh_point() {}
        mesh_point(double x_, double y_, 
                   double dx_, double dy_, 
                   rgba8 c, rgba8 dc_) : 
            x(x_), y(y_), 
            dx(dx_), dy(dy_), 
            color(c), dc(dc_)
        {}
    };

    struct mesh_triangle
    {
        unsigned p1, p2, p3;

        mesh_triangle() {}
        mesh_triangle(unsigned i, unsigned j, unsigned k) : 
            p1(i), p2(j), p3(k) 
        {}
    };

    struct mesh_edge
    {
        unsigned p1, p2;
        int      tl, tr;

        mesh_edge() {}
        mesh_edge(unsigned p1_, unsigned p2_, int tl_, int tr_) :
            p1(p1_), p2(p2_), tl(tl_), tr(tr_) 
        {}
    };


    static double random(double v1, double v2)
    {
        return (v2 - v1) * (rand() % 1000) / 999.0 + v1;
    }


    class mesh_ctrl
    {
    public:
        mesh_ctrl();

        void generate(unsigned cols, unsigned rows, 
                      double cell_w, double cell_h,
                      double start_x, double start_y);

        void randomize_points(double delta); 
        void rotate_colors();


        bool on_mouse_button_down(double x, double y, unsigned flags);
        bool on_mouse_move(double x, double y, unsigned flags);
        bool on_mouse_button_up(double x, double y, unsigned flags);

        unsigned num_vertices() const { return m_vertices.size(); }
        const mesh_point& vertex(unsigned i) const { return m_vertices[i]; }
              mesh_point& vertex(unsigned i)       { return m_vertices[i]; }

        const mesh_point& vertex(unsigned x, unsigned y) const { return m_vertices[y * m_rows + x]; }
              mesh_point& vertex(unsigned x, unsigned y)       { return m_vertices[y * m_rows + x]; }

        unsigned num_triangles() const { return m_triangles.size(); }
        const mesh_triangle& triangle(unsigned i) const { return m_triangles[i]; }
              mesh_triangle& triangle(unsigned i)       { return m_triangles[i]; }

        unsigned num_edges() const { return m_edges.size(); }
        const mesh_edge& edge(unsigned i) const { return m_edges[i]; }
              mesh_edge& edge(unsigned i)       { return m_edges[i]; }

    private:
        unsigned m_cols;
        unsigned m_rows;
        int      m_drag_idx;
        double   m_drag_dx;
        double   m_drag_dy;
        double   m_cell_w;
        double   m_cell_h;
        double   m_start_x;
        double   m_start_y;
        pod_bvector<mesh_point>    m_vertices;
        pod_bvector<mesh_triangle> m_triangles;
        pod_bvector<mesh_edge>     m_edges;
    };


    mesh_ctrl::mesh_ctrl() :
        m_cols(0),
        m_rows(0),
        m_drag_idx(-1),
        m_drag_dx(0),
        m_drag_dy(0)
    {}


    void mesh_ctrl::generate(unsigned cols, unsigned rows, 
                             double cell_w, double cell_h,
                             double start_x, double start_y)
    {
        m_cols = cols;
        m_rows = rows;
        m_cell_w = cell_w;
        m_cell_h = cell_h;
        m_start_x = start_x;
        m_start_y = start_y;

        m_vertices.remove_all();
        unsigned i, j;
        for(i = 0; i < m_rows; i++)
        {
            double x = start_x;
            for(j = 0; j < m_cols; j++)
            {
                double dx = random(-0.5, 0.5);
                double dy = random(-0.5, 0.5);
                rgba8 c(rand() & 0xFF, rand() & 0xFF, rand() & 0xFF);
                rgba8 dc(rand() & 1, rand() & 1, rand() & 1);
                m_vertices.add(mesh_point(x, start_y, dx, dy, c, dc));
                x += cell_w;
            }
            start_y += cell_h;
        }



        //  4---3
        //  |t2/|
        //  | / |
        //  |/t1|
        //  1---2
        m_triangles.remove_all();
        m_edges.remove_all();
        for(i = 0; i < m_rows - 1; i++)
        {
            for(j = 0; j < m_cols - 1; j++)
            {
                int p1 = i * m_cols + j;
                int p2 = p1 + 1;
                int p3 = p2 + m_cols;
                int p4 = p1 + m_cols;
                m_triangles.add(mesh_triangle(p1, p2, p3));
                m_triangles.add(mesh_triangle(p3, p4, p1));

                int curr_cell = i * (m_cols - 1) + j;
                int left_cell = j ? int(curr_cell - 1) : -1;
                int bott_cell = i ? int(curr_cell - (m_cols - 1)) : -1;

                int curr_t1 = curr_cell * 2;
                int curr_t2 = curr_t1 + 1;

                int left_t1 = (left_cell >= 0) ? left_cell * 2 : -1;
                int left_t2 = (left_cell >= 0) ? left_t1 + 1   : -1;

                int bott_t1 = (bott_cell >= 0) ? bott_cell * 2 : -1;
                int bott_t2 = (bott_cell >= 0) ? bott_t1 + 1   : -1;
                
                m_edges.add(mesh_edge(p1, p2, curr_t1, bott_t2));
                m_edges.add(mesh_edge(p1, p3, curr_t2, curr_t1));
                m_edges.add(mesh_edge(p1, p4, left_t1, curr_t2));

                if(j == m_cols - 2) // Last column
                {
                    m_edges.add(mesh_edge(p2, p3, curr_t1, -1));
                }

                if(i == m_rows - 2) // Last row
                {
                    m_edges.add(mesh_edge(p3, p4, curr_t2, -1));
                }
            }
        }
    }

    void mesh_ctrl::randomize_points(double delta)
    {
        unsigned i, j;
        for(i = 0; i < m_rows; i++)
        {
            for(j = 0; j < m_cols; j++)
            {
                double xc = j * m_cell_w + m_start_x;
                double yc = i * m_cell_h + m_start_y;
                double x1 = xc - m_cell_w / 4;
                double y1 = yc - m_cell_h / 4;
                double x2 = xc + m_cell_w / 4;
                double y2 = yc + m_cell_h / 4;
                mesh_point& p = vertex(j, i);
                p.x += p.dx;
                p.y += p.dy;
                if(p.x < x1) { p.x = x1; p.dx = -p.dx; }
                if(p.y < y1) { p.y = y1; p.dy = -p.dy; }
                if(p.x > x2) { p.x = x2; p.dx = -p.dx; }
                if(p.y > y2) { p.y = y2; p.dy = -p.dy; }
            }
        }
    }


    void mesh_ctrl::rotate_colors()
    {
        unsigned i;
        for(i = 1; i < m_vertices.size(); i++)
        {
            rgba8& c = m_vertices[i].color;
            rgba8& dc = m_vertices[i].dc;
            int r = c.r + (dc.r ? 5 : -5);
            int g = c.g + (dc.g ? 5 : -5);
            int b = c.b + (dc.b ? 5 : -5);
            if(r < 0) { r = 0; dc.r ^= 1; } if(r > 255) { r = 255; dc.r ^= 1; }
            if(g < 0) { g = 0; dc.g ^= 1; } if(g > 255) { g = 255; dc.g ^= 1; }
            if(b < 0) { b = 0; dc.b ^= 1; } if(b > 255) { b = 255; dc.b ^= 1; }
            c.r = r;
            c.g = g;
            c.b = b; 
        }
    }


    bool mesh_ctrl::on_mouse_button_down(double x, double y, unsigned flags)
    {
        if(flags & 1)
        {
            unsigned i;
            for(i = 0; i < m_vertices.size(); i++)
            {
                if(calc_distance(x, y, m_vertices[i].x, m_vertices[i].y) < 5)
                {
                    m_drag_idx = i;
                    m_drag_dx = x - m_vertices[i].x;
                    m_drag_dy = y - m_vertices[i].y;
                    return true;
                }
            }
        }
        return false;
    }

    bool mesh_ctrl::on_mouse_move(double x, double y, unsigned flags)
    {
        if(flags & 1)
        {
            if(m_drag_idx >= 0)
            {
                m_vertices[m_drag_idx].x = x - m_drag_dx;
                m_vertices[m_drag_idx].y = y - m_drag_dy;
                return true;
            }
        }
        else
        {
            return on_mouse_button_up(x, y, flags);
        }
        return false;
    }

    bool mesh_ctrl::on_mouse_button_up(double x, double y, unsigned flags)
    {
        bool ret = m_drag_idx >= 0;
        m_drag_idx = -1;
        return ret;
    }



    class styles_gouraud
    {
    public:
        typedef span_gouraud_rgba<rgba8> gouraud_type;

        template<class Gamma>
        styles_gouraud(const mesh_ctrl& mesh, const Gamma& gamma)
        {
            unsigned i;
            for(i = 0; i < mesh.num_triangles(); i++)
            {
                const agg::mesh_triangle& t = mesh.triangle(i);
                const agg::mesh_point& p1 = mesh.vertex(t.p1);
                const agg::mesh_point& p2 = mesh.vertex(t.p2);
                const agg::mesh_point& p3 = mesh.vertex(t.p3);

                agg::rgba8 c1 = p1.color; 
                agg::rgba8 c2 = p2.color; 
                agg::rgba8 c3 = p3.color;
                c1.apply_gamma_dir(gamma);
                c2.apply_gamma_dir(gamma);
                c3.apply_gamma_dir(gamma);
                gouraud_type gouraud(c1, c2, c3,
                                     p1.x, p1.y, 
                                     p2.x, p2.y,
                                     p3.x, p3.y);
                gouraud.prepare();
                m_triangles.add(gouraud);
            }
        }

        bool is_solid(unsigned style) const { return false; }

        rgba8 color(unsigned style) const { return rgba8(0,0,0,0); }

        void generate_span(rgba8* span, int x, int y, unsigned len, unsigned style)
        {
            m_triangles[style].generate(span, x, y, len);
        }

    private:
        pod_bvector<gouraud_type> m_triangles;
    };
}





class the_application : public agg::platform_support
{

public:
    typedef agg::renderer_base<pixfmt> renderer_base;
    typedef agg::renderer_scanline_aa_solid<renderer_base> renderer_scanline;
    typedef agg::rasterizer_scanline_aa<> rasterizer_scanline;
    typedef agg::scanline_u8 scanline;

    agg::mesh_ctrl      m_mesh;
    agg::gamma_lut<>    m_gamma;


    the_application(agg::pix_format_e format, bool flip_y) :
        agg::platform_support(format, flip_y)
    {
//        m_gamma.gamma(2.0);
    }

    virtual void on_init()
    {
        m_mesh.generate(20, 20, 17, 17, 40, 40);
    }


    virtual void on_draw()
    {
        pixfmt pf(rbuf_window());
        renderer_base ren_base(pf);
        ren_base.clear(agg::rgba(0, 0, 0));
        renderer_scanline ren(ren_base);

        rasterizer_scanline ras;
        agg::scanline_u8 sl;
        agg::scanline_bin sl_bin;

        agg::rasterizer_compound_aa<> rasc;
        agg::span_allocator<agg::rgba8> alloc;

        unsigned i;
        agg::styles_gouraud styles(m_mesh, m_gamma);
        start_timer();
        rasc.reset();
        //rasc.clip_box(40, 40, width() - 40, height() - 40);
        for(i = 0; i < m_mesh.num_edges(); i++)
        {
            const agg::mesh_edge& e = m_mesh.edge(i);
            const agg::mesh_point& p1 = m_mesh.vertex(e.p1);
            const agg::mesh_point& p2 = m_mesh.vertex(e.p2);
            rasc.styles(e.tl, e.tr);
            rasc.move_to_d(p1.x, p1.y);
            rasc.line_to_d(p2.x, p2.y);
        }
        agg::render_scanlines_compound(rasc, sl, sl_bin, ren_base, alloc, styles);
        double tm = elapsed_time();

        char buf[256]; 
        agg::gsv_text t;
        t.size(10.0);

        agg::conv_stroke<agg::gsv_text> pt(t);
        pt.width(1.5);
        pt.line_cap(agg::round_cap);
        pt.line_join(agg::round_join);

        sprintf(buf, "%3.2f ms, %d triangles, %.0f tri/sec", 
            tm, 
            m_mesh.num_triangles(),
            m_mesh.num_triangles() / tm * 1000.0);
        t.start_point(10.0, 10.0);
        t.text(buf);

        ras.add_path(pt);
        agg::render_scanlines_aa_solid(ras, sl, ren_base, agg::rgba(1,1,1));


        if(m_gamma.gamma() != 1.0)
        {
            pf.apply_gamma_inv(m_gamma);
        }
    }

    virtual void on_mouse_move(int x, int y, unsigned flags) 
    {
        if(m_mesh.on_mouse_move(x, y, flags))
        {
            force_redraw();
        }
    }

    virtual void on_mouse_button_down(int x, int y, unsigned flags) 
    {
        if(m_mesh.on_mouse_button_down(x, y, flags))
        {
            force_redraw();
        }
    }

    virtual void on_mouse_button_up(int x, int y, unsigned flags) 
    {
        if(m_mesh.on_mouse_button_up(x, y, flags))
        {
            force_redraw();
        }
    }
    
    virtual void on_key(int x, int y, unsigned key, unsigned flags)
    {
    }

    void on_idle() 
    {
        m_mesh.randomize_points(1.0);
        m_mesh.rotate_colors();
        force_redraw();
    }

    virtual void on_ctrl_change()
    {
    }
};


int agg_main(int argc, char* argv[])
{
    the_application app(agg::pix_format_bgra32, flip_y);
    app.caption("AGG Example");

    if(app.init(400, 400, 0))//agg::window_resize))
    {
        app.wait_mode(false);
        return app.run();
    }
    return 1;
}





