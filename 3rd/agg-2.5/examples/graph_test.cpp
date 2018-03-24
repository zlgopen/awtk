#include <math.h> 
#include <stdio.h>
#include "agg_basics.h"
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_rasterizer_outline.h"
#include "agg_conv_stroke.h"
#include "agg_conv_dash.h"
#include "agg_conv_curve.h"
#include "agg_conv_contour.h"
#include "agg_conv_marker.h"
#include "agg_conv_shorten_path.h"
#include "agg_conv_marker_adaptor.h"
#include "agg_conv_concat.h"
#include "agg_arrowhead.h"
#include "agg_vcgen_markers_term.h"
#include "agg_scanline_p.h"
#include "agg_scanline_u.h"
#include "agg_renderer_scanline.h"
#include "agg_renderer_primitives.h"
#include "agg_span_allocator.h"
#include "agg_span_gradient.h"
#include "agg_span_interpolator_linear.h"
#include "agg_pixfmt_rgb.h"
#include "ctrl/agg_slider_ctrl.h"
#include "ctrl/agg_rbox_ctrl.h"
#include "ctrl/agg_cbox_ctrl.h"
#include "platform/agg_platform_support.h"

enum flip_y_e { flip_y = true };


typedef agg::pixfmt_bgr24 pixfmt;
typedef pixfmt::color_type color_type;
typedef agg::renderer_base<pixfmt> base_renderer;
typedef agg::renderer_primitives<base_renderer> primitives_renderer;

typedef agg::renderer_scanline_aa_solid<base_renderer>  solid_renderer;
typedef agg::renderer_scanline_bin_solid<base_renderer> draft_renderer;

typedef agg::gradient_radial_d gradient_function;
typedef agg::span_interpolator_linear<> interpolator;
typedef agg::pod_auto_array<color_type, 256> color_array_type;
typedef agg::span_gradient<color_type, 
                           interpolator, 
                           gradient_function, 
                           color_array_type> gradient_span_gen;
typedef agg::span_allocator<color_type> gradient_span_alloc;

typedef agg::renderer_scanline_aa<base_renderer, 
                                  gradient_span_alloc,
                                  gradient_span_gen> gradient_renderer;

typedef agg::rasterizer_scanline_aa<> scanline_rasterizer;
typedef agg::rasterizer_outline<primitives_renderer> outline_rasterizer;



//============================================================================
class graph
{
public:
    struct node
    {
        double x, y;
        node() {}
        node(double x_, double y_) : x(x_), y(y_) {}
    };

    struct edge
    {
        int node1;
        int node2;
        edge() {}
        edge(int n1, int n2) : node1(n1), node2(n2) {}
    };

	~graph()
    {
        delete [] m_edges;
        delete [] m_nodes;
    }

    graph(int num_nodes, int num_edges) :
        m_num_nodes(num_nodes),
        m_num_edges(num_edges),
        m_nodes(new node[num_nodes]),
        m_edges(new edge[num_edges])
    {
        int i;

	    srand(100);

	    for(i = 0; i < m_num_nodes; i++)
        {
            m_nodes[i].x = (double(rand()) / RAND_MAX) * 0.75 + 0.2;
            m_nodes[i].y = (double(rand()) / RAND_MAX) * 0.85 + 0.1;
        }

	    for(i = 0; i < m_num_edges; i++)
        {
            m_edges[i].node1 = rand() % m_num_nodes;
            m_edges[i].node2 = rand() % m_num_nodes;
            if(m_edges[i].node1 == m_edges[i].node2) i--;
        }
    }

    int get_num_nodes() const { return m_num_nodes; } 
    int get_num_edges() const { return m_num_edges; } 

	node get_node(int idx, double w, double h) const
    {
        node p(0.0, 0.0);
        if(idx < m_num_nodes)
        {
            p = m_nodes[idx];
            p.x = p.x * w;
            p.y = p.y * h;
        }
        return p;
    }

	edge get_edge(int idx) const
    {
        edge b(0,0);
        if(idx < m_num_edges)
        {
            b = m_edges[idx];
        }
        return b;
    }

private:
    graph(const graph&);
    const graph& operator = (const graph&);

    int m_num_nodes;
    int m_num_edges;
    node* m_nodes;
    edge* m_edges;
};








    
//============================================================================
struct line
{
    double x1, y1, x2, y2;
    int f;

    line(double x1_, double y1_, double x2_, double y2_) : 
        x1(x1_), y1(y1_), x2(x2_), y2(y2_), f(0) {}

    void rewind(unsigned) { f = 0; }
    unsigned vertex(double* x, double* y)
    {
        if(f == 0) { ++f; *x = x1; *y = y1; return agg::path_cmd_move_to; }
        if(f == 1) { ++f; *x = x2; *y = y2; return agg::path_cmd_line_to; }
        return agg::path_cmd_stop;
    }
};




//============================================================================
struct curve
{
    agg::curve4 c;

    curve(double x1, double y1, double x2, double y2, double k=0.5)
    {
        c.init(x1, y1, 
               x1 - (y2 - y1) * k,
               y1 + (x2 - x1) * k,
               x2 + (y2 - y1) * k,
               y2 - (x2 - x1) * k,
               x2, y2);
    }

    void rewind(unsigned path_id) { c.rewind(path_id); }
    unsigned vertex(double* x, double* y) { return c.vertex(x, y); }
};



//============================================================================
template<class Source> struct stroke_draft_simple
{
    Source& s;
    stroke_draft_simple(Source& src, double w) : 
        s(src)
    {
    }

    void rewind(unsigned path_id) { s.rewind(path_id); }
    unsigned vertex(double* x, double* y) { return s.vertex(x, y); }
};


//============================================================================
template<class Source> struct stroke_draft_arrow
{
    typedef agg::conv_marker_adaptor<Source, agg::vcgen_markers_term>           stroke_type;
    typedef agg::conv_marker<typename stroke_type::marker_type, agg::arrowhead> marker_type;
    typedef agg::conv_concat<stroke_type, marker_type>                          concat_type;

    stroke_type    s;
    agg::arrowhead ah;
    marker_type    m;
    concat_type    c;

    stroke_draft_arrow(Source& src, double w) : 
        s(src),
        ah(),
        m(s.markers(), ah),
        c(s, m)
    {
        ah.head(0, 10, 5, 0);
        s.shorten(10.0);
    }

    void rewind(unsigned path_id) { c.rewind(path_id); }
    unsigned vertex(double* x, double* y) { return c.vertex(x, y); }
};



//============================================================================
template<class Source> struct stroke_fine_simple
{
    typedef agg::conv_stroke<Source> stroke_type;

    stroke_type    s;

    stroke_fine_simple(Source& src, double w) : 
        s(src)
    {
        s.width(w); 
    }
    void rewind(unsigned path_id) { s.rewind(path_id); }
    unsigned vertex(double* x, double* y) { return s.vertex(x, y); }
};



//============================================================================
template<class Source> struct stroke_fine_arrow
{
    typedef agg::conv_stroke<Source, agg::vcgen_markers_term>                   stroke_type;
    typedef agg::conv_marker<typename stroke_type::marker_type, agg::arrowhead> marker_type;
    typedef agg::conv_concat<stroke_type, marker_type>                          concat_type;

    stroke_type    s;
    agg::arrowhead ah;
    marker_type    m;
    concat_type    c;

    stroke_fine_arrow(Source& src, double w) : 
        s(src),
        ah(),
        m(s.markers(), ah),
        c(s, m)
    {
        s.width(w); 
        ah.head(0, 10, 5, 0);
        s.shorten(w * 2.0);
    }

    void rewind(unsigned path_id) { c.rewind(path_id); }
    unsigned vertex(double* x, double* y) { return c.vertex(x, y); }
};



//============================================================================
template<class Source> struct dash_stroke_draft_simple
{
    typedef agg::conv_dash<Source, agg::vcgen_markers_term> dash_type;

    dash_type d;
    
    dash_stroke_draft_simple(Source& src, 
                             double dash_len, 
                             double gap_len, 
                             double w) : 
        d(src)
    {
        d.add_dash(dash_len, gap_len);
    }

    void rewind(unsigned path_id) { d.rewind(path_id); }
    unsigned vertex(double* x, double* y) { return d.vertex(x, y); }
};


//============================================================================
template<class Source> struct dash_stroke_draft_arrow
{
    typedef agg::conv_dash<Source, agg::vcgen_markers_term>                   dash_type;
    typedef agg::conv_marker<typename dash_type::marker_type, agg::arrowhead> marker_type;
    typedef agg::conv_concat<dash_type, marker_type>                          concat_type;

    dash_type      d;
    agg::arrowhead ah;
    marker_type    m;
    concat_type    c;

    dash_stroke_draft_arrow(Source& src, 
                            double dash_len, double gap_len, double w) : 
        d(src),
        ah(),
        m(d.markers(), ah),
        c(d, m)
    {
        d.add_dash(dash_len, gap_len);
        ah.head(0, 10, 5, 0);
        d.shorten(10.0);
    }

    void rewind(unsigned path_id) { c.rewind(path_id); }
    unsigned vertex(double* x, double* y) { return c.vertex(x, y); }
};




//============================================================================
template<class Source> struct dash_stroke_fine_simple
{
    typedef agg::conv_dash<Source>       dash_type;
    typedef agg::conv_stroke<dash_type>  stroke_type;

    dash_type      d;
    stroke_type    s;

    dash_stroke_fine_simple(Source& src, 
                           double dash_len, double gap_len, double w) : 
        d(src),
        s(d)
    {
        d.add_dash(dash_len, gap_len);
        s.width(w);
    }

    void rewind(unsigned path_id) { s.rewind(path_id); }
    unsigned vertex(double* x, double* y) { return s.vertex(x, y); }
};






//============================================================================
template<class Source> struct dash_stroke_fine_arrow
{
    typedef agg::conv_dash<Source, agg::vcgen_markers_term>                   dash_type;
    typedef agg::conv_stroke<dash_type>                                       stroke_type;
    typedef agg::conv_marker<typename dash_type::marker_type, agg::arrowhead> marker_type;
    typedef agg::conv_concat<stroke_type, marker_type>                        concat_type;

    dash_type      d;
    stroke_type    s;
    agg::arrowhead ah;
    marker_type    m;
    concat_type    c;

    dash_stroke_fine_arrow(Source& src, 
                           double dash_len, double gap_len, double w) : 
        d(src),
        s(d),
        ah(),
        m(d.markers(), ah),
        c(s, m)
    {
        d.add_dash(dash_len, gap_len);
        s.width(w);
        ah.head(0, 10, 5, 0);
        d.shorten(w * 2.0);
    }

    void rewind(unsigned path_id) { c.rewind(path_id); }
    unsigned vertex(double* x, double* y) { return c.vertex(x, y); }
};



//#define stroke_draft      stroke_draft_simple
//#define dash_stroke_draft dash_stroke_draft_simple
//#define stroke_fine       stroke_fine_simple
//#define dash_stroke_fine  dash_stroke_fine_simple

#define stroke_draft      stroke_draft_arrow
#define dash_stroke_draft dash_stroke_draft_arrow
#define stroke_fine       stroke_fine_arrow
#define dash_stroke_fine  dash_stroke_fine_arrow





class the_application : public agg::platform_support
{
    agg::rbox_ctrl<agg::rgba>   m_type;
    agg::slider_ctrl<agg::rgba> m_width;
    agg::cbox_ctrl<agg::rgba>   m_benchmark;
    agg::cbox_ctrl<agg::rgba>   m_draw_nodes;
    agg::cbox_ctrl<agg::rgba>   m_draw_edges;
    agg::cbox_ctrl<agg::rgba>   m_draft;
    agg::cbox_ctrl<agg::rgba>   m_translucent;
    graph                       m_graph;
    color_array_type            m_gradient_colors;
    int                         m_draw;
    agg::scanline_u8            m_sl;


public:
    the_application(agg::pix_format_e format, bool flip_y) :
        agg::platform_support(format, flip_y),
        m_type(-1, -1, -1, -1, !flip_y),
        m_width(110+80, 8.0, 110+200.0+80, 8.0 + 7.0, !flip_y),
        m_benchmark(110+200+80+8, 8.0-2.0, "Benchmark", !flip_y),
        m_draw_nodes(110+200+80+8, 8.0-2.0+15.0, "Draw Nodes", !flip_y),
        m_draw_edges(200+200+80+8, 8.0-2.0+15.0, "Draw Edges", !flip_y),
        m_draft(200+200+80+8, 8.0-2.0, "Draft Mode", !flip_y),
        m_translucent(110+80, 8.0-2.0+15.0, "Translucent Mode", !flip_y),
        m_graph(200, 100),
        m_gradient_colors(),
        m_draw(3)
    {
        add_ctrl(m_type);
        m_type.text_size(8.0);
        m_type.add_item("Solid lines");
        m_type.add_item("Bezier curves");
        m_type.add_item("Dashed curves");
        m_type.add_item("Poygons AA");
        m_type.add_item("Poygons Bin");
        m_type.cur_item(0);

        add_ctrl(m_width);
        m_width.num_steps(20);
        m_width.range(0.0, 5.0);
        m_width.value(2.0);
        m_width.label("Width=%1.2f");

        m_benchmark.text_size(8.0);
        m_draw_nodes.text_size(8.0);
        m_draft.text_size(8.0);
        m_draw_nodes.status(true);
        m_draw_edges.status(true);
        add_ctrl(m_benchmark);
        add_ctrl(m_draw_nodes);
        add_ctrl(m_draw_edges);
        add_ctrl(m_draft);
        add_ctrl(m_translucent);

        agg::rgba c1(1, 1, 0, 0.25);
        agg::rgba c2(0, 0, 1);

        int i;
        for(i = 0; i < 256; i++)
        {
            m_gradient_colors[i] = c1.gradient(c2, double(i) / 255.0);
        }
    }






    //------------------------------------------------------------------------
    void draw_nodes_draft()
    {
        pixfmt pixf(rbuf_window());
        base_renderer rb(pixf);
        primitives_renderer prim(rb);
        int i;
        for(i = 0; i < m_graph.get_num_nodes(); i++)
        {
            graph::node n = m_graph.get_node(i, width(), height());
            prim.fill_color(m_gradient_colors[147]);
            prim.line_color(m_gradient_colors[255]);
            prim.outlined_ellipse(int(n.x), int(n.y), 10, 10);
            prim.fill_color(m_gradient_colors[50]);
            prim.solid_ellipse(int(n.x), int(n.y), 4, 4);
        }
    }



    //------------------------------------------------------------------------
    void draw_nodes_fine(scanline_rasterizer& ras)
    {
        gradient_span_alloc sa;
        pixfmt pixf(rbuf_window());
        base_renderer rb(pixf);
        int i;
        for(i = 0; i < m_graph.get_num_nodes(); i++)
        {
            graph::node n = m_graph.get_node(i, width(), height());
            agg::ellipse ell(n.x, n.y, 5.0 * m_width.value(), 5.0 * m_width.value());

            double x, y;
            switch(m_draw)
            {
                case 0:
                    ell.rewind(0);
                    while(!agg::is_stop(ell.vertex(&x, &y)));
                    break;

                case 1:
                    ras.reset();
                    ras.add_path(ell);
                    break;

                case 2:
                    ras.reset();
                    ras.add_path(ell);
                    ras.sort();
                    break;

                case 3:
                {
                    gradient_function gf;
                    agg::trans_affine mtx;
                    mtx *= agg::trans_affine_scaling(m_width.value() / 2.0);
                    mtx *= agg::trans_affine_translation(n.x, n.y);
                    mtx.invert();
                    interpolator inter(mtx);
                    gradient_span_gen sg(inter, gf, m_gradient_colors, 0.0, 10.0);
                    gradient_renderer ren(rb, sa, sg);
                    ras.add_path(ell);
                    agg::render_scanlines(ras, m_sl, ren);
                }
                break;
            }
        }
    }





    //------------------------------------------------------------------------
    template<class Source>
    void render_edge_fine(scanline_rasterizer& ras, 
                          solid_renderer& ren_fine,
                          draft_renderer& ren_draft,
                          Source& src)
    {
        double x, y;
        switch(m_draw)
        {
            case 0:
                src.rewind(0);
                while(!agg::is_stop(src.vertex(&x, &y)));
            break;

            case 1:
                ras.reset();
                ras.add_path(src);
            break;

            case 2:
                ras.reset();
                ras.add_path(src);
                ras.sort();
            break;

            case 3:
            {
                int r = rand() & 0x7F;
                int g = rand() & 0x7F;
                int b = rand() & 0x7F;
                int a = 255;
                if(m_translucent.status()) a = 80;
                ras.add_path(src);

                if(m_type.cur_item() < 4)
                {
                    ren_fine.color(agg::rgba8(r, g, b, a));
                    agg::render_scanlines(ras, m_sl, ren_fine);
                }
                else
                {
                    ren_draft.color(agg::rgba8(r, g, b, a));
                    agg::render_scanlines(ras, m_sl, ren_draft);
                }
            }
            break;
        }
    }



    //------------------------------------------------------------------------
    void draw_lines_draft()
    {
        pixfmt pixf(rbuf_window());
        base_renderer rb(pixf);
        primitives_renderer prim(rb);
        outline_rasterizer ras(prim);

        int i;
        for(i = 0; i < m_graph.get_num_edges(); i++)
        {
            graph::edge e  = m_graph.get_edge(i);
            graph::node n1 = m_graph.get_node(e.node1, width(), height());
            graph::node n2 = m_graph.get_node(e.node2, width(), height());
            line l(n1.x, n1.y, n2.x, n2.y);
            stroke_draft<line> s(l, m_width.value());

            int r = rand() & 0x7F;
            int g = rand() & 0x7F;
            int b = rand() & 0x7F;
            int a = 255;
            if(m_translucent.status()) a = 80;
            prim.line_color(agg::rgba8(r, g, b, a));
            ras.add_path(s);
        }
    }


    //------------------------------------------------------------------------
    void draw_curves_draft()
    {
        pixfmt pixf(rbuf_window());
        base_renderer rb(pixf);
        primitives_renderer prim(rb);
        outline_rasterizer ras(prim);

        int i;
        for(i = 0; i < m_graph.get_num_edges(); i++)
        {
            graph::edge e  = m_graph.get_edge(i);
            graph::node n1 = m_graph.get_node(e.node1, width(), height());
            graph::node n2 = m_graph.get_node(e.node2, width(), height());
            curve c(n1.x, n1.y, n2.x, n2.y);
            stroke_draft<curve> s(c, m_width.value());

            int r = rand() & 0x7F;
            int g = rand() & 0x7F;
            int b = rand() & 0x7F;
            int a = 255;
            if(m_translucent.status()) a = 80;
            prim.line_color(agg::rgba8(r, g, b, a));
            ras.add_path(s);
        }
    }


    //------------------------------------------------------------------------
    void draw_dashes_draft()
    {
        pixfmt pixf(rbuf_window());
        base_renderer rb(pixf);
        primitives_renderer prim(rb);
        outline_rasterizer ras(prim);

        int i;
        for(i = 0; i < m_graph.get_num_edges(); i++)
        {
            graph::edge e  = m_graph.get_edge(i);
            graph::node n1 = m_graph.get_node(e.node1, width(), height());
            graph::node n2 = m_graph.get_node(e.node2, width(), height());
            curve c(n1.x, n1.y, n2.x, n2.y);
            dash_stroke_draft<curve> s(c, 6.0, 3.0, m_width.value());

            int r = rand() & 0x7F;
            int g = rand() & 0x7F;
            int b = rand() & 0x7F;
            int a = 255;
            if(m_translucent.status()) a = 80;
            prim.line_color(agg::rgba8(r, g, b, a));
            ras.add_path(s);
        }
    }


    //------------------------------------------------------------------------
    void draw_lines_fine(scanline_rasterizer& ras, 
                         solid_renderer& solid,
                         draft_renderer& draft)
    {
        int i;
        for(i = 0; i < m_graph.get_num_edges(); i++)
        {
            graph::edge b  = m_graph.get_edge(i);
            graph::node n1 = m_graph.get_node(b.node1, width(), height());
            graph::node n2 = m_graph.get_node(b.node2, width(), height());
            line l(n1.x, n1.y, n2.x, n2.y);
            stroke_fine<line> s(l, m_width.value());
            render_edge_fine(ras, solid, draft, s);
        }
    }


    //------------------------------------------------------------------------
    void draw_curves_fine(scanline_rasterizer& ras, 
                          solid_renderer& solid,
                          draft_renderer& draft)

    {
        int i;
        for(i = 0; i < m_graph.get_num_edges(); i++)
        {
            graph::edge b  = m_graph.get_edge(i);
            graph::node n1 = m_graph.get_node(b.node1, width(), height());
            graph::node n2 = m_graph.get_node(b.node2, width(), height());
            curve c(n1.x, n1.y, n2.x, n2.y);
            stroke_fine<curve> s(c, m_width.value());
            render_edge_fine(ras, solid, draft, s);
        }
    }


    //------------------------------------------------------------------------
    void draw_dashes_fine(scanline_rasterizer& ras, 
                          solid_renderer& solid,
                          draft_renderer& draft)
    {
        int i;
        for(i = 0; i < m_graph.get_num_edges(); i++)
        {
            graph::edge b  = m_graph.get_edge(i);
            graph::node n1 = m_graph.get_node(b.node1, width(), height());
            graph::node n2 = m_graph.get_node(b.node2, width(), height());
            curve c(n1.x, n1.y, n2.x, n2.y);
            dash_stroke_fine<curve> s(c, 6.0, 3.0, m_width.value());
            render_edge_fine(ras, solid, draft, s);
        }
    }


    //------------------------------------------------------------------------
    void draw_polygons(scanline_rasterizer& ras, 
                       solid_renderer& solid,
                       draft_renderer& draft)
    {
        int i;
        if(m_type.cur_item() == 4)
        {
            ras.gamma(agg::gamma_threshold(0.5));
        }
        for(i = 0; i < m_graph.get_num_edges(); i++)
        {
            graph::edge b  = m_graph.get_edge(i);
            graph::node n1 = m_graph.get_node(b.node1, width(), height());
            graph::node n2 = m_graph.get_node(b.node2, width(), height());
            curve c(n1.x, n1.y, n2.x, n2.y);
            render_edge_fine(ras, solid, draft, c);
        }
        ras.gamma(agg::gamma_none());
    }



    //------------------------------------------------------------------------
    void draw_scene(scanline_rasterizer& ras, 
                    solid_renderer& solid,
                    draft_renderer& draft)
    {
        ras.gamma(agg::gamma_none());
        srand(100);
        if(m_draw_nodes.status())
        {
            if(m_draft.status())
            {
                draw_nodes_draft();
            }
            else
            {
                draw_nodes_fine(ras);
            }
        }

        if(m_draw_edges.status())
        {
            if(m_draft.status()) 
            {
                switch(m_type.cur_item())
                {
                case 0: draw_lines_draft();    break;
                case 1: draw_curves_draft();   break;
                case 2: draw_dashes_draft();   break;
                }
            }
            else
            {
                switch(m_type.cur_item())
                {
                case 0: draw_lines_fine(ras, solid, draft);  break;
                case 1: draw_curves_fine(ras, solid, draft); break;
                case 2: draw_dashes_fine(ras, solid, draft); break;
                case 3: 
                case 4: draw_polygons(ras, solid, draft);    break;
                }
            }
        }
    }












    //------------------------------------------------------------------------
    virtual void on_draw()
    {
        scanline_rasterizer ras;

        pixfmt pixf(rbuf_window());
        base_renderer rb(pixf);
        solid_renderer solid(rb);
        draft_renderer draft(rb);

        rb.clear(agg::rgba(1, 1, 1));
        draw_scene(ras, solid, draft);

        ras.filling_rule(agg::fill_non_zero);
        agg::render_ctrl(ras, m_sl, rb, m_type);
        agg::render_ctrl(ras, m_sl, rb, m_width);
        agg::render_ctrl(ras, m_sl, rb, m_benchmark);
        agg::render_ctrl(ras, m_sl, rb, m_draw_nodes);
        agg::render_ctrl(ras, m_sl, rb, m_draw_edges);
        agg::render_ctrl(ras, m_sl, rb, m_draft);
        agg::render_ctrl(ras, m_sl, rb, m_translucent);
    }




    virtual void on_ctrl_change()
    {
        if(m_benchmark.status())
        {
            int i;
            on_draw();
            update_window();

            scanline_rasterizer ras;

            pixfmt pixf(rbuf_window());
            base_renderer rb(pixf);
            solid_renderer solid(rb);
            draft_renderer draft(rb);

            char buf[256];
            if(m_draft.status())
            {
                start_timer();
                for(i = 0; i < 10; i++)
                {
                    draw_scene(ras, solid, draft);
                }
                sprintf(buf, "%3.3f milliseconds", elapsed_time());
            }
            else
            {
                double times[5];
                for(m_draw = 0; m_draw < 4; m_draw++)
                {
                    start_timer();
                    for(i = 0; i < 10; i++)
                    {
                        draw_scene(ras, solid, draft);
                    }
                    times[m_draw] = elapsed_time();
                }
                m_draw = 3;

                times[4]  = times[3];
                times[3] -= times[2];
                times[2] -= times[1];
                times[1] -= times[0];
            
                FILE* fd = fopen(full_file_name("benchmark"), "a");
                fprintf(fd, "%10.3f %10.3f %10.3f %10.3f %10.3f\n", 
                            times[0], times[1], times[2], times[3], times[4]);
                fclose(fd);
            

                sprintf(buf, "  pipeline  add_path         sort       render       total\n"
                             "%10.3f %10.3f %10.3f %10.3f %10.3f", 
                        times[0], times[1], times[2], times[3], times[4]);
            }
            message(buf);

            m_benchmark.status(false);
            force_redraw();
        }
    }
};



int agg_main(int argc, char* argv[])
{
    the_application app(agg::pix_format_bgr24, flip_y);
    app.caption("AGG Example. Line Join");

    if(app.init(600+100, 500+30, agg::window_resize))
    {
        return app.run();
    }
    return 1;
}


