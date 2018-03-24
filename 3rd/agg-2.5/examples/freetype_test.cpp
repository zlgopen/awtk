#include <stdio.h>
#include "agg_basics.h"
#include "agg_rendering_buffer.h"
#include "agg_scanline_u.h"
#include "agg_scanline_bin.h"
#include "agg_renderer_scanline.h"
#include "agg_renderer_primitives.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_conv_curve.h"
#include "agg_conv_contour.h"
#include "agg_pixfmt_rgb.h"
#include "agg_font_freetype.h"
#include "platform/agg_platform_support.h"

#include "ctrl/agg_slider_ctrl.h"
#include "ctrl/agg_cbox_ctrl.h"
#include "ctrl/agg_rbox_ctrl.h"


enum flip_y_e { flip_y = true };
bool font_flip_y = !flip_y;


#define pix_format agg::pix_format_bgr24
typedef agg::pixfmt_bgr24 pixfmt_type;


static char text[] = 
"Anti-Grain Geometry is designed as a set of loosely coupled "
"algorithms and class templates united with a common idea, "
"so that all the components can be easily combined. Also, "
"the template based design allows you to replace any part of "
"the library without the necessity to modify a single byte in "
"the existing code. "
"AGG is designed keeping in mind extensibility and flexibility. "
"Basically I just wanted to create a toolkit that would allow me "
"(and anyone else) to add new fancy algorithms very easily. "
"AGG does not dictate you any style of its use, you are free to "
"use any part of it. However, AGG is often associated with a tool "
"for rendering images in memory. That is not quite true, but it can "
"be a good starting point in studying. The tutorials describe the "
"use of AGG starting from the low level functionality that deals with "
"frame buffers and pixels. Then you will gradually understand how to "
"abstract different parts of the library and how to use them separately. "
"Remember, the raster picture is often not the only thing you want to "
"obtain, you will probably want to print your graphics with highest "
"possible quality and in this case you can easily combine the \"vectorial\" "
"part of the library with some API like Windows GDI, having a common "
"external interface. If that API can render multi-polygons with non-zero "
"and even-odd filling rules it's all you need to incorporate AGG into "
"your application. For example, Windows API PolyPolygon perfectly fits "
"these needs, except certain advanced things like gradient filling, "
"Gouraud shading, image transformations, and so on. Or, as an alternative, "
"you can use all AGG algorithms producing high resolution pixel images and "
"then to send the result to the printer as a pixel map."
"Below is a typical brief scheme of the AGG rendering pipeline. "
"Please note that any component between the Vertex Source "
"and Screen Output is not mandatory. It all depends on your "
"particular needs. For example, you can use your own rasterizer, "
"based on Windows API. In this case you won't need the AGG rasterizer "
"and renderers. Or, if you need to draw only lines, you can use the "
"AGG outline rasterizer that has certain restrictions but works faster. "
"The number of possibilities is endless. "
"Vertex Source is some object that produces polygons or polylines as "
"a set of consecutive 2D vertices with commands like MoveTo, LineTo. "
"It can be a container or some other object that generates vertices "
"on demand. "
"Coordinate conversion pipeline consists of a number of coordinate "
"converters. It always works with vectorial data (X,Y) represented "
"as floating point numbers (double). For example, it can contain an "
"affine transformer, outline (stroke) generator, some marker "
"generator (like arrowheads/arrowtails), dashed lines generator, "
"and so on. The pipeline can have branches and you also can have "
"any number of different pipelines. You also can write your own "
"converter and include it into the pipeline. "
"Scanline Rasterizer converts vectorial data into a number of "
"horizontal scanlines. The scanlines usually (but not obligatory) "
"carry information about Anti-Aliasing as coverage values. "
"Renderers render scanlines, sorry for the tautology. The simplest "
"example is solid filling. The renderer just adds a color to the "
"scanline and writes the result into the rendering buffer. "
"More complex renderers can produce multi-color result, "
"like gradients, Gouraud shading, image transformations, "
"patterns, and so on. Rendering Buffer is a buffer in memory "
"that will be displayed afterwards. Usually but not obligatory "
"it contains pixels in format that fits your video system. "
"For example, 24 bits B-G-R, 32 bits B-G-R-A, or 15 "
"bits R-G-B-555 for Windows. But in general, there're no "
"restrictions on pixel formats or color space if you write "
"your own low level class that supports that format. "
"Colors in AGG appear only in renderers, that is, when you "
"actually put some data to the rendering buffer. In general, "
"there's no general purpose structure or class like color, "
"instead, AGG always operates with concrete color space. "
"There are plenty of color spaces in the world, like RGB, "
"HSV, CMYK, etc., and all of them have certain restrictions. "
"For example, the RGB color space is just a poor subset of "
"colors that a human eye can recognize. If you look at the full "
"CIE Chromaticity Diagram, you will see that the RGB triangle "
"is just a little part of it. "
"In other words there are plenty of colors in the real world "
"that cannot be reproduced with RGB, CMYK, HSV, etc. Any color "
"space except the one existing in Nature is restrictive. Thus, "
"it was decided not to introduce such an object like color in "
"order not to restrict the possibilities in advance. Instead, "
"there are objects that operate with concrete color spaces. "
"Currently there are agg::rgba and agg::rgba8 that operate "
"with the most popular RGB color space (strictly speaking there's "
"RGB plus Alpha). The RGB color space is used with different "
"pixel formats, like 24-bit RGB or 32-bit RGBA with different "
"order of color components. But the common property of all of "
"them is that they are essentially RGB. Although, AGG doesn't "
"explicitly support any other color spaces, there is at least "
"a potential possibility of adding them. It means that all "
"class and function templates that depend on the color type "
"are parameterized with the ColorT argument. "
"Basically, AGG operates with coordinates of the output device. "
"On your screen there are pixels. But unlike many other libraries "
"and APIs AGG initially supports Subpixel Accuracy. It means "
"that the coordinates are represented as doubles, where fractional "
"values actually take effect. AGG doesn't have an embedded "
"conversion mechanism from world to screen coordinates in order "
"not to restrict your freedom. It's very important where and when "
"you do that conversion, so, different applications can require "
"different approaches. AGG just provides you a transformer of "
"that kind, namely, that can convert your own view port to the "
"device one. And it's your responsibility to include it into "
"the proper place of the pipeline. You can also write your "
"own very simple class that will allow you to operate with "
"millimeters, inches, or any other physical units. "
"Internally, the rasterizers use integer coordinates of the "
"format 24.8 bits, that is, 24 bits for the integer part and 8 "
"bits for the fractional one. In other words, all the internal "
"coordinates are multiplied by 256. If you intend to use AGG in "
"some embedded system that has inefficient floating point "
"processing, you still can use the rasterizers with their "
"integer interfaces. Although, you won't be able to use the "
"floating point coordinate pipelines in this case. ";
 






template<class VS> void dump_path(VS& path)
{
    FILE* fd = fopen("dump_path", "a");
    fprintf(fd, "-------\n");
    path.rewind(0);
    unsigned cmd;
    double x, y;
    while(!agg::is_stop(cmd = path.vertex(&x, &y)))
    {
        fprintf(fd, "%02X %8.2f %8.2f\n", cmd, x, y);
    }
    fclose(fd);
}




class the_application : public agg::platform_support
{
    typedef agg::renderer_base<pixfmt_type> base_ren_type;
    typedef agg::renderer_scanline_aa_solid<base_ren_type> renderer_solid;
    typedef agg::renderer_scanline_bin_solid<base_ren_type> renderer_bin;
    typedef agg::font_engine_freetype_int32 font_engine_type;
    typedef agg::font_cache_manager<font_engine_type> font_manager_type;

    agg::rbox_ctrl<agg::rgba8>   m_ren_type;
    agg::slider_ctrl<agg::rgba8> m_height;
    agg::slider_ctrl<agg::rgba8> m_width;
    agg::slider_ctrl<agg::rgba8> m_weight;
    agg::slider_ctrl<agg::rgba8> m_gamma;
    agg::cbox_ctrl<agg::rgba8>   m_hinting;
    agg::cbox_ctrl<agg::rgba8>   m_kerning;
    agg::cbox_ctrl<agg::rgba8>   m_performance;
    font_engine_type             m_feng;
    font_manager_type            m_fman;
    double                       m_old_height;

    // Pipeline to process the vectors glyph paths (curves + contour)
    agg::conv_curve<font_manager_type::path_adaptor_type> m_curves;
    agg::conv_contour<agg::conv_curve<font_manager_type::path_adaptor_type> > m_contour;




public:
    the_application(agg::pix_format_e format, bool flip_y) :
        agg::platform_support(format, flip_y),
        m_ren_type     (5.0, 5.0, 5.0+150.0,   110.0,  !flip_y),
        m_height       (160, 10.0, 640-5.0,    18.0,   !flip_y),
        m_width        (160, 30.0, 640-5.0,    38.0,   !flip_y),
        m_weight       (160, 50.0, 640-5.0,    58.0,   !flip_y),
        m_gamma        (260, 70.0, 640-5.0,    78.0,   !flip_y),
        m_hinting      (160, 65.0, "Hinting", !flip_y),
        m_kerning      (160, 80.0, "Kerning", !flip_y),
        m_performance  (160, 95.0, "Test Performance", !flip_y),
        m_feng(),
        m_fman(m_feng),
        m_old_height(0.0),
        m_curves(m_fman.path_adaptor()),
        m_contour(m_curves)
    {
        m_ren_type.add_item("Native Mono");
        m_ren_type.add_item("Native Gray 8");
        m_ren_type.add_item("Outline");
        m_ren_type.add_item("AGG Mono");
        m_ren_type.add_item("AGG Gray 8");
        m_ren_type.cur_item(1);
        add_ctrl(m_ren_type);
        m_ren_type.no_transform();

        m_height.label("Font Height=%.2f");
        m_height.range(8, 32);
        m_height.num_steps(32-8);
        m_height.value(18);
        m_height.text_thickness(1.5);
        add_ctrl(m_height);
        m_height.no_transform();

        m_width.label("Font Width=%.2f");
        m_width.range(8, 32);
        m_width.num_steps(32-8);
        m_width.text_thickness(1.5);
        m_width.value(18);
        add_ctrl(m_width);
        m_width.no_transform();

        m_weight.label("Font Weight=%.2f");
        m_weight.range(-1, 1);
        m_weight.text_thickness(1.5);
        add_ctrl(m_weight);
        m_weight.no_transform();

        m_gamma.label("Gamma=%.2f");
        m_gamma.range(0.1, 2.0);
        m_gamma.value(1.0);
        m_gamma.text_thickness(1.5);
        add_ctrl(m_gamma);
        m_gamma.no_transform();

        add_ctrl(m_hinting);
        m_hinting.status(true);
        m_hinting.no_transform();

        add_ctrl(m_kerning);
        m_kerning.status(true);
        m_kerning.no_transform();

        add_ctrl(m_performance);
        m_performance.no_transform();

        m_curves.approximation_scale(2.0);
        m_contour.auto_detect_orientation(false);
    }


    template<class Rasterizer, class Scanline, class RenSolid, class RenBin>
    unsigned draw_text(Rasterizer& ras, Scanline& sl, 
                       RenSolid& ren_solid, RenBin& ren_bin)
    {
        agg::glyph_rendering gren = agg::glyph_ren_native_mono;
        switch(m_ren_type.cur_item())
        {
        case 0: gren = agg::glyph_ren_native_mono;  break;
        case 1: gren = agg::glyph_ren_native_gray8; break;
        case 2: gren = agg::glyph_ren_outline;      break;
        case 3: gren = agg::glyph_ren_agg_mono;     break;
        case 4: gren = agg::glyph_ren_agg_gray8;    break;
        }

        unsigned num_glyphs = 0;

        m_contour.width(-m_weight.value() * m_height.value() * 0.05);

        if(m_feng.load_font(full_file_name("timesi.ttf"), 0, gren))
        {
            m_feng.hinting(m_hinting.status());
            m_feng.height(m_height.value());
            m_feng.width(m_width.value());
            m_feng.flip_y(font_flip_y);

            agg::trans_affine mtx;
            mtx *= agg::trans_affine_rotation(agg::deg2rad(-4.0));
            //mtx *= agg::trans_affine_skewing(-0.4, 0);
            //mtx *= agg::trans_affine_translation(1, 0);
            m_feng.transform(mtx);

            double x  = 10.0;
            double y0 = height() - m_height.value() - 10.0;
            double y  = y0;
            const char* p = text;

            while(*p)
            {
                const agg::glyph_cache* glyph = m_fman.glyph(*p);
                if(glyph)
                {
                    if(m_kerning.status())
                    {
                        m_fman.add_kerning(&x, &y);
                    }

                    if(x >= width() - m_height.value())
                    {
                        x = 10.0;
                        y0 -= m_height.value();
                        if(y0 <= 120) break;
                        y = y0;
                    }

                    m_fman.init_embedded_adaptors(glyph, x, y);

                    switch(glyph->data_type)
                    {
                    default: break;
                    case agg::glyph_data_mono:
                        ren_bin.color(agg::rgba8(0, 0, 0));
                        agg::render_scanlines(m_fman.mono_adaptor(), 
                                              m_fman.mono_scanline(), 
                                              ren_bin);
                        break;

                    case agg::glyph_data_gray8:
                        ren_solid.color(agg::rgba8(0, 0, 0));
                        agg::render_scanlines(m_fman.gray8_adaptor(), 
                                              m_fman.gray8_scanline(), 
                                              ren_solid);
                        break;

                    case agg::glyph_data_outline:
                        ras.reset();
                        if(fabs(m_weight.value()) <= 0.01)
                        {
                            // For the sake of efficiency skip the
                            // contour converter if the weight is about zero.
                            //-----------------------
                            ras.add_path(m_curves);
                        }
                        else
                        {
                            ras.add_path(m_contour);
                        }
                        ren_solid.color(agg::rgba8(0, 0, 0));
                        agg::render_scanlines(ras, sl, ren_solid);
//dump_path(m_fman.path_adaptor());
                        break;
                    }

                    // increment pen position
                    x += glyph->advance_x;
                    y += glyph->advance_y;
                    ++num_glyphs;
                }
                ++p;
            }
        }
        else
        {
            message("Please copy file timesi.ttf to the current directory\n"
                    "or download it from http://www.antigrain.com/timesi.zip");
        }

        return num_glyphs;
    }


    virtual void on_draw()
    {
        pixfmt_type pf(rbuf_window());
        base_ren_type ren_base(pf);
        renderer_solid ren_solid(ren_base);
        renderer_bin ren_bin(ren_base);
        ren_base.clear(agg::rgba(1,1,1));

        agg::scanline_u8 sl;
        agg::rasterizer_scanline_aa<> ras;

        if(m_height.value() != m_old_height)
        {
            m_width.value(m_old_height = m_height.value());
        }

        if(m_ren_type.cur_item() == 3)
        {
            // When rendering in mono format, 
            // Set threshold gamma = 0.5
            //-------------------
            m_feng.gamma(agg::gamma_threshold(m_gamma.value() / 2.0));
        }
        else
        {
            m_feng.gamma(agg::gamma_power(m_gamma.value()));
        }

        if(m_ren_type.cur_item() == 2)
        {
            // For outline cache set gamma for the rasterizer
            //-------------------
            ras.gamma(agg::gamma_power(m_gamma.value()));
        }

//ren_base.copy_hline(0, int(height() - m_height.value()) - 10, 100, agg::rgba(0,0,0));
        draw_text(ras, sl, ren_solid, ren_bin);

        ras.gamma(agg::gamma_power(1.0));


        agg::render_ctrl(ras, sl, ren_base, m_ren_type);
        agg::render_ctrl(ras, sl, ren_base, m_height);
        agg::render_ctrl(ras, sl, ren_base, m_width);
        agg::render_ctrl(ras, sl, ren_base, m_weight);
        agg::render_ctrl(ras, sl, ren_base, m_gamma);
        agg::render_ctrl(ras, sl, ren_base, m_hinting);
        agg::render_ctrl(ras, sl, ren_base, m_kerning);
        agg::render_ctrl(ras, sl, ren_base, m_performance);
    }



    virtual void on_ctrl_change()
    {
        if(m_performance.status())
        {
            pixfmt_type pf(rbuf_window());
            base_ren_type ren_base(pf);
            renderer_solid ren_solid(ren_base);
            renderer_bin ren_bin(ren_base);
            ren_base.clear(agg::rgba(1,1,1));

            agg::scanline_u8 sl;
            agg::rasterizer_scanline_aa<> ras;

            unsigned num_glyphs = 0;
            start_timer();
            for(int i = 0; i < 50; i++)
            {
                num_glyphs += draw_text(ras, sl, ren_solid, ren_bin);
            }
            double t = elapsed_time();
            char buf[100];
            sprintf(buf, 
                    "Glyphs=%u, Time=%.3fms, %.3f glyps/sec, %.3f microsecond/glyph", 
                    num_glyphs,
                    t, 
                    (num_glyphs / t) * 1000.0, 
                    (t / num_glyphs) * 1000.0);
            message(buf);

            m_performance.status(false);
            force_redraw();
        }
    }


    virtual void on_key(int x, int y, unsigned key, unsigned flags)
    {
        if(key == ' ')
        {
            font_flip_y = !font_flip_y;
            force_redraw();
        }
    }

};



int agg_main(int argc, char* argv[])
{
    the_application app(pix_format, flip_y);
    app.caption("AGG Example. Rendering Fonts with FreeType");

    if(app.init(640, 520, agg::window_resize))
    {
        return app.run();
    }
    return 1;
}


