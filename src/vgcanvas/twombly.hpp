/*adapted from: https://github.com/zshipko/libtwombly.git*/

#include <stdlib.h>
#include <stdint.h>

#ifndef TWOMBLY_DRAW_HEADER
#define TWOMBLY_DRAW_HEADER

// Defined for both C and C++

enum line_cap_style {
    butt_cap,
    square_cap,
    round_cap
};

enum line_join_style {
    miter_join         = 0,
    miter_join_revert  = 1,
    round_join         = 2,
    bevel_join         = 3,
    miter_join_round   = 4
};

enum filling_rule {
    fill_non_zero,
    fill_even_odd
};

enum path_commands {
    path_cmd_stop     = 0,
    path_cmd_move_to  = 1,
    path_cmd_line_to  = 2,
    path_cmd_curve3   = 3,
    path_cmd_curve4   = 4,
    path_cmd_curve_n   = 5,
    path_cmd_catrom   = 6,
    path_cmd_ubspline = 7,
    path_cmd_end_poly = 0xff,
    path_cmd_mask     = 0xff
};

enum gradient_type {
    gradient_type_circle,
    gradient_type_radial,
    gradient_type_radial_d,
    gradient_type_radial_focus,
    gradient_type_x,
    gradient_type_y,
    gradient_type_diamond,
    gradient_type_xy,
    gradient_type_sqrt_xy,
    gradient_type_conic
};

#define TW_POLY_SIZE(p) (sizeof(p) / (sizeof(*p) * 2))

#ifdef __cplusplus
#include <vector>

#include "agg_basics.h"
#include "agg_rendering_buffer.h"
#include "agg_renderer_base.h"
#include "agg_pixfmt_rgb.h"
#include "agg_pixfmt_rgba.h"
#include "agg_pixfmt_gray.h"
#include "agg_conv_stroke.h"
#include "agg_conv_curve.h"
#include "agg_conv_contour.h"
#include "agg_conv_dash.h"
#include "agg_conv_transform.h"
#include "agg_curves.h"
#include "agg_ellipse.h"
#include "agg_blur.h"
#include "agg_bezier_arc.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_p.h"
#include "agg_renderer_scanline.h"
#include "agg_path_storage.h"
#include "agg_span_allocator.h"
#include "agg_span_gouraud.h"
#include "agg_span_gradient.h"
#include "agg_span_image_filter.h"
#include "agg_span_image_filter_rgb.h"
#include "agg_span_image_filter_rgba.h"
#include "agg_span_pattern_gray.h"
#include "agg_span_pattern_rgb.h"
#include "agg_span_pattern_rgba.h"
#include "agg_rounded_rect.h"
#include "agg_gamma_lut.h"
#include "agg_gsv_text.h"
#include "agg_image_accessors.h"
#include "agg_pixfmt_amask_adaptor.h"
#include "agg_trans_affine.h"

namespace tw {

// The Color type is used when dealing with
// colors withing the drawing context. comes in both 8 and 16 bit variations
typedef agg::rgba8 Color;
typedef agg::rgba16 Color16;

typedef agg::pixfmt_rgba32 rgba32;
typedef agg::pixfmt_rgb24 rgb24;
typedef agg::pixfmt_bgra32 bgra32;
typedef agg::pixfmt_bgr24 bgr24;
typedef agg::pixfmt_gray8 gray8;
typedef agg::pixfmt_rgba64 rgba64;
typedef agg::pixfmt_rgb48 rgb48;
typedef agg::pixfmt_bgra64 bgra64;
typedef agg::pixfmt_bgr48 bgr48;
typedef agg::pixfmt_gray16 gray16;

inline Color rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
inline Color rgb(uint8_t r, uint8_t g, uint8_t b);
inline Color16 rgba_16(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
inline Color16 rgb_16(uint8_t r, uint8_t g, uint8_t b);

template <typename ColorType, int S=256>
class Gradient {
public:
    std::vector<ColorType> colors;
    agg::pod_auto_array<ColorType, S> arr;
    agg::trans_affine mtx;

    Gradient() {}

    Gradient(ColorType beg, ColorType mid, ColorType end){
        add_stop(beg);
        add_stop(mid);
        add_stop(end);
    }

    void add_stop(ColorType const & c){
        colors.push_back(c);
    }

    void generate(){
        unsigned stop_len = arr.size() / (colors.size()-1);
        for(size_t color_index = 0; color_index < colors.size()-1; color_index++){
            for(unsigned i = 0; i < stop_len; i++){
                arr[i + color_index*stop_len] = colors[color_index].gradient(colors[color_index+1], (i - stop_len) / double(stop_len));
            }
        }
    }
};

// Drawing type
template <typename DrawingType>
class Drawing : public agg::path_storage {
private:
    void *data; // buffer data
    // render settings
    bool _antialias, _preserve;
    double _linewidth; // linewidth
    double _miterlimit;
    line_cap_style _linecap;
    line_join_style _linejoin;
    bool owns_data, owns_alpha; // data ownership information
    unsigned pathid; // stores current path
    Color current_color;
    Color stroke_color;
    Color fill_color;

public:
    agg::trans_affine mtx;
    // rendering
    agg::renderer_scanline_aa_solid<agg::renderer_base<DrawingType> > render_aa;
    agg::renderer_scanline_bin_solid<agg::renderer_base<DrawingType> > render_bin;
    agg::renderer_base<DrawingType> base;
    agg::rendering_buffer buffer;
    agg::rasterizer_scanline_aa<> *raster;
    agg::scanline32_p8 *sl;
    std::vector<unsigned> all_paths;

    DrawingType pix;
    struct size {
        size(int32_t _x, int32_t _y, int32_t _c) : x(_x), y(_y), c(_c){}
        int32_t x, y, c;
    } size;


    Drawing<DrawingType>(int32_t w, int32_t h, int c, uint8_t *d) :
        data(d != NULL ? (void*)d : calloc(w * h * c, sizeof(uint8_t))), 
        _antialias(true), 
        _preserve(false), 
        _linewidth(1), 
        owns_data(d == NULL), 
        pathid(0), 
        buffer((uint8_t*)data, w, h, w * c), 
        raster(NULL), 
        sl(NULL), 
        pix(buffer), 
        size(w, h, c) {
        alloc();
    }

    ~Drawing(){
        if (raster){
            delete raster;
            raster = NULL;
        }

        if (sl){
            delete sl;
            sl = NULL;
        }

        if (owns_data){
            if (data) free(data);
            data = NULL;
        }
    }

    // Actually create the image,
    // this function will replace the current context if it already exists
    void alloc(){
        _miterlimit = 1;
        base = agg::renderer_base<DrawingType>(pix);
        render_aa = agg::renderer_scanline_aa_solid<agg::renderer_base<DrawingType> >(base);
        render_bin = agg::renderer_scanline_bin_solid<agg::renderer_base<DrawingType> >(base);

        if (raster)
            delete raster;

        raster = new agg::rasterizer_scanline_aa<>();

        if (sl)
            delete sl;

        sl = new agg::scanline32_p8();
    }

    bool preserve(){
        return _preserve;
    }

    void preserve(bool p){
        _preserve = p;
    }

    // Render getter/setters
    bool antialias(){
        return _antialias;
    }

    void antialias(bool aa){
        _antialias = aa;
    }

    void line_width(double w){
        _linewidth = w;
    }

    double line_width(){
        return _linewidth;
    }

    void miter_limit(double m){
        _miterlimit = m;
    }

    double miter_limit(){
        return _miterlimit;
    }

    void line_join(line_join_style lj){
        _linejoin = lj;
    }

    line_join_style line_join(){
        return _linejoin;
    }

    void line_cap(line_cap_style lc){
        _linecap = lc;
    }

    line_cap_style line_cap(){
        return _linecap;
    }

    // Set the current pathid
    void active_path(unsigned p){
        pathid = p;
    }

    // Get the current pathid;
    unsigned active_path(){
        return pathid;
    }

    // Start new path, returns new pathid
    unsigned new_path(){
        if (!raster){
            return 0;
        }

        raster->reset();
        pathid = start_new_path();
        all_paths.push_back(pathid);
        return pathid;
    }

    // Set rotation
    inline void rotate(double rad){
        mtx *= agg::trans_affine_rotation(rad);
    }

    // Set translation
    inline void translate(double x, double y){
        mtx *= agg::trans_affine_translation(x, y);
    }

    // Set scale
    inline void scale(double n){
        mtx *= agg::trans_affine_scaling(n);
    }

    inline void scale(double n, double m){
        mtx *= agg::trans_affine_scaling(n, m);
    }

    // Set skew
    void skew(double x, double y){
        mtx *= agg::trans_affine_skewing(x, y);
    }

    // Clear all transformations
    inline void clear_transforms(){
        mtx.reset();
    }

    // End the current polygon and begin a new one
    inline void end_polygon(){
        end_poly();
    }

    // Reset everything
    void reset(){
        if (!raster){
            return;
        }
        _linewidth = 1;
        _miterlimit = 1;
        _preserve = false;
        raster->reset();
        reset_clip();
        clear_transforms();
        remove_all();
    }

    // Clear drawing context with specific rgba color
    inline void clear(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
        base.clear(Color(r, g, b, a));
    }

    inline void clear(uint8_t r, uint8_t g, uint8_t b){
        base.clear(Color(r, g, b));
    }

    // Create shape objects
    void ellipse(double x, double y, double rx, double ry, int n = 100){
        agg::ellipse ell(x, y, rx, ry, n);
        concat_path<agg::ellipse>(ell);
    }

    inline void rect(double x, double y, double x1, double y1, double border_radius=0){
        agg::rounded_rect r(x, y, x1, y1, border_radius);
        concat_path(r);
    }

    inline void clip(double x, double y, double x1, double y1){
        if (!raster){
            return;
        }
        raster->clip_box(x, y, x1, y1);
    }

    inline void reset_clip(){
        if (!raster){
            return;
        }
        raster->reset_clipping();
    }

    inline void filling_rule(filling_rule fr){
        if (!raster){
            return;
        }
        raster->filling_rule((agg::filling_rule_e)fr);
    }

    // Get previous position
    inline void last_position(double *x, double* y){
        if (x){
            *x = last_x();
        }

        if (y){
            *y = last_y();
        }
    }

    inline void curve_to(double x, double y){
        curve3(x, y);
    }

    inline void curve_rel(double x, double y){
        curve3_rel(x, y);
    }

    inline void curve_to(double x, double y, double a, double b){
        curve3(a, b, x, y);
    }

    inline void curve_rel(double x, double y, double a, double b){
        curve3_rel(a, b, x, y);
    }

    inline void curve_to(double x, double y, double a, double b, double c, double d){
        curve4(a, b, c, d, x, y);
    }

    inline void curve_rel(double x, double y, double a, double b, double c, double d){
        curve4_rel(a, b, c, d, x, y);
    }

    inline void arc_to(double rx, double ry, double x, double y, double angle, bool large = false, bool sweep = false){
        if (angle == 0)
            return;

        agg::path_storage::arc_to(rx, ry, angle, large, sweep, x, y);
    }

    inline void arc_rel( double rx, double ry, double x, double y, double angle, bool large = false, bool sweep = false){
        agg::path_storage::arc_rel(rx, ry, angle, large, sweep, x, y);
    }

    /* A list of important functions provided directly by agg::path_storage
     * move_to
     * move_rel
     * line_to
     * line_rel
     * hline_to
     * hline_rel
     * vline_to
     * vline_rel
     * arc_to
     * arc_rel
     * curve3
     * curve3_rel
     * curve4
     * curve4_rel
     * rel_to_abs
     * last_x
     * last_y
     * modify_vertex
     * modify_command
     * swap_vertices
     * total_vertices
     * vertex
     * command
     * vertex
     * flip_x
     * flip_y
     * translate_all_paths
     */

    template <typename ColorType>
    inline void blur(double a){
        agg::recursive_blur<ColorType, agg::recursive_blur_calc_rgb<> > sb;
        sb.blur(pix, a);
    }

    // Draw text without freetype
    double text_simple(double x, double y, const char *txt, int size=50, double width=2.0, const char *font=NULL, bool flip_y = true){
        if (!raster){
            return 0.0;
        }
        agg::gsv_text text;
        agg::gsv_text_outline<agg::trans_affine> outline(text, mtx);

        text.text(txt);
        text.flip(flip_y);
        text.size(size);
        text.start_point(x, y);
        outline.width(width);

        if (font){
            text.load_font(font);
        }

        raster->add_path(outline);
        return text.text_width();
    }

    // Copy from another drawing of the same type
    void copy_from(Drawing<DrawingType> &d){
        buffer.copy_from(d.buffer);
    }

    // Set the current rendering color
    inline void set_color(Color c){
        render_aa.color(c);
        render_bin.color(c);
    }
    
    inline void set_fill_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
      fill_color = Color(r, g, b, a);
    }
    
    inline void set_stroke_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
      stroke_color = Color(r, g, b, a);
    }

    inline void set_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a=255){
        set_color(Color(r, g, b, a));
    }

    void fill(){
        set_color(fill_color);
        agg::conv_curve<agg::path_storage> pth(*this);
        agg::conv_transform<agg::conv_curve<agg::path_storage> > m(pth, mtx);
        paint(m);
    }

    void stroke(){
        set_color(stroke_color);
        agg::conv_curve<agg::path_storage> p(*this);
        agg::conv_stroke<agg::conv_curve<agg::path_storage> > pth(p);

        pth.width(_linewidth);
        pth.line_cap((agg::line_cap_e)_linecap);
        pth.line_join((agg::line_join_e)_linejoin);
        pth.miter_limit(_miterlimit);

        agg::conv_transform<agg::conv_stroke<agg::conv_curve<agg::path_storage> > > m(pth, mtx);
        paint(m);
    }

    inline void dash(Color c, double x, double y){
        set_color(c);
        dash(x, y);
    }

    void dash(double a, double b){
        agg::conv_curve<agg::path_storage> curve(*this);
        agg::conv_dash<agg::conv_curve<agg::path_storage> > p(curve);
        p.add_dash(a, b);
        agg::conv_stroke<agg::conv_dash<agg::conv_curve<agg::path_storage> > > pth(p);
        pth.width(_linewidth);
        pth.line_cap((agg::line_cap_e)_linecap);
        pth.line_join((agg::line_join_e)_linejoin);
        pth.miter_limit(_miterlimit);
        agg::conv_transform<agg::conv_stroke<agg::conv_dash<agg::conv_curve<agg::path_storage> > > > m(pth, mtx);
        paint(m);
    }

    void paint(){
        paint(*this);
    }

    template <typename PathType>
    void paint(PathType &pth){
        if (!raster || !sl){
            return;
        }

        raster->add_path(pth, pathid);
        if (_antialias){
            agg::render_scanlines(*raster, *sl, render_aa);
        } else {
            agg::render_scanlines(*raster, *sl, render_bin);
        }

        if (!_preserve){
            remove_all();
            clear_transforms();
        }
    }

    inline void auto_close(bool c){
        if (!raster){
            return;
        }
        raster->auto_close(c);
    }

    // Check if point is in rasterizer
    // note: perserve must be set to true for this to work
    inline bool is_drawn(double x, double y){
        if (!raster){
            return false;
        }
        return raster->hit_test(x, y);
    }

    // Check if current point is in current path
    inline bool in_path(double x, double y){
        double a, b;
        for(int i = 0; i < agg::path_storage::total_vertices(); i++){
            agg::path_storage::vertex(i, &a, &b);
            if (a == x && b == y)
                return true;
        }
        return false;
    }
};

// Drawing types
typedef Drawing<rgba32> DrawingRGBA32;
typedef Drawing<rgb24> DrawingRGB24;
typedef Drawing<rgba64> DrawingRGBA64;
typedef Drawing<rgb48> DrawingRGB48;
typedef Drawing<bgra32> DrawingBGRA32;
typedef Drawing<bgr24> DrawingBGR24;
typedef Drawing<bgra64> DrawingBGRA64;
typedef Drawing<bgr48> DrawingBGR48;

DrawingRGBA32 draw(int32_t w, int32_t h, uint8_t *data);
DrawingRGBA64 draw(int32_t w, int32_t h, uint16_t *data);
DrawingRGB24 draw_rgb(int32_t w, int32_t h, uint8_t *data);
DrawingRGB48 draw_rgb(int32_t w, int32_t h, uint16_t *data);


#endif // cplusplus

#ifdef __cplusplus
} // namesapce tw
#endif

#endif // TWOMBLY_DRAW_HEADER

