#ifndef INTERACTIVE_POLYGON_INCLUDED
#define INTERACTIVE_POLYGON_INCLUDED

#include "agg_array.h"
#include "agg_conv_stroke.h"
#include "agg_ellipse.h"

namespace agg
{
    class simple_polygon_vertex_source
    {
    public:
        simple_polygon_vertex_source(const double* polygon, unsigned np, 
                                     bool roundoff = false,
                                     bool close = true) :
            m_polygon(polygon),
            m_num_points(np),
            m_vertex(0),
            m_roundoff(roundoff),
            m_close(close)
        {
        }

        void close(bool f) { m_close = f;    }
        bool close() const { return m_close; }

        void rewind(unsigned)
        {
            m_vertex = 0;
        }

        unsigned vertex(double* x, double* y)
        {
            if(m_vertex > m_num_points) return path_cmd_stop;
            if(m_vertex == m_num_points) 
            {
                ++m_vertex;
                return path_cmd_end_poly | (m_close ? path_flags_close : 0);
            }
            *x = m_polygon[m_vertex * 2];
            *y = m_polygon[m_vertex * 2 + 1];
            if(m_roundoff)
            {
                *x = floor(*x) + 0.5;
                *y = floor(*y) + 0.5;
            }
            ++m_vertex;
            return (m_vertex == 1) ? path_cmd_move_to : path_cmd_line_to;
        }

    private:
        const double* m_polygon;
        unsigned m_num_points;
        unsigned m_vertex;
        bool     m_roundoff;
        bool     m_close;
    };




    class interactive_polygon
    {
    public:
        interactive_polygon(unsigned np, double point_radius);

        unsigned num_points() const { return m_num_points; }
        double xn(unsigned n) const { return m_polygon[n * 2];     }
        double yn(unsigned n) const { return m_polygon[n * 2 + 1]; }
        double& xn(unsigned n) { return m_polygon[n * 2];     }
        double& yn(unsigned n) { return m_polygon[n * 2 + 1]; }
    
        const double* polygon() const { return &m_polygon[0]; }

        int  node() const { return m_node; }
        void node(int n) { m_node = n; }

        void close(bool f) { m_vs.close(f);       }
        bool close() const { return m_vs.close(); }

        void rewind(unsigned);
        unsigned vertex(double* x, double* y);

        bool on_mouse_button_down(double x, double y);
        bool on_mouse_move(double x, double y);
        bool on_mouse_button_up(double x, double y);


    private:
        bool check_edge(unsigned i, double x, double y) const;
        bool point_in_polygon(double x, double y) const;


        pod_array<double> m_polygon;
        unsigned          m_num_points;
        int               m_node;
        int               m_edge;
        simple_polygon_vertex_source m_vs;
        conv_stroke<simple_polygon_vertex_source> m_stroke;
        ellipse  m_ellipse;
        double   m_point_radius;
        unsigned m_status;
        double   m_dx;
        double   m_dy;
    };


}

#endif

