#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_p.h"
#include "agg_renderer_scanline.h"
#include "agg_conv_transform.h"
#include "agg_conv_stroke.h"
#include "agg_bspline.h"
#include "agg_ellipse.h"
#include "agg_gsv_text.h"
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
enum default_num_points_e { default_num_points = 20000 };

enum start_size_e
{
    start_width  = 400,
    start_height = 400
};


enum atom_color_e
{
    atom_color_general = 0,
    atom_color_N       = 1,
    atom_color_O       = 2,
    atom_color_S       = 3,
    atom_color_P       = 4,
    atom_color_halogen = 5,
    end_of_atom_colors
};



struct atom_type
{
    double   x;
    double   y;
    char     label[4];
    int      charge;
    unsigned color_idx;
};

struct bond_type
{
    unsigned idx1;
    unsigned idx2;
    double   x1;
    double   y1;
    double   x2;
    double   y2;
    unsigned order;
    int      stereo;
    int      topology;
};


class molecule
{
public:
    ~molecule();
    molecule();

    bool read(FILE* fd);

    unsigned num_atoms() const { return m_num_atoms; }
    unsigned num_bonds() const { return m_num_bonds; }

    const atom_type& atom(unsigned idx) const { return m_atoms[idx]; }
    const bond_type& bond(unsigned idx) const { return m_bonds[idx]; }

    double average_bond_len() const { return m_avr_len; }

    const char* name() const { return m_name; }

    static int    get_int(const char* buf, int pos, int len);
    static double get_dbl(const char* buf, int pos, int len);
    static char*  get_str(char* dst, const char* buf, int pos, int len);

private:
    atom_type* m_atoms;
    unsigned   m_num_atoms;
    bond_type* m_bonds;
    unsigned   m_num_bonds;
    char       m_name[128];
    double     m_avr_len;
};




molecule::~molecule()
{
    delete [] m_bonds;
    delete [] m_atoms;
}




molecule::molecule() :
    m_atoms(0),
    m_num_atoms(0),
    m_bonds(0),
    m_num_bonds(0),
    m_avr_len(0)
{
    m_name[0] = 0;
}


int molecule::get_int(const char* buf, int pos, int len)
{
    char tmp[32];
    return atoi(get_str(tmp, buf, pos, len));
}

double molecule::get_dbl(const char* buf, int pos, int len)
{
    char tmp[32];
    return atof(get_str(tmp, buf, pos, len));
}

char* molecule::get_str(char* dst, const char* buf, int pos, int len)
{
    --pos;
    int buf_len = strlen(buf);
    buf += pos;
    while(pos + len < buf_len && isspace(*buf)) 
    {
        ++buf;
        --len;
    }
    if(len >= 0)
    {
        if(len > 0) memcpy(dst, buf, len);
        dst[len] = 0;
    }
    
    char* ts = dst;
    while(*ts && !isspace(*ts)) ts++;
    *ts = 0;
    return dst;
}


unsigned trim_cr_lf(char* buf)
{
    unsigned len = strlen(buf);

    // Trim "\n\r" at the beginning 
    unsigned pos = 0;
    while(len && (buf[0] == '\n' || buf[0] == '\r'))
    {
        --len;
        ++pos;
    }
    if(pos) strcpy(buf, buf + pos);

    // Trim "\n\r" at the end 
    while(len && (buf[len-1] == '\n' || buf[len-1] == '\r')) --len;
    buf[len] = 0;
    return len;
}


/*
MFCD00191150
  Mt7.00  09020210442D

 23 23  0  0  1  0  0  0  0  0999 V2000
   -2.6793   -0.2552    0.0000 C   0  0  0  0  0  0  0  0  0  0  0  0
   . . .
  1  2  1  0  0  0  0
   . . .
M  END
> <MDLNUMBER>
MFCD00191150

$$$$
*/
bool molecule::read(FILE* fd)
{
    char buf[512];
    unsigned len;
    if(!fgets(buf, 510, fd)) return false;
    len = trim_cr_lf(buf);
    if(len > 128) len = 128;

    if(len) memcpy(m_name, buf, len);
    m_name[len] = 0;

    if(!fgets(buf, 510, fd)) return false;
    if(!fgets(buf, 510, fd)) return false;
    if(!fgets(buf, 510, fd)) return false;
    trim_cr_lf(buf);
    m_num_atoms = get_int(buf, 1, 3);
    m_num_bonds = get_int(buf, 4, 3);
    if(m_num_atoms == 0 || m_num_bonds == 0) return false;
    m_atoms = new atom_type[m_num_atoms];
    m_bonds = new bond_type[m_num_bonds];
    memset(m_atoms, 0, sizeof(atom_type) * m_num_atoms);
    memset(m_bonds, 0, sizeof(bond_type) * m_num_bonds);

    unsigned i;
    for(i = 0; i < m_num_atoms; i++)
    {
        if(!fgets(buf, 510, fd)) return false;
        trim_cr_lf(buf);
/*
   -2.6793   -0.2552    0.0000 C   0  0  0  0  0  0  0  0  0  0  0  0
*/
        m_atoms[i].x = get_dbl(buf, 1,  10);
        m_atoms[i].y = get_dbl(buf, 11, 10);
        get_str(m_atoms[i].label, buf, 32, 3);
        m_atoms[i].charge = get_int(buf, 39, 1);
        if(m_atoms[i].charge) m_atoms[i].charge = 4 - m_atoms[i].charge;

        if(strcmp(m_atoms[i].label, "N") == 0) m_atoms[i].color_idx = atom_color_N;
        if(strcmp(m_atoms[i].label, "O") == 0) m_atoms[i].color_idx = atom_color_O;
        if(strcmp(m_atoms[i].label, "S") == 0) m_atoms[i].color_idx = atom_color_S;
        if(strcmp(m_atoms[i].label, "P") == 0) m_atoms[i].color_idx = atom_color_P;
        if(strcmp(m_atoms[i].label, "F") == 0 ||
           strcmp(m_atoms[i].label, "Cl") == 0 || 
           strcmp(m_atoms[i].label, "Br") == 0 || 
           strcmp(m_atoms[i].label, "I")  == 0) m_atoms[i].color_idx = atom_color_halogen;
    }

    m_avr_len = 0.0;
    for(i = 0; i < m_num_bonds; i++)
    {
        if(!fgets(buf, 510, fd)) return false;
        trim_cr_lf(buf);
/*
  1  2  1  0  0  0  0
*/
        m_bonds[i].idx1 = get_int(buf, 1, 3) - 1;
        m_bonds[i].idx2 = get_int(buf, 4, 3) - 1;

        if(m_bonds[i].idx1 >= m_num_atoms || m_bonds[i].idx2 >= m_num_atoms) return false;

        m_bonds[i].x1 = m_atoms[m_bonds[i].idx1].x;
        m_bonds[i].y1 = m_atoms[m_bonds[i].idx1].y;
        m_bonds[i].x2 = m_atoms[m_bonds[i].idx2].x;
        m_bonds[i].y2 = m_atoms[m_bonds[i].idx2].y;
        m_bonds[i].order    = get_int(buf, 7,  3);
        m_bonds[i].stereo   = get_int(buf, 10, 3);
        m_bonds[i].topology = get_int(buf, 13, 3);

        m_avr_len += sqrt((m_bonds[i].x1 - m_bonds[i].x2) * (m_bonds[i].x1 - m_bonds[i].x2) + 
                          (m_bonds[i].y1 - m_bonds[i].y2) * (m_bonds[i].y1 - m_bonds[i].y2));
    }
    m_avr_len /= double(m_num_bonds);

    while(fgets(buf, 510, fd))
    {
        trim_cr_lf(buf);
        if(buf[0] == '$') return true;
    }

    return false;
}



namespace agg
{
    class line
    {
    public:
        line() :
            m_x1(0.0), m_y1(0.0), m_x2(1.0), m_y2(0.0), m_thickness(0.1)
        {
        }

        line(double x1, double y1, double x2, double y2, double thickness) :
            m_x1(x1), m_y1(y1), m_x2(x2), m_y2(y2), m_thickness(thickness)
        {
        }

        void init(double x1, double y1, double x2, double y2)
        {
            m_x1 = x1;
            m_y1 = y1;
            m_x2 = x2;
            m_y2 = y2;
        }

        void thickness(double th)
        {
            m_thickness = th;
        }

        void rewind(unsigned start);
        unsigned vertex(double* x, double* y);

    private:
        double   m_x1;
        double   m_y1;
        double   m_x2;
        double   m_y2;
        double   m_dx;
        double   m_dy;
        double   m_thickness;
        unsigned m_vertex;
    };



    inline void line::rewind(unsigned)
    {
        calc_orthogonal(m_thickness*0.5, m_x1, m_y1, m_x2, m_y2, &m_dx, &m_dy);
        m_vertex = 0;
    }



    inline unsigned line::vertex(double* x, double* y)
    {
        switch(m_vertex)
        {
        case 0:
            *x = m_x1 - m_dx;
            *y = m_y1 - m_dy;
            m_vertex++;
            return path_cmd_move_to;

        case 1:
            *x = m_x2 - m_dx;
            *y = m_y2 - m_dy;
            m_vertex++;
            return path_cmd_line_to;

        case 2:
            *x = m_x2 + m_dx;
            *y = m_y2 + m_dy;
            m_vertex++;
            return path_cmd_line_to;

        case 3:
            *x = m_x1 + m_dx;
            *y = m_y1 + m_dy;
            m_vertex++;
            return path_cmd_line_to;
        }
        return path_cmd_stop;
    }



    class solid_wedge
    {
    public:
        solid_wedge() :
            m_x1(0.0), m_y1(0.0), m_x2(1.0), m_y2(0.0), m_thickness(0.1)
        {
        }

        solid_wedge(double x1, double y1, double x2, double y2, double thickness) :
            m_x1(x1), m_y1(y1), m_x2(x2), m_y2(y2), m_thickness(thickness)
        {
        }

        void init(double x1, double y1, double x2, double y2)
        {
            m_x1 = x1;
            m_y1 = y1;
            m_x2 = x2;
            m_y2 = y2;
        }

        void thickness(double th)
        {
            m_thickness = th;
        }

        void rewind(unsigned start);
        unsigned vertex(double* x, double* y);

    private:
        double   m_x1;
        double   m_y1;
        double   m_x2;
        double   m_y2;
        double   m_dx;
        double   m_dy;
        double   m_thickness;
        unsigned m_vertex;
    };



    inline void solid_wedge::rewind(unsigned)
    {
        calc_orthogonal(m_thickness*2.0, m_x1, m_y1, m_x2, m_y2, &m_dx, &m_dy);
        m_vertex = 0;
    }



    inline unsigned solid_wedge::vertex(double* x, double* y)
    {
        switch(m_vertex)
        {
        case 0:
            *x = m_x1;
            *y = m_y1;
            m_vertex++;
            return path_cmd_move_to;

        case 1:
            *x = m_x2 - m_dx;
            *y = m_y2 - m_dy;
            m_vertex++;
            return path_cmd_line_to;

        case 2:
            *x = m_x2 + m_dx;
            *y = m_y2 + m_dy;
            m_vertex++;
            return path_cmd_line_to;
        }
        return path_cmd_stop;
    }









    class dashed_wedge
    {
    public:
        dashed_wedge() :
            m_x1(0.0), m_y1(0.0), m_x2(1.0), m_y2(0.0), 
            m_thickness(0.1),
            m_num_dashes(8)
        {
        }

        dashed_wedge(double x1, double y1, double x2, double y2, 
                     double thickness, unsigned num_dashes=8) :
            m_x1(x2), m_y1(y2), m_x2(x1), m_y2(y1), 
            m_thickness(thickness),
            m_num_dashes(num_dashes)
        {
        }

        void init(double x1, double y1, double x2, double y2)
        {
            m_x1 = x2;
            m_y1 = y2;
            m_x2 = x1;
            m_y2 = y1;
        }

        void num_dashes(unsigned nd)
        {
            m_num_dashes = nd;
        }

        void thickness(double th)
        {
            m_thickness = th;
        }

        void rewind(unsigned start);
        unsigned vertex(double* x, double* y);

    private:
        double   m_x1;
        double   m_y1;
        double   m_x2;
        double   m_y2;
        double   m_xt2;
        double   m_yt2;
        double   m_xt3;
        double   m_yt3;
        double   m_xd[4];
        double   m_yd[4];
        double   m_thickness;
        unsigned m_num_dashes;
        unsigned m_vertex;
    };



    void dashed_wedge::rewind(unsigned)
    {
        double dx;
        double dy;
        calc_orthogonal(m_thickness*2.0, m_x1, m_y1, m_x2, m_y2, &dx, &dy);
        m_xt2 = m_x2 - dx;
        m_yt2 = m_y2 - dy;
        m_xt3 = m_x2 + dx;
        m_yt3 = m_y2 + dy;
        m_vertex = 0;
    }


    unsigned dashed_wedge::vertex(double* x, double* y)
    {
        if(m_vertex < m_num_dashes * 4)
        {
            if((m_vertex % 4) == 0)
            {
                double k1 = double(m_vertex / 4) / double(m_num_dashes);
                double k2 = k1 + 0.4 / double(m_num_dashes);

                m_xd[0] = m_x1 + (m_xt2 - m_x1) * k1;
                m_yd[0] = m_y1 + (m_yt2 - m_y1) * k1;
                m_xd[1] = m_x1 + (m_xt2 - m_x1) * k2;
                m_yd[1] = m_y1 + (m_yt2 - m_y1) * k2;
                m_xd[2] = m_x1 + (m_xt3 - m_x1) * k2;
                m_yd[2] = m_y1 + (m_yt3 - m_y1) * k2;
                m_xd[3] = m_x1 + (m_xt3 - m_x1) * k1;
                m_yd[3] = m_y1 + (m_yt3 - m_y1) * k1;
                *x = m_xd[0];
                *y = m_yd[0];
                m_vertex++;
                return path_cmd_move_to;
            }
            else
            {
                *x = m_xd[m_vertex % 4];
                *y = m_yd[m_vertex % 4];
                m_vertex++;
                return path_cmd_line_to;
            }
        }
        return path_cmd_stop;
    }











}






class bond_vertex_generator
{
    enum bond_style_e
    {
        bond_single,
        bond_wedged_solid,
        bond_wedged_dashed,
        bond_double,
        bond_double_left,
        bond_double_right,
        bond_triple
    };

public:
    bond_vertex_generator(const bond_type& bond, double thickness) :
        m_bond(bond),
        m_thickness(thickness),
        m_style(bond_single)
    {
        if(bond.order == 1)
        {
            if(bond.stereo == 1) m_style = bond_wedged_solid;
            if(bond.stereo == 6) m_style = bond_wedged_dashed;
        }
        if(bond.order == 2)
        {
            m_style = bond_double;
            if(bond.topology == 1) m_style = bond_double_left;
            if(bond.topology == 2) m_style = bond_double_right;
        }
        if(bond.order == 3) m_style = bond_triple;
        m_line1.thickness(thickness);
        m_line2.thickness(thickness);
        m_line3.thickness(thickness);
        m_solid_wedge.thickness(thickness);
        m_dashed_wedge.thickness(thickness);
    }

    void rewind(unsigned)
    {
        double dx, dy, dx1, dy1, dx2, dy2;

        switch(m_style)
        {
        case bond_wedged_solid:
            m_solid_wedge.init(m_bond.x1, m_bond.y1, m_bond.x2, m_bond.y2);
            m_solid_wedge.rewind(0);
            break;

        case bond_wedged_dashed:
            m_dashed_wedge.init(m_bond.x1, m_bond.y1, m_bond.x2, m_bond.y2);
            m_dashed_wedge.rewind(0);
            break;

        case bond_double:
        case bond_double_left:
        case bond_double_right:
            agg::calc_orthogonal(m_thickness, 
                                 m_bond.x1, m_bond.y1, 
                                 m_bond.x2, m_bond.y2, 
                                 &dx, &dy);
            dx1 = dy1 = 0;

            // To Do: ring perception and the proper drawing 
            // of the double bonds in the aromatic rings.
            //if(m_style == bond_double)
            {
                dx1 = dx2 = dx;
                dy1 = dy2 = dy;
            }
/*
            else if(m_style == bond_double_left)
            {
                dx2 = dx * 2.0;
                dy2 = dy * 2.0;
            }
            else
            {
                dx2 = -dx * 2.0;
                dy2 = -dy * 2.0;
            }
*/

            m_line1.init(m_bond.x1 - dx1, 
                         m_bond.y1 - dy1, 
                         m_bond.x2 - dx1, 
                         m_bond.y2 - dy1);
            m_line1.rewind(0);

            m_line2.init(m_bond.x1 + dx2, 
                         m_bond.y1 + dy2, 
                         m_bond.x2 + dx2, 
                         m_bond.y2 + dy2);
            m_line2.rewind(0);
            m_status = 0;
            break;

        case bond_triple:
            // To Do: triple bonds drawing.

        default:
            m_line1.init(m_bond.x1, m_bond.y1, m_bond.x2, m_bond.y2);
            m_line1.rewind(0);
            break;
        }
    }


    unsigned vertex(double* x, double* y)
    {
        unsigned flag = agg::path_cmd_stop;
        switch(m_style)
        {
            case bond_wedged_solid:
                return m_solid_wedge.vertex(x, y);

            case bond_wedged_dashed:
                return m_dashed_wedge.vertex(x, y);

            case bond_double_left:
            case bond_double_right:
            case bond_double:
                if(m_status == 0)
                {
                    flag = m_line1.vertex(x, y);
                    if(flag == agg::path_cmd_stop)
                    {
                        m_status = 1;
                    }
                }

                if(m_status == 1)
                {
                    flag = m_line2.vertex(x, y);
                }
                return flag;

            case bond_triple:
                break;
        }
        return m_line1.vertex(x, y);
    }



private:
    bond_vertex_generator(const bond_vertex_generator&);
    const bond_vertex_generator& operator = (const bond_vertex_generator&);

    const bond_type& m_bond;
    double m_thickness;
    bond_style_e m_style;
    agg::line m_line1;
    agg::line m_line2;
    agg::line m_line3;
    agg::solid_wedge m_solid_wedge;
    agg::dashed_wedge m_dashed_wedge;
    unsigned m_status;
};











class the_application : public agg::platform_support
{   
    molecule*                    m_molecules;
    unsigned                     m_num_molecules;
    unsigned                     m_cur_molecule;
    agg::slider_ctrl<agg::rgba8> m_thickness;
    agg::slider_ctrl<agg::rgba8> m_text_size;
    double     m_pdx;
    double     m_pdy;
    double     m_center_x;
    double     m_center_y;
    double     m_scale;
    double     m_prev_scale;
    double     m_angle;
    double     m_prev_angle;
    bool       m_mouse_move;
    agg::rgba8 m_atom_colors[end_of_atom_colors];


public:
    virtual ~the_application()
    {
        delete [] m_molecules;
    }

    the_application(agg::pix_format_e format, bool flip_y, const char* fname) :
        agg::platform_support(format, flip_y),
        m_molecules(new molecule [100]),
        m_num_molecules(0),
        m_cur_molecule(0),
        m_thickness(5, 5,  start_width-5, 12),
        m_text_size(5, 20, start_width-5, 27),
        m_pdx(0.0),
        m_pdy(0.0),
        m_center_x(start_width / 2),
        m_center_y(start_height / 2),
        m_scale(1.0),
        m_prev_scale(1.0),
        m_angle(0.0),
        m_prev_angle(0.0),
        m_mouse_move(false)
    {
        m_thickness.label("Thickness=%3.2f");
        m_text_size.label("Label Size=%3.2f");
        add_ctrl(m_thickness);
        add_ctrl(m_text_size);

        FILE* fd = fopen(full_file_name(fname), "r");
        if(fd)
        {
            unsigned i;
            for(i = 0; i < 100; i++)
            {
                if(!m_molecules[m_num_molecules].read(fd)) break;
                m_num_molecules++;
            }
            fclose(fd);
        }
        else
        {
            char buf[256];
            sprintf(buf, "File not found: '%s'. Download http://www.antigrain.com/%s\n",
                    fname, fname);
            message(buf);
        }
        memset(m_atom_colors, 0, sizeof(agg::rgba8) * end_of_atom_colors);
        m_atom_colors[atom_color_general] = agg::rgba8(0,0,0);
        m_atom_colors[atom_color_N]       = agg::rgba8(0,0,120);
        m_atom_colors[atom_color_O]       = agg::rgba8(200,0,0);
        m_atom_colors[atom_color_S]       = agg::rgba8(120,120,0);
        m_atom_colors[atom_color_P]       = agg::rgba8(80,50,0);
        m_atom_colors[atom_color_halogen] = agg::rgba8(0,200,0);
    }


    virtual void on_init()
    {
    }


    virtual void on_draw()
    {
        double width = initial_width();
        double height = initial_height();

        agg::rasterizer_scanline_aa<> ras;
        agg::scanline_p8 sl;

        typedef agg::renderer_base<pixfmt> renderer_base;
        typedef agg::renderer_scanline_aa_solid<renderer_base> renderer_solid;

        pixfmt pixf(rbuf_window());
        renderer_base rb(pixf);
        renderer_solid rs(rb);

        ras.clip_box(0, 0, rb.width(), rb.height());

        rb.clear(agg::rgba(1,1,1));

        const molecule& mol = m_molecules[m_cur_molecule];
        unsigned i;
        double min_x =  1e100;
        double max_x = -1e100;
        double min_y =  1e100;
        double max_y = -1e100;

        for(i = 0; i < mol.num_atoms(); i++)
        {
            if(mol.atom(i).x < min_x) min_x = mol.atom(i).x;
            if(mol.atom(i).y < min_y) min_y = mol.atom(i).y;
            if(mol.atom(i).x > max_x) max_x = mol.atom(i).x;
            if(mol.atom(i).y > max_y) max_y = mol.atom(i).y;
        }

        agg::trans_affine mtx;

        mtx *= agg::trans_affine_translation(-(max_x + min_x) * 0.5, -(max_y + min_y) * 0.5);
        
        double scale = width  / (max_x - min_x);
        double t = height / (max_y - min_y);
        if(scale > t) scale = t;
        
        double text_size = mol.average_bond_len() * m_text_size.value() / 4.0;
        double thickness = mol.average_bond_len() / 
                           sqrt(m_scale < 0.0001 ? 0.0001 : m_scale) /
                           8.0;
        
        mtx *= agg::trans_affine_scaling(scale*0.80, scale*0.80);
        mtx *= agg::trans_affine_rotation(m_angle);
        mtx *= agg::trans_affine_scaling(m_scale, m_scale);
        mtx *= agg::trans_affine_translation(m_center_x, m_center_y);
        mtx *= trans_affine_resizing();

        rs.color(agg::rgba(0,0,0));
        for(i = 0; i < mol.num_bonds(); i++)
        {
            bond_vertex_generator bond(mol.bond(i), 
                m_thickness.value() * thickness);
            agg::conv_transform<bond_vertex_generator> tr(bond, mtx);
            ras.add_path(tr);
            agg::render_scanlines(ras, sl, rs);
        }


        agg::ellipse ell;
        agg::conv_transform<agg::ellipse> tr(ell, mtx);
        for(i = 0; i < mol.num_atoms(); i++)
        {
            if(strcmp(mol.atom(i).label, "C") != 0)
            {
                ell.init(mol.atom(i).x, 
                         mol.atom(i).y, 
                         text_size * 2.5,
                         text_size * 2.5,
                         20);
                ras.add_path(tr);
                rs.color(agg::rgba(1.0, 1.0, 1.0));
                agg::render_scanlines(ras, sl, rs);
            }
        }


        text_size *= 3.0;


        agg::gsv_text label;
        agg::conv_stroke<agg::gsv_text> ls(label);
        agg::conv_transform<agg::conv_stroke<agg::gsv_text> > lo(ls, mtx);
        ls.line_join(agg::round_join);
        ls.line_cap(agg::round_cap);
        ls.approximation_scale(mtx.scale());
        for(i = 0; i < mol.num_atoms(); i++)
        {
            if(strcmp(mol.atom(i).label, "C") != 0)
            {
                ls.width(m_thickness.value() * thickness);
                label.text(mol.atom(i).label);
                label.start_point(mol.atom(i).x - text_size/2, mol.atom(i).y - text_size/2);
                label.size(text_size);
                ras.add_path(lo);
                rs.color(m_atom_colors[mol.atom(i).color_idx]);
                agg::render_scanlines(ras, sl, rs);
            }
        }


        ls.approximation_scale(1.0);
        agg::conv_transform<agg::conv_stroke<agg::gsv_text> > name(ls, trans_affine_resizing());
        ls.width(1.5);
        label.text(mol.name());
        label.size(10.0);
        label.start_point(10.0, start_height - 20.0);
        ras.reset();
        ras.add_path(name);
        rs.color(agg::rgba(0,0,0));
        agg::render_scanlines(ras, sl, rs);

        
        
        agg::render_ctrl(ras, sl, rb, m_thickness);
        agg::render_ctrl(ras, sl, rb, m_text_size);
    }














    virtual void on_idle()
    {
        m_angle += agg::deg2rad(0.1);
        force_redraw();
    }






    virtual void on_mouse_button_down(int x, int y, unsigned flags)
    {
        m_mouse_move = true;
        double x2 = x;
        double y2 = y;
        trans_affine_resizing().inverse_transform(&x2, &y2);

        m_pdx = m_center_x - x2;
        m_pdy = m_center_y - y2;
        m_prev_scale = m_scale;
        m_prev_angle = m_angle + agg::pi;
        force_redraw();
    }





    virtual void on_mouse_button_up(int x, int y, unsigned flags)
    {
        m_mouse_move = false;
    }

    virtual void on_mouse_move(int x, int y, unsigned flags)
    {
        double x2 = x;
        double y2 = y;
        trans_affine_resizing().inverse_transform(&x2, &y2);

        if(m_mouse_move && (flags & agg::mouse_left) != 0)
        {
            double dx = x2 - m_center_x;
            double dy = y2 - m_center_y;
            m_scale = m_prev_scale * 
                      sqrt(dx * dx + dy * dy) / 
                      sqrt(m_pdx * m_pdx + m_pdy * m_pdy);

            m_angle = m_prev_angle + atan2(dy, dx) - atan2(m_pdy, m_pdx);
            force_redraw();
        }

        if(m_mouse_move && (flags & agg::mouse_right) != 0)
        {
            m_center_x = x2 + m_pdx;
            m_center_y = y2 + m_pdy;
            force_redraw();
        }

    }







    virtual void on_key(int, int, unsigned key, unsigned)
    {
        switch(key)
        {
        case agg::key_left:
        case agg::key_up:
        case agg::key_page_up:
            if(m_cur_molecule) --m_cur_molecule;
            force_redraw();
            break;

        case agg::key_right:
        case agg::key_down:
        case agg::key_page_down:
            if(m_cur_molecule < m_num_molecules - 1) ++m_cur_molecule;
            force_redraw();
            break;

        case ' ':
            wait_mode(!wait_mode());
            break;
        }
    }

};





int agg_main(int argc, char* argv[])
{
    const char* fname = "1.sdf";
    if(argc > 1)
    {
        fname = argv[1];
    }


    the_application app(pix_format, flip_y, fname);
    app.caption("AGG - A Simple SDF Molecular Viewer");

    if(app.init(start_width, start_height, agg::window_resize))
    {
        return app.run();
    }

    return 1;
}


