/* Picasso - a vector graphics library
 *
 * Copyright (C) 2013 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#include <stdio.h>
#include "common.h"
#include "convert.h"
#include "gfx_font_adapter.h"
#include "gfx_rasterizer_scanline.h"
#include "gfx_scanline.h"
#include "gfx_scanline_renderer.h"
#include "gfx_scanline_storage.h"
#include "gfx_trans_affine.h"

#if ENABLE(FREE_TYPE2)
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include "graphic_path.h"
#include "graphic_helper.h"
#include "graphic_base.h"

namespace gfx {

extern FT_Library g_library;
extern char * _font_by_name(const char* face);

class font_adapter_impl
{
public:
    font_adapter_impl()
        : font(0)
        , antialias(false)
        , flip_y(false)
        , hinting(false)
        , weight(0)
        , height(0)
        , ascent(0)
        , descent(0)
        , leading(0)
        , units_per_em(0)
        , library(g_library)
        , cur_glyph_index(0)
        , cur_data_size(0)
        , cur_data_type(glyph_type_outline)
        , cur_bound_rect(0,0,0,0)
        , cur_advance_x(0)
        , cur_advance_y(0)

    {
    }

    ~font_adapter_impl()
    {
        if (font) {
            FT_Done_Face(font);
            font = 0;
        }
    }

    FT_Face font;
    bool antialias;
    bool flip_y;
    bool hinting;
    scalar weight;
    gfx_trans_affine matrix;
    scalar height;
    scalar ascent;
    scalar descent;
    scalar leading;
    unsigned int units_per_em;
    //font special
    FT_Library library;
    //current glyph
    unsigned int cur_glyph_index;
    unsigned int cur_data_size;
    glyph_type cur_data_type;
    rect cur_bound_rect;
    scalar cur_advance_x;
    scalar cur_advance_y;
    picasso::graphic_path cur_font_path;
    gfx_scanline_storage_bin cur_font_scanlines_bin;
    gfx_serialized_scanlines_adaptor_bin cur_font_storage_bin;
};

gfx_font_adapter::gfx_font_adapter(const char* name, int charset, scalar height, scalar weight,
                                bool italic, bool hint, bool flip, bool a, const abstract_trans_affine* mtx)
    :m_impl(new font_adapter_impl)
{
    FT_Encoding char_set = (charset == charset_latin) ? FT_ENCODING_NONE : FT_ENCODING_UNICODE;
    m_impl->antialias = a;
    m_impl->flip_y = flip;
    m_impl->hinting = hint;
    m_impl->weight = weight;
    int error = FT_New_Face(m_impl->library, _font_by_name(name), 0, &m_impl->font);
    if ((error == 0) && m_impl->font) {
        FT_Set_Pixel_Sizes(m_impl->font, 0, uround(height*FLT_TO_SCALAR(64.0f))>>6);
        FT_Select_Charmap(m_impl->font, char_set);
    }
    m_impl->matrix = *static_cast<gfx_trans_affine*>(const_cast<abstract_trans_affine*>(mtx));
    if (italic)
        m_impl->matrix.shear(-0.4f, 0.0f);

    m_impl->height = height;

    if (m_impl->font) {
        scalar top_base = FLT_TO_SCALAR(fabsf(m_impl->font->ascender * m_impl->height / m_impl->font->height));
        scalar top_leading = FLT_TO_SCALAR((m_impl->font->height -
              (abs(m_impl->font->ascender) + abs(m_impl->font->descender))) * m_impl->height / m_impl->font->height);
        m_impl->ascent = top_base + top_leading;
        m_impl->descent = FLT_TO_SCALAR(fabs(m_impl->font->descender * m_impl->height / m_impl->font->height));
        m_impl->leading = FLT_TO_SCALAR((m_impl->font->size->metrics.height -
                    m_impl->font->size->metrics.ascender+m_impl->font->size->metrics.descender)/64.0);
        m_impl->units_per_em = m_impl->font->units_per_EM;
    }
}

gfx_font_adapter::~gfx_font_adapter()
{

    delete m_impl;
}

void gfx_font_adapter::active(void)
{
}

void gfx_font_adapter::deactive(void)
{
}

static inline float int26p6_to_flt(int p)
{
    return float(p) / 64.0f;
}

void gfx_font_adapter::add_kerning(unsigned int first, unsigned int second, scalar* x, scalar* y)
{
    if (m_impl->font && first && second && FT_HAS_KERNING(m_impl->font)) {
        FT_Vector delta;
        FT_Get_Kerning(m_impl->font, first, second, FT_KERNING_DEFAULT, &delta);
        scalar dx = int26p6_to_flt(delta.x);
        scalar dy = int26p6_to_flt(delta.y);
        if (m_impl->font->glyph->format != FT_GLYPH_FORMAT_BITMAP)
            m_impl->matrix.transform_2x2(&dx, &dy);
        *x += dx;
        *y += dy;
    }
}

static bool decompose_ft_outline(const FT_Outline& outline,
                              bool flip_y, const gfx_trans_affine& mtx, graphic_path& path)
{
    FT_Vector   v_last;
    FT_Vector   v_control;
    FT_Vector   v_start;
    float x1, y1, x2, y2, x3, y3;

    FT_Vector*  point;
    FT_Vector*  limit;
    char*       tags;

    int   first;     // index of first point in contour
    char  tag;       // current point's state

    first = 0;

    for (int n = 0; n < outline.n_contours; n++) {
        int  last;  // index of last point in contour

        last  = outline.contours[n];
        limit = outline.points + last;

        v_start = outline.points[first];
        v_last  = outline.points[last];

        v_control = v_start;

        point = outline.points + first;
        tags  = outline.tags  + first;
        tag   = FT_CURVE_TAG(tags[0]);

        // A contour cannot start with a cubic control point!
        if (tag == FT_CURVE_TAG_CUBIC)
            return false;

        // check first point to determine origin
        if (tag == FT_CURVE_TAG_CONIC) {
            // first point is conic control.
            if (FT_CURVE_TAG(outline.tags[last]) == FT_CURVE_TAG_ON) {
                // start at last point if it is on the curve
                v_start = v_last;
                limit--;
            } else {
                // if both first and last points are conic,
                // start at their middle and record its position
                // for closure
                v_start.x = (v_start.x + v_last.x) / 2;
                v_start.y = (v_start.y + v_last.y) / 2;

                v_last = v_start;
            }
            point--;
            tags--;
        }

        x1 = int26p6_to_flt(v_start.x);
        y1 = int26p6_to_flt(v_start.y);

        if (flip_y)
            y1 = -y1;

        mtx.transform(&x1, &y1);
        path.move_to(FLT_TO_SCALAR(x1), FLT_TO_SCALAR(y1));

        while (point < limit) {
            point++;
            tags++;

            tag = FT_CURVE_TAG(tags[0]);
            switch (tag) {
                case FT_CURVE_TAG_ON:  // emit a single line_to
                    {
                        x1 = int26p6_to_flt(point->x);
                        y1 = int26p6_to_flt(point->y);
                        if (flip_y)
                            y1 = -y1;
                        mtx.transform(&x1, &y1);
                        path.line_to(FLT_TO_SCALAR(x1), FLT_TO_SCALAR(y1));
                        continue;
                    }
                case FT_CURVE_TAG_CONIC:  // consume conic arcs
                    {
                        v_control.x = point->x;
                        v_control.y = point->y;
                    Do_Conic:
                        if (point < limit) {
                            FT_Vector vec;
                            FT_Vector v_middle;

                            point++;
                            tags++;
                            tag = FT_CURVE_TAG(tags[0]);

                            vec.x = point->x;
                            vec.y = point->y;

                            if (tag == FT_CURVE_TAG_ON) {
                                x1 = int26p6_to_flt(v_control.x);
                                y1 = int26p6_to_flt(v_control.y);
                                x2 = int26p6_to_flt(vec.x);
                                y2 = int26p6_to_flt(vec.y);
                                if (flip_y) {
                                    y1 = -y1;
                                    y2 = -y2;
                                }
                                mtx.transform(&x1, &y1);
                                mtx.transform(&x2, &y2);
                                path.curve3(FLT_TO_SCALAR(x1),
                                            FLT_TO_SCALAR(y1),
                                            FLT_TO_SCALAR(x2),
                                            FLT_TO_SCALAR(y2));
                                continue;
                            }

                            if (tag != FT_CURVE_TAG_CONIC)
                                return false;

                            v_middle.x = (v_control.x + vec.x) / 2;
                            v_middle.y = (v_control.y + vec.y) / 2;

                            x1 = int26p6_to_flt(v_control.x);
                            y1 = int26p6_to_flt(v_control.y);
                            x2 = int26p6_to_flt(v_middle.x);
                            y2 = int26p6_to_flt(v_middle.y);
                            if (flip_y) {
                                y1 = -y1;
                                y2 = -y2;
                            }
                            mtx.transform(&x1, &y1);
                            mtx.transform(&x2, &y2);
                            path.curve3(FLT_TO_SCALAR(x1),
                                        FLT_TO_SCALAR(y1),
                                        FLT_TO_SCALAR(x2),
                                        FLT_TO_SCALAR(y2));
                            v_control = vec;
                            goto Do_Conic;
                        }

                        x1 = int26p6_to_flt(v_control.x);
                        y1 = int26p6_to_flt(v_control.y);
                        x2 = int26p6_to_flt(v_start.x);
                        y2 = int26p6_to_flt(v_start.y);
                        if (flip_y) {
                            y1 = -y1;
                            y2 = -y2;
                        }
                        mtx.transform(&x1, &y1);
                        mtx.transform(&x2, &y2);
                        path.curve3(FLT_TO_SCALAR(x1),
                                    FLT_TO_SCALAR(y1),
                                    FLT_TO_SCALAR(x2),
                                    FLT_TO_SCALAR(y2));
                        goto Close;
                    }
                default:  // FT_CURVE_TAG_CUBIC
                    {
                        FT_Vector vec1, vec2;

                        if (point + 1 > limit || FT_CURVE_TAG(tags[1]) != FT_CURVE_TAG_CUBIC) {
                            return false;
                        }

                        vec1.x = point[0].x;
                        vec1.y = point[0].y;
                        vec2.x = point[1].x;
                        vec2.y = point[1].y;

                        point += 2;
                        tags  += 2;

                        if (point <= limit) {
                            FT_Vector vec;

                            vec.x = point->x;
                            vec.y = point->y;

                            x1 = int26p6_to_flt(vec1.x);
                            y1 = int26p6_to_flt(vec1.y);
                            x2 = int26p6_to_flt(vec2.x);
                            y2 = int26p6_to_flt(vec2.y);
                            x3 = int26p6_to_flt(vec.x);
                            y3 = int26p6_to_flt(vec.y);
                            if (flip_y) {
                                y1 = -y1;
                                y2 = -y2;
                                y3 = -y3;
                            }
                            mtx.transform(&x1, &y1);
                            mtx.transform(&x2, &y2);
                            mtx.transform(&x3, &y3);
                            path.curve4(FLT_TO_SCALAR(x1),
                                        FLT_TO_SCALAR(y1),
                                        FLT_TO_SCALAR(x2),
                                        FLT_TO_SCALAR(y2),
                                        FLT_TO_SCALAR(x3),
                                        FLT_TO_SCALAR(y3));
                            continue;
                        }

                        x1 = int26p6_to_flt(vec1.x);
                        y1 = int26p6_to_flt(vec1.y);
                        x2 = int26p6_to_flt(vec2.x);
                        y2 = int26p6_to_flt(vec2.y);
                        x3 = int26p6_to_flt(v_start.x);
                        y3 = int26p6_to_flt(v_start.y);
                        if (flip_y) {
                            y1 = -y1;
                            y2 = -y2;
                            y3 = -y3;
                        }
                        mtx.transform(&x1, &y1);
                        mtx.transform(&x2, &y2);
                        mtx.transform(&x3, &y3);
                        path.curve4(FLT_TO_SCALAR(x1),
                                    FLT_TO_SCALAR(y1),
                                    FLT_TO_SCALAR(x2),
                                    FLT_TO_SCALAR(y2),
                                    FLT_TO_SCALAR(x3),
                                    FLT_TO_SCALAR(y3));
                        goto Close;
                    }
            }
        }

        path.close_polygon();
Close:
        first = last + 1;
    }
    return true;
}

static rect get_bounding_rect(graphic_path& path)
{
    rect_s rc(0,0,0,0);
    bounding_rect(path, 0, &rc.x1, &rc.y1, &rc.x2, &rc.y2);
    return rect((int)Floor(rc.x1), (int)Floor(rc.y1), (int)Ceil(rc.x2), (int)Ceil(rc.y2));
}

static void decompose_ft_bitmap_mono(const FT_Bitmap& bitmap, int x, int y,
                            bool flip_y, gfx_scanline_bin& sl, gfx_scanline_storage_bin& storage)
{
    const byte* buf = (const byte*)bitmap.buffer;
    int pitch = bitmap.pitch;
    sl.reset(x, x + bitmap.width);
    storage.prepare();
    if (flip_y) {
        buf += bitmap.pitch * (bitmap.rows - 1);
        y += bitmap.rows;
        pitch = -pitch;
    }

    for (int i = 0; i < (int)bitmap.rows; i++) {
        sl.reset_spans();
        bitset_iterator bits(buf, 0);
        for (int j = 0; j < (int)bitmap.width; j++) {
            if (bits.bit()) sl.add_cell(x + j, cover_full)
                ; //do nothing
            ++bits;
        }
        buf += pitch;
        if (sl.num_spans()) {
            sl.finalize(y - i - 1);
            storage.render(sl);
        }
    }
}

bool gfx_font_adapter::prepare_glyph(unsigned int code)
{
    if (m_impl->font) {
        m_impl->cur_glyph_index = FT_Get_Char_Index(m_impl->font, code);

        int error = FT_Load_Glyph(m_impl->font, m_impl->cur_glyph_index,
                                 m_impl->hinting ? FT_LOAD_DEFAULT : FT_LOAD_NO_HINTING);

        bool is_sys_bitmap = false;
        if (m_impl->font->glyph->format == FT_GLYPH_FORMAT_BITMAP)
            is_sys_bitmap = true;

        if (error == 0) {
            if (m_impl->antialias && !is_sys_bitmap) {
                if (m_impl->weight == 500) {
                    int strength = 1 << 5;
                    FT_Outline_Embolden(&(m_impl->font->glyph->outline), strength);
                } else if (m_impl->weight == 700) {
                    int strength = 1 << 6;
                    FT_Outline_Embolden(&(m_impl->font->glyph->outline), strength);
                } else if (m_impl->weight == 900) {
                    int strength = 1 << 7;
                    FT_Outline_Embolden(&(m_impl->font->glyph->outline), strength);
                }
                // outline text
                m_impl->cur_data_type = glyph_type_outline;
                m_impl->cur_font_path.remove_all();

                if (decompose_ft_outline(m_impl->font->glyph->outline,
                            m_impl->flip_y, m_impl->matrix, m_impl->cur_font_path))
                {
                    m_impl->cur_bound_rect = get_bounding_rect(m_impl->cur_font_path);
                    m_impl->cur_data_size = m_impl->cur_font_path.total_byte_size()+sizeof(unsigned int);//count data
                    m_impl->cur_advance_x = FLT_TO_SCALAR(int26p6_to_flt(m_impl->font->glyph->advance.x));
                    m_impl->cur_advance_y = FLT_TO_SCALAR(int26p6_to_flt(m_impl->font->glyph->advance.y));
                    m_impl->matrix.transform(&m_impl->cur_advance_x, &m_impl->cur_advance_y);
                    return true;
                }
            } else {
                m_impl->cur_data_type = glyph_type_mono;
                if (is_sys_bitmap || !FT_IS_SCALABLE(m_impl->font) || m_impl->matrix.is_identity()) {
                    gfx_scanline_bin sl;
                    error = FT_Render_Glyph(m_impl->font->glyph, FT_RENDER_MODE_MONO);
                    if (error == 0) {
                        decompose_ft_bitmap_mono(m_impl->font->glyph->bitmap,
                           m_impl->font->glyph->bitmap_left, m_impl->flip_y ? -m_impl->font->glyph->bitmap_top :
                           m_impl->font->glyph->bitmap_top, m_impl->flip_y, sl, m_impl->cur_font_scanlines_bin);

                        m_impl->cur_bound_rect = rect(m_impl->cur_font_scanlines_bin.min_x(),
                                m_impl->cur_font_scanlines_bin.min_y(),
                                m_impl->cur_font_scanlines_bin.max_x() + 1,
                                m_impl->cur_font_scanlines_bin.max_y() + 1);
                        m_impl->cur_data_size = m_impl->cur_font_scanlines_bin.byte_size();
                        m_impl->cur_advance_x = FLT_TO_SCALAR(int26p6_to_flt(m_impl->font->glyph->advance.x));
                        m_impl->cur_advance_y = FLT_TO_SCALAR(int26p6_to_flt(m_impl->font->glyph->advance.y));
                        return true;
                    }
                } else {
                    if (m_impl->weight == 500) {
                        int strength = 1 << 5;
                        FT_Outline_Embolden(&(m_impl->font->glyph->outline), strength);
                    } else if (m_impl->weight == 700) {
                        int strength = 1 << 6;
                        FT_Outline_Embolden(&(m_impl->font->glyph->outline), strength);
                    } else if (m_impl->weight == 900) {
                        int strength = 1 << 7;
                        FT_Outline_Embolden(&(m_impl->font->glyph->outline), strength);
                    }

                    m_impl->cur_font_path.remove_all();
                    if (decompose_ft_outline(m_impl->font->glyph->outline,
                                m_impl->flip_y, m_impl->matrix, m_impl->cur_font_path))
                    {
                        gfx_rasterizer_scanline_aa<> rasterizer;
                        picasso::conv_curve curves(m_impl->cur_font_path);
                        curves.approximation_scale(4.0);
                        rasterizer.add_path(curves);

                        gfx_scanline_bin sl;
                        m_impl->cur_font_scanlines_bin.prepare(); // Remove all
                        gfx_render_scanlines(rasterizer, sl, m_impl->cur_font_scanlines_bin);
                        m_impl->cur_bound_rect = rect(m_impl->cur_font_scanlines_bin.min_x(),
                                m_impl->cur_font_scanlines_bin.min_y(),
                                m_impl->cur_font_scanlines_bin.max_x() + 1,
                                m_impl->cur_font_scanlines_bin.max_y() + 1);
                        m_impl->cur_data_size = m_impl->cur_font_scanlines_bin.byte_size();
                        m_impl->cur_advance_x = FLT_TO_SCALAR(int26p6_to_flt(m_impl->font->glyph->advance.x));
                        m_impl->cur_advance_y = FLT_TO_SCALAR(int26p6_to_flt(m_impl->font->glyph->advance.y));
                        m_impl->matrix.transform(&m_impl->cur_advance_x, &m_impl->cur_advance_y);
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void gfx_font_adapter::write_glyph_to(byte* buffer)
{
    if (buffer && m_impl->cur_data_size) {
        if (m_impl->cur_data_type == glyph_type_outline) {
            unsigned int count = m_impl->cur_font_path.total_vertices();
            mem_copy(buffer, &count, sizeof(unsigned int));
            buffer += sizeof(unsigned int);
            m_impl->cur_font_path.serialize_to(buffer);
        } else { // mono glyph
            m_impl->cur_font_scanlines_bin.serialize(buffer);
        }
    }
}

void* gfx_font_adapter::create_storage(byte* buf, unsigned int len, scalar x, scalar y)
{
    m_impl->cur_font_storage_bin.init(buf, len, SCALAR_TO_FLT(Ceil(x)), SCALAR_TO_FLT(Ceil(y)));
    return (void*)&m_impl->cur_font_storage_bin;
}

void gfx_font_adapter::destroy_storage(void*)
{
    // do nothing
}

void gfx_font_adapter::translate_storage(void* storage, scalar x, scalar y)
{
    gfx_serialized_scanlines_adaptor_bin* sd = (gfx_serialized_scanlines_adaptor_bin*)storage;
    int ox = sd->x();
    int oy = sd->y();
    sd->setX(ox + SCALAR_TO_INT(Ceil(x)));
    sd->setY(oy + SCALAR_TO_INT(Ceil(y)));
}

scalar gfx_font_adapter::height(void) const
{
    return m_impl->height;
}

scalar gfx_font_adapter::ascent(void) const
{
    return m_impl->ascent;
}

scalar gfx_font_adapter::descent(void) const
{
    return m_impl->descent;
}

scalar gfx_font_adapter::leading(void) const
{
    return m_impl->leading;
}

unsigned int gfx_font_adapter::units_per_em(void) const
{
    return m_impl->units_per_em;
}
unsigned int gfx_font_adapter::glyph_index(void) const
{
    return m_impl->cur_glyph_index;
}

unsigned int gfx_font_adapter::data_size(void) const
{
    return m_impl->cur_data_size;
}

glyph_type gfx_font_adapter::data_type(void) const
{
    return m_impl->cur_data_type;
}

const rect& gfx_font_adapter::bounds(void) const
{
    return m_impl->cur_bound_rect;
}

scalar gfx_font_adapter::advance_x(void) const
{
    return m_impl->cur_advance_x;
}

scalar gfx_font_adapter::advance_y(void) const
{
    return m_impl->cur_advance_y;
}

}
#endif /* FREE_TYPE2 */
