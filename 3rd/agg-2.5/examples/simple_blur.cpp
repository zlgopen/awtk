#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include "agg_basics.h"
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_rasterizer_outline.h"
#include "agg_path_storage.h"
#include "agg_conv_stroke.h"
#include "agg_conv_transform.h"
#include "agg_bounding_rect.h"
#include "agg_scanline_u.h"
#include "agg_scanline_p.h"
#include "agg_pixfmt_rgb.h"
#include "agg_renderer_base.h"
#include "agg_renderer_outline_aa.h"
#include "agg_rasterizer_outline_aa.h"
#include "agg_renderer_scanline.h"
#include "agg_span_allocator.h"
#include "agg_ellipse.h"
#include "platform/agg_platform_support.h"

enum flip_y_e { flip_y = true };

agg::path_storage g_path;
agg::rgba8        g_colors[100];
unsigned          g_path_idx[100];
unsigned          g_npaths = 0;
double            g_x1 = 0;
double            g_y1 = 0;
double            g_x2 = 0;
double            g_y2 = 0;
double            g_base_dx = 0;
double            g_base_dy = 0;
double            g_angle = 0;
double            g_scale = 1.0;
double            g_skew_x = 0;
double            g_skew_y = 0;
int               g_nclick = 0;




unsigned parse_lion(agg::path_storage& ps, agg::rgba8* colors, unsigned* path_idx);
void parse_lion()
{
    g_npaths = parse_lion(g_path, g_colors, g_path_idx);
    agg::pod_array_adaptor<unsigned> path_idx(g_path_idx, 100);
    agg::bounding_rect(g_path, path_idx, 0, g_npaths, &g_x1, &g_y1, &g_x2, &g_y2);
    g_base_dx = (g_x2 - g_x1) / 2.0;
    g_base_dy = (g_y2 - g_y1) / 2.0;
}






namespace agg
{
    template<class Order> class span_simple_blur_rgb24
    {
    public:
        //--------------------------------------------------------------------
        typedef rgba8 color_type;
        
        //--------------------------------------------------------------------
        span_simple_blur_rgb24() : m_source_image(0) {}

        //--------------------------------------------------------------------
        span_simple_blur_rgb24(const rendering_buffer& src) : 
            m_source_image(&src) {}

        //--------------------------------------------------------------------
        void source_image(const rendering_buffer& src) { m_source_image = &src; }
        const rendering_buffer& source_image() const { return *m_source_image; }

        //--------------------------------------------------------------------
        void prepare() {}

        //--------------------------------------------------------------------
        void generate(color_type* span, int x, int y, int len)
        {
            if(y < 1 || y >= int(m_source_image->height() - 1))
            {
                do
                {
                    *span++ = rgba8(0,0,0,0);
                }
                while(--len);
                return;
            }

            do
            {
                int color[4];
                color[0] = color[1] = color[2] = color[3] = 0;
                if(x > 0 && x < int(m_source_image->width()-1))
                {
                    int i = 3;
                    do
                    {
                        const int8u* ptr = m_source_image->row_ptr(y - i + 2) + (x - 1) * 3;

                        color[0] += *ptr++;
                        color[1] += *ptr++;
                        color[2] += *ptr++;
                        color[3] += 255;

                        color[0] += *ptr++;
                        color[1] += *ptr++;
                        color[2] += *ptr++;
                        color[3] += 255;

                        color[0] += *ptr++;
                        color[1] += *ptr++;
                        color[2] += *ptr++;
                        color[3] += 255;
                    }
                    while(--i);
                    color[0] /= 9;
                    color[1] /= 9;
                    color[2] /= 9;
                    color[3] /= 9;
                }
                *span++ = rgba8(color[Order::R], color[Order::G], color[Order::B], color[3]);
                ++x;
            }
            while(--len);
        }

    private:
        const rendering_buffer* m_source_image;
    };

}












class the_application : public agg::platform_support
{
    double m_cx;
    double m_cy;

public:
    virtual ~the_application() 
    {
    }

    the_application(agg::pix_format_e format, bool flip_y) :
        agg::platform_support(format, flip_y),
        m_cx(100),
        m_cy(102)
    {
        parse_lion();
    }

    virtual void on_resize(int cx, int cy)
    {
    }

    virtual void on_draw()
    {
        typedef agg::pixfmt_bgr24 pixfmt;
        typedef agg::renderer_base<pixfmt> renderer_base;
        typedef agg::renderer_scanline_aa_solid<renderer_base> renderer_solid;
        
        pixfmt pixf(rbuf_window());
        renderer_base rb(pixf);
        renderer_solid rs(rb);
        
        rb.clear(agg::rgba(1, 1, 1));

        agg::trans_affine mtx;
        agg::conv_transform<agg::path_storage, agg::trans_affine> trans(g_path, mtx);

        mtx *= agg::trans_affine_translation(-g_base_dx, -g_base_dy);
        mtx *= agg::trans_affine_scaling(g_scale, g_scale);
        mtx *= agg::trans_affine_rotation(g_angle + agg::pi);
        mtx *= agg::trans_affine_skewing(g_skew_x/1000.0, g_skew_y/1000.0);
        mtx *= agg::trans_affine_translation(initial_width()/4, initial_height()/2);
        mtx *= trans_affine_resizing();

        agg::rasterizer_scanline_aa<> ras2;
        agg::scanline_p8 sl;
        agg::scanline_u8 sl2;

        agg::render_all_paths(ras2, sl, rs, trans, g_colors, g_path_idx, g_npaths);

        mtx *= ~trans_affine_resizing();
        mtx *= agg::trans_affine_translation(initial_width()/2, 0);
        mtx *= trans_affine_resizing();

        agg::line_profile_aa profile;
        profile.width(1.0);
        agg::renderer_outline_aa<renderer_base> rp(rb, profile);
        agg::rasterizer_outline_aa<agg::renderer_outline_aa<renderer_base> > ras(rp);
        ras.round_cap(true);

        ras.render_all_paths(trans, g_colors, g_path_idx, g_npaths);

        agg::ellipse ell(m_cx, m_cy, 100.0, 100.0, 100);
        agg::conv_stroke<agg::ellipse> ell_stroke1(ell);
        ell_stroke1.width(6.0);
        agg::conv_stroke<agg::conv_stroke<agg::ellipse> > ell_stroke2(ell_stroke1);

        ell_stroke2.width(2.0);
        rs.color(agg::rgba(0,0.2,0));
        ras2.add_path(ell_stroke2);
        agg::render_scanlines(ras2, sl, rs);

        typedef agg::span_simple_blur_rgb24<agg::order_bgr> span_blur_gen;
        typedef agg::span_allocator<span_blur_gen::color_type> span_blur_alloc;

        span_blur_alloc sa;
        span_blur_gen sg;

        sg.source_image(rbuf_img(0));
        ras2.add_path(ell);

        copy_window_to_img(0);
        agg::render_scanlines_aa(ras2, sl2, rb, sa, sg);

        // More blur if desired :-)
        //copy_window_to_img(0);
        //agg::render_scanlines(ras2, sl2, rblur);
        //copy_window_to_img(0);
        //agg::render_scanlines(ras2, sl2, rblur);
        //copy_window_to_img(0);
        //agg::render_scanlines(ras2, sl2, rblur);
    }


    virtual void on_mouse_button_down(int x, int y, unsigned flags)
    {
        if(flags & agg::mouse_left)
        {
            m_cx = x;
            m_cy = y;
            force_redraw();
        }
    }


    virtual void on_mouse_move(int x, int y, unsigned flags)
    {
        on_mouse_button_down(x, y, flags);
    }

};






int agg_main(int argc, char* argv[])
{
    the_application app(agg::pix_format_bgr24, flip_y);
    app.caption("AGG Example. Lion with Alpha-Masking");

    if(app.init(512, 400, agg::window_resize))
    {
        return app.run();
    }
    return 1;
}






