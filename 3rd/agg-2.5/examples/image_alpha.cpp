#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "agg_ellipse.h"
#include "agg_trans_affine.h"
#include "agg_conv_transform.h"

#include "agg_rendering_buffer.h"
#include "agg_pixfmt_rgb.h"
#include "agg_span_allocator.h"
#include "agg_span_image_filter_rgb.h"
#include "agg_image_accessors.h"
#include "agg_span_interpolator_linear.h"
#include "agg_span_converter.h"
#include "agg_scanline_u.h"
#include "agg_renderer_scanline.h"
#include "agg_rasterizer_scanline_aa.h"

#include "ctrl/agg_spline_ctrl.h"
#include "platform/agg_platform_support.h"

enum flip_y_e { flip_y = true };


namespace agg
{
    
    //--------------------------------------------------------------------
    class span_conv_brightness_alpha_rgb8
    {
    public:
        typedef rgba8 color_type;
        typedef int8u alpha_type;

        enum array_size_e
        {
            array_size = 256 * 3
        };

        span_conv_brightness_alpha_rgb8(const alpha_type* alpha_array) :
            m_alpha_array(alpha_array)
        {
        }

        void prepare() {}
        void generate(color_type* span, int x, int y, unsigned len) const
        {
            do
            {
                span->a = m_alpha_array[span->r + span->g + span->b];
                ++span;
            }
            while(--len);
        }

    private:
        const alpha_type* m_alpha_array;
    };

}




class the_application : public agg::platform_support
{
    agg::spline_ctrl<agg::rgba8> m_alpha;
    double     m_x[50];
    double     m_y[50];
    double     m_rx[50];
    double     m_ry[50];
    agg::rgba8 m_colors[50];

public:
    the_application(agg::pix_format_e format, bool flip_y) :
        agg::platform_support(format, flip_y),
        m_alpha(2,  2,  200, 30,  6, !flip_y)
    {
        m_alpha.value(0, 1.0);
        m_alpha.value(1, 1.0);
        m_alpha.value(2, 1.0);
        m_alpha.value(3, 0.5);
        m_alpha.value(4, 0.5);
        m_alpha.value(5, 1.0);
        m_alpha.update_spline();
        add_ctrl(m_alpha);
    }

    virtual ~the_application()
    {
    }


    virtual void on_init()
    {
        unsigned i;
        for(i = 0; i < 50; i++)
        {
            m_x[i]  = rand() % int(width());
            m_y[i]  = rand() % int(height());
            m_rx[i] = rand() % 60 + 10;
            m_ry[i] = rand() % 60 + 10;
            m_colors[i] = agg::rgba8(rand() & 0xFF, 
                                     rand() & 0xFF, 
                                     rand() & 0xFF, 
                                     rand() & 0xFF);
        }
    }
    


    virtual void on_draw()
    {
        typedef agg::pixfmt_bgr24 pixfmt;
        typedef agg::renderer_base<pixfmt> renderer_base;

        pixfmt pixf(rbuf_window());
        renderer_base rb(pixf);

        rb.clear(agg::rgba(1.0, 1.0, 1.0));

        agg::trans_affine src_mtx;
        src_mtx *= agg::trans_affine_translation(-initial_width()/2, -initial_height()/2);
        src_mtx *= agg::trans_affine_rotation(10.0 * agg::pi / 180.0);
        src_mtx *= agg::trans_affine_translation(initial_width()/2, initial_height()/2);
        src_mtx *= trans_affine_resizing();

        agg::trans_affine img_mtx = src_mtx;
        img_mtx.invert();

        typedef agg::span_allocator<agg::rgba8> span_alloc;

        unsigned i;

        unsigned char brightness_alpha_array[agg::span_conv_brightness_alpha_rgb8::array_size];
        for(i = 0; i < agg::span_conv_brightness_alpha_rgb8::array_size; i++)
        {
            brightness_alpha_array[i] = 
                agg::int8u(m_alpha.value(double(i) / 
                         double(agg::span_conv_brightness_alpha_rgb8::array_size)) * 255.0);
        }
        agg::span_conv_brightness_alpha_rgb8 color_alpha(brightness_alpha_array);



        typedef agg::image_accessor_clip<pixfmt> img_source_type;
        typedef agg::span_interpolator_linear<> interpolator_type; 
        typedef agg::span_image_filter_rgb_bilinear<img_source_type,
                                                    interpolator_type> span_gen;
        typedef agg::span_converter<span_gen,
                                    agg::span_conv_brightness_alpha_rgb8> span_conv;


        span_alloc sa;
        interpolator_type interpolator(img_mtx);
        pixfmt img_pixf(rbuf_img(0));
        img_source_type img_src(img_pixf, agg::rgba(0,0,0,0));
        span_gen sg(img_src, interpolator);
        span_conv sc(sg, color_alpha);
        agg::ellipse ell;
        agg::rasterizer_scanline_aa<> ras;
        agg::scanline_u8 sl;
        
        for(i = 0; i < 50; i++)
        {
            ell.init(m_x[i], m_y[i], m_rx[i], m_ry[i], 50);
            ras.add_path(ell);
            agg::render_scanlines_aa_solid(ras, sl, rb, m_colors[i]);
        }


        ell.init(initial_width()  / 2.0, 
                 initial_height() / 2.0, 
                 initial_width()  / 1.9, 
                 initial_height() / 1.9, 200);


        agg::conv_transform<agg::ellipse> tr(ell, src_mtx);


        ras.add_path(tr);
        agg::render_scanlines_aa(ras, sl, rb, sa, sc);

        agg::render_ctrl(ras, sl, rb, m_alpha);
    }

    virtual void on_key(int x, int y, unsigned key, unsigned flags)
    {
        if(key == ' ')
        {
            FILE* fd = fopen(full_file_name("alpha"), "w");

            int i;
            for(i = 0; i < agg::span_conv_brightness_alpha_rgb8::array_size; i++)
            {
                int alpha = 
                    agg::int8u(m_alpha.value(double(i) / 
                             double(agg::span_conv_brightness_alpha_rgb8::array_size)) * 255.0);
                if(i % 32 == 0) fprintf(fd, "\n");
                fprintf(fd, "%3d, ", alpha);
            }

            fclose(fd);
        }
    }




};





int agg_main(int argc, char* argv[])
{
    the_application app(agg::pix_format_bgr24, flip_y);
    app.caption("Image Affine Transformations with Alpha-function");

    const char* img_name = "spheres";
    if(argc >= 2) img_name = argv[1];
    if(!app.load_img(0, img_name)) 
    {
        char buf[256];
        if(strcmp(img_name, "spheres") == 0)
        {
            sprintf(buf, "File not found: %s%s. Download http://www.antigrain.com/%s%s\n"
                         "or copy it from another directory if available.",
                    img_name, app.img_ext(), img_name, app.img_ext());
        }
        else
        {
            sprintf(buf, "File not found: %s%s", img_name, app.img_ext());
        }
        app.message(buf);
        return 1;
    }

    if(app.init(app.rbuf_img(0).width(), app.rbuf_img(0).height(), agg::window_resize))
    {
        return app.run();
    }
    return 0;
}


