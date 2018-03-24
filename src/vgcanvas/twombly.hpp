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
#include <cstdint>

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
#include "agg_alpha_mask_u8.h"
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
    double _width; // linewidth
    double _miterlimit;
    line_cap_style _linecap;
    line_join_style _linejoin;
    bool owns_data, owns_alpha; // data ownership information
    unsigned pathid; // stores current path
    Color current_color;
public:
    agg::trans_affine mtx;
    uint8_t *alpha_mask;
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

    Drawing() : data(nullptr), pix(buffer), sl(nullptr), raster(nullptr), alpha_mask(nullptr), size(0, 0, 0) {
        base = agg::renderer_base<DrawingType>(pix);
        render_aa = agg::renderer_scanline_aa_solid<agg::renderer_base<DrawingType> >(base);
        render_bin = agg::renderer_scanline_bin_solid<agg::renderer_base<DrawingType> >(base);
    }

    Drawing<DrawingType>(int32_t w, int32_t h, int c, uint8_t *d=nullptr, uint8_t *_alpha_mask=nullptr) :
        data(d), 
        _antialias(true), 
        _preserve(false), 
        _width(1), 
        owns_data(d == nullptr), 
        owns_alpha(_alpha_mask == nullptr), 
        pathid(0), 
        alpha_mask(_alpha_mask), 
        buffer((uint8_t*)data, w, h, w * c), 
        raster(nullptr), 
        sl(nullptr), 
        pix(buffer), 
        size(w, h, c)
        {
        alloc();
    }

    Drawing<DrawingType>(int32_t w, int32_t h, int c, uint16_t *d=nullptr, uint8_t *_alpha_mask=nullptr) :
        data(d != nullptr ? (void*)d : calloc(w * h * c, sizeof(uint16_t))), buffer((uint8_t*)data, w, h, w * c * 2), pix(buffer), _antialias(true), _preserve(false), _width(1), pathid(0), raster(nullptr), sl(nullptr), size(w, h, c),  alpha_mask(_alpha_mask), owns_data(d == nullptr), owns_alpha(_alpha_mask == nullptr) {
        alloc();
    }


    ~Drawing(){
        if (raster){
            delete raster;
            raster = nullptr;
        }

        if (sl){
            delete sl;
            sl = nullptr;
        }

        if (owns_data){
            if (data) free(data);
            data = nullptr;
        }

        if (owns_alpha){
            alpha_mask_free();
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

    void alpha_mask_init(){
        if (alpha_mask == nullptr){
            alpha_mask = new uint8_t[size.x * size.y]();
            memset(alpha_mask, 255, size.x * size.y);
        }

    }

    void alpha_mask_fill(uint8_t a){
        if (alpha_mask){
            memset(alpha_mask, a, size.x * size.y);
        }
    }

    void alpha_mask_set(int32_t x, int32_t y, uint8_t val){
        if (alpha_mask && x < size.x && y < size.y && x >= 0 && y >= 0){
            alpha_mask[(y * size.x) + x] = val;
        }
    }

    uint8_t alpha_mask_get(int32_t x, int32_t y){
        if (!alpha_mask){
            return 0;
        }
        return alpha_mask[(y * size.x) + x];
    }

    uint8_t *alpha_mask_ptr_offs(int32_t x, int32_t y){
        if (!alpha_mask){
            return nullptr;
        }
        return alpha_mask + (y * size.x) + x;
    }

    uint8_t *alpha_mask_ptr(){
        return alpha_mask;
    }

    void alpha_mask_free(){
        if (alpha_mask){
            if (owns_alpha){
                delete[] alpha_mask;
            }
            alpha_mask = nullptr;
        }
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
        _width = w;
    }

    double line_width(){
        return _width;
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

    inline void invert_polygon(){
        agg::path_storage::invert_polygon(pathid);
    }

    template <typename PathType>
    void concat_polygon(PathType *data, size_t n = 0, bool closed = false){
        concat_poly(data, n ? n : TW_POLY_SIZE(data), closed);
    }

    template <typename PathType>
    void join_polygon(PathType *data, size_t n = 0, bool closed = false){
        join_poly(data,  n ? n : TW_POLY_SIZE(data), closed);
    }

    // Reset everything
    void reset(){
        if (!raster){
            return;
        }
        _width = 1;
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
    double text_simple(double x, double y, const char *txt, int size=50, double width=2.0, const char *font=nullptr, bool flip_y = true){
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
        current_color = c;
    }

    inline void set_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a=255){
        set_color(Color(r, g, b, a));
    }

    inline void fill(Color c){
        set_color(c);
        fill();
    }

    void fill(){
        agg::conv_curve<agg::path_storage> pth(*this);
        agg::conv_transform<agg::conv_curve<agg::path_storage> > m(pth, mtx);
        paint(m);
    }

    inline void stroke(Color c){
        set_color(c);
        stroke();
    }

    void stroke(){
        agg::conv_curve<agg::path_storage> p(*this);
        agg::conv_stroke<agg::conv_curve<agg::path_storage> > pth(p);

        pth.width(_width);
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
        pth.width(_width);
        pth.line_cap((agg::line_cap_e)_linecap);
        pth.line_join((agg::line_join_e)_linejoin);
        pth.miter_limit(_miterlimit);
        agg::conv_transform<agg::conv_stroke<agg::conv_dash<agg::conv_curve<agg::path_storage> > > > m(pth, mtx);
        paint(m);
    }

    template <typename ColorType, typename GradientType>
    void stroke_gradient(Gradient<ColorType> grad, int s, int x){
        grad.generate();
        stroke_gradient<ColorType, GradientType>(grad.arr, s, x, grad.mtx);
    }


    template <typename ColorType, typename GradientType>
    void fill_gradient(Gradient<ColorType> grad, int s, int x){
        grad.generate();
        fill_gradient<ColorType, GradientType>(grad.arr, s, x, grad.mtx);
    }

    template <typename ColorType>
    void stroke_gradient(Gradient<ColorType> grad, int s, int x, gradient_type grad_type){
        grad.generate();
        switch(grad_type){
            case gradient_type_circle:
                stroke_gradient<ColorType, agg::gradient_circle>(grad, s, x);
                break;
            case gradient_type_radial:
                stroke_gradient<ColorType, agg::gradient_radial>(grad, s, x);
                break;
            case gradient_type_radial_d:
                stroke_gradient<ColorType, agg::gradient_radial_d>(grad, s, x);
                break;
            case gradient_type_radial_focus:
                stroke_gradient<ColorType, agg::gradient_radial_focus>(grad, s, x);
                break;
            case gradient_type_x:
                stroke_gradient<ColorType, agg::gradient_x>(grad, s, x);
                break;
            case gradient_type_y:
                stroke_gradient<ColorType, agg::gradient_y>(grad, s, x);
                break;
            case gradient_type_diamond:
                stroke_gradient<ColorType, agg::gradient_diamond>(grad, s, x);
                break;
            case gradient_type_xy:
                stroke_gradient<ColorType, agg::gradient_xy>(grad, s, x);
                break;
            case gradient_type_sqrt_xy:
                stroke_gradient<ColorType, agg::gradient_sqrt_xy>(grad, s, x);
                break;
            case gradient_type_conic:
                stroke_gradient<ColorType, agg::gradient_conic>(grad, s, x);
                break;
        }
    }

    template <typename ColorType>
    void fill_gradient(Gradient<ColorType> grad, int s, int x, gradient_type grad_type){
        grad.generate();
        switch(grad_type){
            case gradient_type_circle:
                fill_gradient<ColorType, agg::gradient_circle>(grad, s, x);
                break;
            case gradient_type_radial:
                fill_gradient<ColorType, agg::gradient_radial>(grad, s, x);
                break;
            case gradient_type_radial_d:
                fill_gradient<ColorType, agg::gradient_radial_d>(grad, s, x);
                break;
            case gradient_type_radial_focus:
                fill_gradient<ColorType, agg::gradient_radial_focus>(grad, s, x);
                break;
            case gradient_type_x:
                fill_gradient<ColorType, agg::gradient_x>(grad, s, x);
                break;
            case gradient_type_y:
                fill_gradient<ColorType, agg::gradient_y>(grad, s, x);
                break;
            case gradient_type_diamond:
                fill_gradient<ColorType, agg::gradient_diamond>(grad, s, x);
                break;
            case gradient_type_xy:
                fill_gradient<ColorType, agg::gradient_xy>(grad, s, x);
                break;
            case gradient_type_sqrt_xy:
                fill_gradient<ColorType, agg::gradient_sqrt_xy>(grad, s, x);
                break;
            case gradient_type_conic:
                fill_gradient<ColorType, agg::gradient_conic>(grad, s, x);
                break;
        }
    }

    template<typename ColorType, typename GradientType>
    void fill_gradient(agg::pod_auto_array<ColorType, 256> color_array, int s, int x, agg::trans_affine _mtx=agg::trans_affine()){
        if (!raster || !sl){
            return;
        }
        typedef agg::pod_auto_array<ColorType, 256> color_array_type;
        typedef agg::span_interpolator_linear<> interpolator_type;
        typedef agg::span_allocator<ColorType> span_allocator_type;
        typedef agg::span_gradient<ColorType, interpolator_type, GradientType, color_array_type> span_gradient_type;
        typedef agg::pixfmt_amask_adaptor<DrawingType, agg::amask_no_clip_gray8> alpha_adaptor_type;
        typedef agg::renderer_base<alpha_adaptor_type> renderer_base_type;
        typedef agg::renderer_scanline_aa<renderer_base_type, span_allocator_type, span_gradient_type> renderer_gradient_type;
        typedef agg::renderer_scanline_bin<renderer_base_type, span_allocator_type, span_gradient_type> renderer_gradient_type_bin;

        GradientType  gradient_func;
        interpolator_type   span_interpolator(_mtx);
        span_allocator_type span_allocator;

        span_gradient_type span_gradient(span_interpolator,
                                         gradient_func,
                                         color_array,
                                         s, x);

        // transform
        agg::conv_curve<agg::path_storage> pth(*this);
        agg::conv_transform<agg::conv_curve<agg::path_storage> > m(pth, mtx);
        raster->add_path(m, pathid);

        if (alpha_mask != nullptr){
            agg::rendering_buffer alpha_mask_rbuf(alpha_mask, size.x, size.y, size.x);
            agg::amask_no_clip_gray8 alpha_mask(alpha_mask_rbuf);
            alpha_adaptor_type alpha_mask_adaptor(pix, alpha_mask);
            agg::renderer_base<alpha_adaptor_type> alpha_base(alpha_mask_adaptor);

            if (_antialias){
                renderer_gradient_type ren_gradient(alpha_base, span_allocator, span_gradient);
                agg::render_scanlines(*raster, *sl, ren_gradient);
            } else {
                renderer_gradient_type_bin ren_gradient(alpha_base, span_allocator, span_gradient);
                agg::render_scanlines(*raster, *sl, ren_gradient);
            }
         } else {
            if (_antialias){
                agg::renderer_scanline_aa<agg::renderer_base<DrawingType>, span_allocator_type, span_gradient_type> ren_gradient(base, span_allocator, span_gradient);
                agg::render_scanlines(*raster, *sl, ren_gradient);
            } else {
                agg::renderer_scanline_bin<agg::renderer_base<DrawingType>, span_allocator_type, span_gradient_type> ren_gradient(base, span_allocator, span_gradient);
                agg::render_scanlines(*raster, *sl, ren_gradient);
            }
        };

        if (!_preserve){
            remove_all();
            clear_transforms();
        }
    }


    template<typename ColorType, typename GradientType>
    void stroke_gradient(agg::pod_auto_array<ColorType, 256> color_array, int s, int x, agg::trans_affine _mtx=agg::trans_affine()){
        typedef agg::pod_auto_array<ColorType, 256> color_array_type;
        typedef agg::span_interpolator_linear<> interpolator_type;
        typedef agg::span_allocator<ColorType> span_allocator_type;
        typedef agg::span_gradient<ColorType, interpolator_type, GradientType, color_array_type> span_gradient_type;
        typedef agg::pixfmt_amask_adaptor<DrawingType, agg::amask_no_clip_gray8> alpha_adaptor_type;
        typedef agg::renderer_base<alpha_adaptor_type> renderer_base_type;
        typedef agg::renderer_scanline_aa<renderer_base_type, span_allocator_type, span_gradient_type> renderer_gradient_type;
        typedef agg::renderer_scanline_bin<renderer_base_type, span_allocator_type, span_gradient_type> renderer_gradient_type_bin;

        GradientType gradient_func;
        interpolator_type span_interpolator(_mtx);
        span_allocator_type span_allocator;
        span_gradient_type span_gradient(span_interpolator,
                                         gradient_func,
                                         color_array,
                                         s, x);

        // transform
        agg::conv_curve<agg::path_storage> p(*this);
        agg::conv_stroke<agg::conv_curve<agg::path_storage> > q(p);
        q.width(_width);
        q.line_cap((agg::line_cap_e)_linecap);
        q.line_join((agg::line_join_e)_linejoin);
        q.miter_limit(_miterlimit);
        agg::conv_transform<agg::conv_stroke<agg::conv_curve<agg::path_storage> > > m(q, mtx);
        raster->add_path(m, pathid);

        if (alpha_mask != nullptr){
            agg::rendering_buffer alpha_mask_rbuf(alpha_mask, size.x, size.y, size.x);
            agg::amask_no_clip_gray8 alpha_mask(alpha_mask_rbuf);
            alpha_adaptor_type alpha_mask_adaptor(pix, alpha_mask);
            agg::renderer_base<alpha_adaptor_type> alpha_base(alpha_mask_adaptor);

            if (_antialias){
                renderer_gradient_type ren_gradient(alpha_base, span_allocator, span_gradient);
                agg::render_scanlines(*raster, *sl, ren_gradient);
            } else {
                renderer_gradient_type_bin ren_gradient(alpha_base, span_allocator, span_gradient);
                agg::render_scanlines(*raster, *sl, ren_gradient);
            }
         } else {
            if (_antialias){
                agg::renderer_scanline_aa<agg::renderer_base<DrawingType>, span_allocator_type, span_gradient_type> ren_gradient(base, span_allocator, span_gradient);
                agg::render_scanlines(*raster, *sl, ren_gradient);
            } else {
                agg::renderer_scanline_bin<agg::renderer_base<DrawingType>, span_allocator_type, span_gradient_type> ren_gradient(base, span_allocator, span_gradient);
                agg::render_scanlines(*raster, *sl, ren_gradient);
            }
        };

        if (!_preserve){
            remove_all();
            clear_transforms();
        }

    }

    void paint(){
        paint(*this);
    }

    template <typename PathType>
    void paint(PathType &pth){
        if (!raster || !sl){
            return;
        }
        typedef agg::pixfmt_amask_adaptor<DrawingType, agg::amask_no_clip_gray8> alpha_adaptor_type;
        raster->add_path(pth, pathid);
        if (alpha_mask != nullptr){
            agg::rendering_buffer alpha_mask_rbuf(alpha_mask, size.x, size.y, size.x);
            agg::amask_no_clip_gray8 alpha_mask(alpha_mask_rbuf);
            alpha_adaptor_type alpha_mask_adaptor(pix, alpha_mask);
            agg::renderer_base<alpha_adaptor_type> alpha_base(alpha_mask_adaptor);
            if (_antialias){
                agg::renderer_scanline_aa_solid<agg::renderer_base<alpha_adaptor_type> > alpha_ren(alpha_base);
                alpha_ren.color(current_color);
                agg::render_scanlines(*raster, *sl, alpha_ren);
            } else {
                agg::renderer_scanline_bin_solid<agg::renderer_base<alpha_adaptor_type> > alpha_ren(alpha_base);
                alpha_ren.color(current_color);
                agg::render_scanlines(*raster, *sl, alpha_ren);
            }
         } else {
            if (_antialias){
                agg::render_scanlines(*raster, *sl, render_aa);
            } else {
                agg::render_scanlines(*raster, *sl, render_bin);
            }
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

