/* Picasso - a vector graphics library
 *
 * Copyright (C) 2014 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#ifndef _GFX_SCANLINE_STORAGE_H_
#define _GFX_SCANLINE_STORAGE_H_

#include "common.h"
#include "data_vector.h"

namespace gfx {

// scanline cell storage
template <typename T>
class gfx_scanline_cell_storage
{
    typedef struct {
        T* ptr;
        unsigned int len;
    } extra_span;

public:
    typedef T value_type;

    ~gfx_scanline_cell_storage()
    {
        remove_all();
    }

    gfx_scanline_cell_storage()
        : m_cells(128-2)
    {
    }

    gfx_scanline_cell_storage(const gfx_scanline_cell_storage<T>& v)
        : m_cells(v.m_cells)
    {
        copy_extra_storage(v);
    }

    const gfx_scanline_cell_storage<T>&
    operator = (const gfx_scanline_cell_storage<T>& v)
    {
        if (this == &v)
            return *this;

        remove_all();
        m_cells = v.m_cells;
        copy_extra_storage(v);
        return *this;
    }

    void remove_all(void)
    {
        for (int i = m_extra_storage.size()-1; i >= 0; --i) {
            pod_allocator<T>::deallocate(m_extra_storage[i].ptr,
                                         m_extra_storage[i].len);
        }
        m_extra_storage.clear();
        m_cells.clear();
    }

    int add_cells(const T* cells, unsigned int num_cells)
    {
        int idx = m_cells.allocate_continuous_block(num_cells);
        if (idx >= 0) {
            T* ptr = &m_cells[idx];
            mem_copy(ptr, cells, sizeof(T) * num_cells);
            return idx;
        }
        extra_span s;
        s.len = num_cells;
        s.ptr = pod_allocator<T>::allocate(num_cells);
        mem_copy(s.ptr, cells, sizeof(T) * num_cells);
        m_extra_storage.add(s);
        return -(int)(m_extra_storage.size());
    }

    const T* operator [] (int idx) const
    {
        if (idx >= 0) {
            if ((unsigned int)idx >= m_cells.size())
                return 0;
            return &m_cells[(unsigned int)idx];
        }
        unsigned int i = (unsigned int)(-idx - 1);
        if (i >= m_extra_storage.size())
            return 0;
        return m_extra_storage[i].ptr;
    }

    T* operator [] (int idx)
    {
        if (idx >= 0) {
            if ((unsigned int)idx >= m_cells.size())
                return 0;
            return &m_cells[(unsigned int)idx];
        }
        unsigned int i = (unsigned int)(-idx - 1);
        if (i >= m_extra_storage.size())
            return 0;
        return m_extra_storage[i].ptr;
    }

private:
    void copy_extra_storage(const gfx_scanline_cell_storage<T>& v)
    {
        for (unsigned int i = 0; i < v.m_extra_storage.size(); ++i) {
            const extra_span& src = v.m_extra_storage[i];
            extra_span dst;
            dst.len = src.len;
            dst.ptr = pod_allocator<T>::allocate(dst.len);
            mem_copy(dst.ptr, src.ptr, dst.len * sizeof(T));
            m_extra_storage.add(dst);
        }
    }

    pod_bvector<T, 12> m_cells;
    pod_bvector<extra_span, 6> m_extra_storage;
};

// scanline storage antialias
template <typename T>
class gfx_scanline_storage_aa
{
public:
    typedef T cover_type;

    typedef struct {
        int32_t x;
        int32_t len; // If negative, it's a solid span, covers is valid
        int covers_id; // The index of the cells in the gfx_scanline_cell_storage
    } span_data;

    typedef struct {
        int y;
        unsigned int num_spans;
        unsigned int start_span;
    } scanline_data;

    // inner class for embeded scanline
    class embedded_scanline
    {
    public:

        class const_iterator
        {
        public:
            typedef struct {
                int32_t x;
                int32_t len; // If negative, it's a solid span, covers is valid
                const T* covers;
            } span;

            const_iterator() : m_storage(0), m_span_idx(0) { }
            const_iterator(const embedded_scanline& sl)
                : m_storage(sl.m_storage)
                , m_span_idx(sl.m_scanline.start_span)
            {
                init_span();
            }

            const span& operator*() const { return m_span; }
            const span* operator->() const { return &m_span; }

            void operator ++ ()
            {
                ++m_span_idx;
                init_span();
            }

        private:
            void init_span(void)
            {
                const span_data& s = m_storage->span_by_index(m_span_idx);
                m_span.x = s.x;
                m_span.len = s.len;
                m_span.covers = m_storage->covers_by_index(s.covers_id);
            }

            const gfx_scanline_storage_aa* m_storage;
            unsigned int m_span_idx;
            span m_span;
        };

        friend class const_iterator;

        embedded_scanline(const gfx_scanline_storage_aa& storage)
            : m_storage(&storage)
            , m_scanline_idx(0)
        {
            init(0);
        }

        void reset(int, int) { }
        unsigned int num_spans(void) const { return m_scanline.num_spans; }
        int y(void) const { return m_scanline.y; }
        const_iterator begin() const { return const_iterator(*this); }

        void init(unsigned int scanline_idx)
        {
            m_scanline_idx = scanline_idx;
            m_scanline = m_storage->scanline_by_index(m_scanline_idx);
        }

    private:
        const gfx_scanline_storage_aa* m_storage;
        unsigned int m_scanline_idx;
        scanline_data m_scanline;
    };

    // scanline storage
    gfx_scanline_storage_aa()
        : m_spans(256-2)         // block increment size
        , m_cur_scanline(0)
        , m_min_x(0x7FFFFFFF)
        , m_min_y(0x7FFFFFFF)
        , m_max_x(-0x7FFFFFFF)
        , m_max_y(-0x7FFFFFFF)
    {
        m_fake_scanline.y = 0;
        m_fake_scanline.num_spans = 0;
        m_fake_scanline.start_span = 0;
        m_fake_span.x = 0;
        m_fake_span.len = 0;
        m_fake_span.covers_id = 0;
    }

    // renderer interface
    void prepare(void)
    {
        m_covers.remove_all();
        m_scanlines.remove_all();
        m_spans.remove_all();
        m_cur_scanline = 0;
        m_min_x = 0x7FFFFFFF;
        m_min_y = 0x7FFFFFFF;
        m_max_x = -0x7FFFFFFF;
        m_max_y = -0x7FFFFFFF;
    }

    template <typename Scanline>
    void render(const Scanline& sl)
    {
        scanline_data sl_this;

        int y = sl.y();
        if (y < m_min_y) m_min_y = y;
        if (y > m_max_y) m_max_y = y;

        sl_this.y = y;
        sl_this.num_spans = sl.num_spans();
        sl_this.start_span = m_spans.size();
        typename Scanline::const_iterator span_iterator = sl.begin();

        unsigned int num_spans = sl_this.num_spans;
        for (;;) {
            span_data sp;
            sp.x = span_iterator->x;
            sp.len = span_iterator->len;
            int len = abs(int(sp.len));
            sp.covers_id = m_covers.add_cells(span_iterator->covers, (unsigned int)len);

            m_spans.add(sp);
            int x1 = sp.x;
            int x2 = sp.x + len - 1;
            if (x1 < m_min_x) m_min_x = x1;
            if (x2 > m_max_x) m_max_x = x2;
            if (--num_spans == 0) break;
            ++span_iterator;
        }
        m_scanlines.add(sl_this);
    }

    // iterate scanlines interface
    int min_x(void) const { return m_min_x; }
    int min_y(void) const { return m_min_y; }
    int max_x(void) const { return m_max_x; }
    int max_y(void) const { return m_max_y; }

    bool rewind_scanlines(void)
    {
        m_cur_scanline = 0;
        return m_scanlines.size() > 0;
    }

    template <typename Scanline>
    bool sweep_scanline(Scanline& sl)
    {
        sl.reset_spans();
        for (;;) {
            if (m_cur_scanline >= m_scanlines.size())
                return false;
            const scanline_data& sl_this = m_scanlines[m_cur_scanline];

            unsigned int num_spans = sl_this.num_spans;
            unsigned int span_idx = sl_this.start_span;
            do {
                const span_data& sp = m_spans[span_idx++];
                const T* covers = covers_by_index(sp.covers_id);
                if (sp.len < 0) {
                    sl.add_span(sp.x, unsigned(-sp.len), *covers);
                } else {
                    sl.add_cells(sp.x, sp.len, covers);
                }
            } while(--num_spans);

            ++m_cur_scanline;
            if (sl.num_spans()) {
                sl.finalize(sl_this.y);
                break;
            }
        }
        return true;
    }

    // specialization for embedded_scanline
    bool sweep_scanline(embedded_scanline& sl)
    {
        do {
            if (m_cur_scanline >= m_scanlines.size())
                return false;
            sl.init(m_cur_scanline);
            ++m_cur_scanline;
        } while(sl.num_spans() == 0);
        return true;
    }

    unsigned int byte_size(void) const
    {
        unsigned int size = sizeof(int32_t) * 4; // min_x, min_y, max_x, max_y

        for (unsigned int i = 0; i < m_scanlines.size(); ++i) {
            size += sizeof(int32_t) * 3; // scanline size in bytes, Y, num_spans

            const scanline_data& sl_this = m_scanlines[i];

            unsigned int num_spans = sl_this.num_spans;
            unsigned int span_idx  = sl_this.start_span;
            do {
                const span_data& sp = m_spans[span_idx++];

                size += sizeof(int32_t) * 2; // X, span_len
                if (sp.len < 0) {
                    size += sizeof(T); // cover
                } else {
                    size += sizeof(T) * (unsigned int)sp.len; // covers
                }
            } while(--num_spans);
        }
        return size;
    }

    static void write_int32(uint8_t* dst, int32_t val)
    {
        dst[0] = ((const uint8_t*)&val)[0];
        dst[1] = ((const uint8_t*)&val)[1];
        dst[2] = ((const uint8_t*)&val)[2];
        dst[3] = ((const uint8_t*)&val)[3];
    }

    void serialize(uint8_t* data) const
    {
        write_int32(data, min_x()); // min_x
        data += sizeof(int32_t);
        write_int32(data, min_y()); // min_y
        data += sizeof(int32_t);
        write_int32(data, max_x()); // max_x
        data += sizeof(int32_t);
        write_int32(data, max_y()); // max_y
        data += sizeof(int32_t);

        for (unsigned int i = 0; i < m_scanlines.size(); ++i) {
            const scanline_data& sl_this = m_scanlines[i];

            uint8_t* size_ptr = data;
            data += sizeof(int32_t);  // reserve space for scanline size in bytes

            write_int32(data, sl_this.y);  // Y
            data += sizeof(int32_t);

            write_int32(data, sl_this.num_spans);    // num_spans
            data += sizeof(int32_t);

            unsigned int num_spans = sl_this.num_spans;
            unsigned int span_idx = sl_this.start_span;
            do {
                const span_data& sp = m_spans[span_idx++];
                const T* covers = covers_by_index(sp.covers_id);

                write_int32(data, sp.x);            // X
                data += sizeof(int32_t);

                write_int32(data, sp.len);          // span_len
                data += sizeof(int32_t);

                if (sp.len < 0) {
                    mem_copy(data, covers, sizeof(T));
                    data += sizeof(T);
                } else {
                    mem_copy(data, covers, unsigned(sp.len) * sizeof(T));
                    data += sizeof(T) * unsigned(sp.len);
                }
            } while(--num_spans);
            write_int32(size_ptr, (int32_t)((unsigned int)(data - size_ptr)));
        }
    }

    const scanline_data& scanline_by_index(unsigned int i) const
    {
        return (i < m_scanlines.size()) ? m_scanlines[i] : m_fake_scanline;
    }

    const span_data& span_by_index(unsigned int i) const
    {
        return (i < m_spans.size()) ? m_spans[i] : m_fake_span;
    }

    const T* covers_by_index(int i) const
    {
        return m_covers[i];
    }

private:
    gfx_scanline_cell_storage<T> m_covers;
    picasso::pod_bvector<span_data, 10> m_spans;
    picasso::pod_bvector<scanline_data, 8> m_scanlines;
    span_data m_fake_span;
    scanline_data m_fake_scanline;
    unsigned int m_cur_scanline;
    int m_min_x;
    int m_min_y;
    int m_max_x;
    int m_max_y;
};

// scanline storage antialias
typedef gfx_scanline_storage_aa<uint8_t>  gfx_scanline_storage_aa_u8;


// serialized scanlines adaptor antialias
template <typename T>
class gfx_serialized_scanlines_adaptor_aa
{
public:
    typedef T cover_type;

    // inner class for embeded scanline
    class embedded_scanline
    {
    public:
        typedef T cover_type;

        class const_iterator
        {
        public:
            typedef struct {
                int32_t x;
                int32_t len; // If negative, it's a solid span, "covers" is valid
                const T* covers;
            } span;

            const_iterator() : m_ptr(0), m_dx(0) { }
            const_iterator(const embedded_scanline& sl)
                : m_ptr(sl.m_ptr)
                , m_dx(sl.m_dx)
            {
                init_span();
            }

            const span& operator*() const { return m_span;  }
            const span* operator->() const { return &m_span; }

            void operator ++ ()
            {
                if (m_span.len < 0) {
                    m_ptr += sizeof(T);
                } else {
                    m_ptr += m_span.len * sizeof(T);
                }
                init_span();
            }

        private:
            int read_int32(void)
            {
                int32_t val;
                ((uint8_t*)&val)[0] = *m_ptr++;
                ((uint8_t*)&val)[1] = *m_ptr++;
                ((uint8_t*)&val)[2] = *m_ptr++;
                ((uint8_t*)&val)[3] = *m_ptr++;
                return val;
            }

            void init_span(void)
            {
                m_span.x = read_int32() + m_dx;
                m_span.len = read_int32();
                m_span.covers = m_ptr;
            }

            const uint8_t* m_ptr;
            span m_span;
            int m_dx;
        };

        friend class const_iterator;

        embedded_scanline() : m_ptr(0), m_y(0), m_num_spans(0), m_dx(0) { }

        void reset(int, int) { }
        unsigned int num_spans(void) const { return m_num_spans; }
        int y(void) const { return m_y; }
        const_iterator begin(void) const { return const_iterator(*this); }

    private:
        int read_int32(void)
        {
            int32_t val;
            ((uint8_t*)&val)[0] = *m_ptr++;
            ((uint8_t*)&val)[1] = *m_ptr++;
            ((uint8_t*)&val)[2] = *m_ptr++;
            ((uint8_t*)&val)[3] = *m_ptr++;
            return val;
        }

    public:
        void init(const uint8_t* ptr, int dx, int dy)
        {
            m_ptr = ptr;
            m_y = read_int32() + dy;
            m_num_spans = (unsigned int)read_int32();
            m_dx = dx;
        }

    private:
        const uint8_t* m_ptr;
        int m_y;
        unsigned int m_num_spans;
        int m_dx;
    };

public:
    gfx_serialized_scanlines_adaptor_aa()
        : m_data(0)
        , m_end(0)
        , m_ptr(0)
        , m_dx(0)
        , m_dy(0)
        , m_min_x(0x7FFFFFFF)
        , m_min_y(0x7FFFFFFF)
        , m_max_x(-0x7FFFFFFF)
        , m_max_y(-0x7FFFFFFF)
    {
    }

    gfx_serialized_scanlines_adaptor_aa(const uint8_t* data,
                                        unsigned int size, scalar dx, scalar dy)
        : m_data(data)
        , m_end(data + size)
        , m_ptr(data)
        , m_dx(iround(dx))
        , m_dy(iround(dy))
        , m_min_x(0x7FFFFFFF)
        , m_min_y(0x7FFFFFFF)
        , m_max_x(-0x7FFFFFFF)
        , m_max_y(-0x7FFFFFFF)
    {
    }

    void init(const uint8_t* data, unsigned size, scalar dx, scalar dy)
    {
        m_data = data;
        m_end = data + size;
        m_ptr = data;
        m_dx = iround(dx);
        m_dy = iround(dy);
        m_min_x = 0x7FFFFFFF;
        m_min_y = 0x7FFFFFFF;
        m_max_x = -0x7FFFFFFF;
        m_max_y = -0x7FFFFFFF;
    }

private:
    int read_int32(void)
    {
        int32_t val;
        ((uint8_t*)&val)[0] = *m_ptr++;
        ((uint8_t*)&val)[1] = *m_ptr++;
        ((uint8_t*)&val)[2] = *m_ptr++;
        ((uint8_t*)&val)[3] = *m_ptr++;
        return val;
    }

    unsigned int read_int32u(void)
    {
        uint32_t val;
        ((uint8_t*)&val)[0] = *m_ptr++;
        ((uint8_t*)&val)[1] = *m_ptr++;
        ((uint8_t*)&val)[2] = *m_ptr++;
        ((uint8_t*)&val)[3] = *m_ptr++;
        return val;
    }

public:
    // iterate scanlines interface
    bool rewind_scanlines(void)
    {
        m_ptr = m_data;
        if (m_ptr < m_end) {
            m_min_x = read_int32() + m_dx;
            m_min_y = read_int32() + m_dy;
            m_max_x = read_int32() + m_dx;
            m_max_y = read_int32() + m_dy;
        }
        return m_ptr < m_end;
    }

    int min_x(void) const { return m_min_x; }
    int min_y(void) const { return m_min_y; }
    int max_x(void) const { return m_max_x; }
    int max_y(void) const { return m_max_y; }

    template <typename Scanline>
    bool sweep_scanline(Scanline& sl)
    {
        sl.reset_spans();
        for (;;) {
            if (m_ptr >= m_end)
                return false;

            read_int32();      // Skip scanline size in bytes
            int y = read_int32() + m_dy;
            unsigned int num_spans = read_int32();

            do {
                int x = read_int32() + m_dx;
                int len = read_int32();

                if (len < 0) {
                    sl.add_span(x, (unsigned int)(-len), *m_ptr);
                    m_ptr += sizeof(T);
                } else {
                    sl.add_cells(x, len, m_ptr);
                    m_ptr += len * sizeof(T);
                }
            } while(--num_spans);

            if (sl.num_spans()) {
                sl.finalize(y);
                break;
            }
        }
        return true;
    }

    // specialization for embedded_scanline
    bool sweep_scanline(embedded_scanline& sl)
    {
        do {
            if (m_ptr >= m_end)
                return false;

            unsigned int byte_size = read_int32u();
            sl.init(m_ptr, m_dx, m_dy);
            m_ptr += byte_size - sizeof(int32_t);
        } while(sl.num_spans() == 0);
        return true;
    }

private:
    const uint8_t* m_data;
    const uint8_t* m_end;
    const uint8_t* m_ptr;
    int m_dx;
    int m_dy;
    int m_min_x;
    int m_min_y;
    int m_max_x;
    int m_max_y;
};

// serialized scanlines adaptor antialias uint8_t
typedef gfx_serialized_scanlines_adaptor_aa<uint8_t>  gfx_serialized_scanlines_adaptor_aa_u8;



// scanline storage binary
class gfx_scanline_storage_bin
{
public:
    typedef struct {
        int32_t x;
        int32_t len;
    } span_data;

    typedef struct {
        int y;
        unsigned int num_spans;
        unsigned int start_span;
    } scanline_data;

    // inner class for embeded scanline
    class embedded_scanline
    {
    public:

        class const_iterator
        {
        public:
            const_iterator() : m_storage(0), m_span_idx(0) { }
            const_iterator(const embedded_scanline& sl)
                : m_storage(sl.m_storage)
                , m_span_idx(sl.m_scanline.start_span)
            {
                m_span = m_storage->span_by_index(m_span_idx);
            }

            const span_data& operator*() const { return m_span; }
            const span_data* operator->() const { return &m_span; }

            void operator ++ ()
            {
                ++m_span_idx;
                m_span = m_storage->span_by_index(m_span_idx);
            }

        private:
            const gfx_scanline_storage_bin* m_storage;
            unsigned int m_span_idx;
            span_data m_span;
        };

        friend class const_iterator;

        embedded_scanline(const gfx_scanline_storage_bin& storage)
            : m_storage(&storage)
            , m_scanline_idx(0)
        {
            init(0);
        }

        void reset(int, int) { }
        unsigned int num_spans(void) const { return m_scanline.num_spans; }
        int y(void) const { return m_scanline.y; }
        const_iterator begin() const { return const_iterator(*this); }

        void init(unsigned int scanline_idx)
        {
            m_scanline_idx = scanline_idx;
            m_scanline = m_storage->scanline_by_index(m_scanline_idx);
        }

    private:
        const gfx_scanline_storage_bin* m_storage;
        unsigned int m_scanline_idx;
        scanline_data m_scanline;
    };

    // scanline storage
    gfx_scanline_storage_bin()
        : m_spans(256-2)         // block increment size
        , m_cur_scanline(0)
        , m_min_x(0x7FFFFFFF)
        , m_min_y(0x7FFFFFFF)
        , m_max_x(-0x7FFFFFFF)
        , m_max_y(-0x7FFFFFFF)
    {
        m_fake_scanline.y = 0;
        m_fake_scanline.num_spans = 0;
        m_fake_scanline.start_span = 0;
        m_fake_span.x = 0;
        m_fake_span.len = 0;
    }

    // renderer interface
    void prepare(void)
    {
        m_scanlines.clear();
        m_spans.clear();
        m_cur_scanline = 0;
        m_min_x = 0x7FFFFFFF;
        m_min_y = 0x7FFFFFFF;
        m_max_x = -0x7FFFFFFF;
        m_max_y = -0x7FFFFFFF;
    }

    template <typename Scanline>
    void render(const Scanline& sl)
    {
        scanline_data sl_this;

        int y = sl.y();
        if (y < m_min_y) m_min_y = y;
        if (y > m_max_y) m_max_y = y;

        sl_this.y = y;
        sl_this.num_spans = sl.num_spans();
        sl_this.start_span = m_spans.size();
        typename Scanline::const_iterator span_iterator = sl.begin();

        unsigned int num_spans = sl_this.num_spans;
        for (;;) {
            span_data sp;
            sp.x = span_iterator->x;
            sp.len = (int32_t)abs((int)(span_iterator->len));
            m_spans.add(sp);
            int x1 = sp.x;
            int x2 = sp.x + sp.len - 1;
            if (x1 < m_min_x) m_min_x = x1;
            if (x2 > m_max_x) m_max_x = x2;
            if (--num_spans == 0) break;
            ++span_iterator;
        }
        m_scanlines.add(sl_this);
    }

    // iterate scanlines interface
    int min_x(void) const { return m_min_x; }
    int min_y(void) const { return m_min_y; }
    int max_x(void) const { return m_max_x; }
    int max_y(void) const { return m_max_y; }

    bool rewind_scanlines(void)
    {
        m_cur_scanline = 0;
        return m_scanlines.size() > 0;
    }

    template <typename Scanline>
    bool sweep_scanline(Scanline& sl)
    {
        sl.reset_spans();
        for (;;) {
            if (m_cur_scanline >= m_scanlines.size())
                return false;
            const scanline_data& sl_this = m_scanlines[m_cur_scanline];

            unsigned int num_spans = sl_this.num_spans;
            unsigned int span_idx = sl_this.start_span;
            do {
                const span_data& sp = m_spans[span_idx++];
                sl.add_span(sp.x, sp.len, cover_full);
            } while(--num_spans);

            ++m_cur_scanline;
            if (sl.num_spans()) {
                sl.finalize(sl_this.y);
                break;
            }
        }
        return true;
    }

    // specialization for embedded_scanline
    bool sweep_scanline(embedded_scanline& sl)
    {
        do {
            if (m_cur_scanline >= m_scanlines.size())
                return false;
            sl.init(m_cur_scanline);
            ++m_cur_scanline;
        } while(sl.num_spans() == 0);
        return true;
    }

    unsigned int byte_size(void) const
    {
        unsigned int size = sizeof(int32_t) * 4; // min_x, min_y, max_x, max_y

        for (unsigned int i = 0; i < m_scanlines.size(); ++i) {
            size += sizeof(int32_t) * 2 + // Y, num_spans
                    (unsigned int)m_scanlines[i].num_spans * sizeof(int32_t) * 2; // X, span_len
        }
        return size;
    }

    static void write_int32(uint8_t* dst, int32_t val)
    {
        dst[0] = ((const uint8_t*)&val)[0];
        dst[1] = ((const uint8_t*)&val)[1];
        dst[2] = ((const uint8_t*)&val)[2];
        dst[3] = ((const uint8_t*)&val)[3];
    }

    void serialize(uint8_t* data) const
    {
        write_int32(data, min_x()); // min_x
        data += sizeof(int32_t);
        write_int32(data, min_y()); // min_y
        data += sizeof(int32_t);
        write_int32(data, max_x()); // max_x
        data += sizeof(int32_t);
        write_int32(data, max_y()); // max_y
        data += sizeof(int32_t);

        for (unsigned int i = 0; i < m_scanlines.size(); ++i) {
            const scanline_data& sl_this = m_scanlines[i];

            write_int32(data, sl_this.y); // Y
            data += sizeof(int32_t);

            write_int32(data, sl_this.num_spans); // num_spans
            data += sizeof(int32_t);

            unsigned int num_spans = sl_this.num_spans;
            unsigned int span_idx = sl_this.start_span;
            do {
                const span_data& sp = m_spans[span_idx++];

                write_int32(data, sp.x); // X
                data += sizeof(int32_t);

                write_int32(data, sp.len); // len
                data += sizeof(int32_t);
            } while(--num_spans);
        }
    }

    const scanline_data& scanline_by_index(unsigned int i) const
    {
        return (i < m_scanlines.size()) ? m_scanlines[i] : m_fake_scanline;
    }

    const span_data& span_by_index(unsigned int i) const
    {
        return (i < m_spans.size()) ? m_spans[i] : m_fake_span;
    }

private:
    pod_bvector<span_data, 10> m_spans;
    pod_bvector<scanline_data, 8> m_scanlines;
    span_data m_fake_span;
    scanline_data m_fake_scanline;
    unsigned int m_cur_scanline;
    int m_min_x;
    int m_min_y;
    int m_max_x;
    int m_max_y;
};


// serialized scanlines adaptor binary
class gfx_serialized_scanlines_adaptor_bin
{
public:
    typedef bool cover_type;

    // inner class for embeded scanline
    class embedded_scanline
    {
    public:

        class const_iterator
        {
        public:
            typedef struct {
                int32_t x;
                int32_t len;
            } span;

            const_iterator() : m_ptr(0), m_dx(0) { }
            const_iterator(const embedded_scanline& sl)
                : m_ptr(sl.m_ptr)
                , m_dx(sl.m_dx)
            {
                m_span.x = read_int32() + m_dx;
                m_span.len = read_int32();
            }

            const span& operator*() const { return m_span;  }
            const span* operator->() const { return &m_span; }

            void operator ++ ()
            {
                m_span.x = read_int32() + m_dx;
                m_span.len = read_int32();
            }

        private:
            int read_int32(void)
            {
                int32_t val;
                ((uint8_t*)&val)[0] = *m_ptr++;
                ((uint8_t*)&val)[1] = *m_ptr++;
                ((uint8_t*)&val)[2] = *m_ptr++;
                ((uint8_t*)&val)[3] = *m_ptr++;
                return val;
            }

            const uint8_t* m_ptr;
            span m_span;
            int m_dx;
        };

        friend class const_iterator;

        embedded_scanline() : m_ptr(0), m_y(0), m_num_spans(0), m_dx(0) { }

        void reset(int, int) { }
        unsigned int num_spans(void) const { return m_num_spans; }
        int y(void) const { return m_y; }
        const_iterator begin(void) const { return const_iterator(*this); }

    private:
        int read_int32(void)
        {
            int32_t val;
            ((uint8_t*)&val)[0] = *m_ptr++;
            ((uint8_t*)&val)[1] = *m_ptr++;
            ((uint8_t*)&val)[2] = *m_ptr++;
            ((uint8_t*)&val)[3] = *m_ptr++;
            return val;
        }

    public:
        void init(const uint8_t* ptr, int dx, int dy)
        {
            m_ptr = ptr;
            m_y = read_int32() + dy;
            m_num_spans = (unsigned int)read_int32();
            m_dx = dx;
        }

    private:
        const uint8_t* m_ptr;
        int m_y;
        unsigned int m_num_spans;
        int m_dx;
    };

public:
    gfx_serialized_scanlines_adaptor_bin()
        : m_data(0)
        , m_end(0)
        , m_ptr(0)
        , m_dx(0)
        , m_dy(0)
        , m_min_x(0x7FFFFFFF)
        , m_min_y(0x7FFFFFFF)
        , m_max_x(-0x7FFFFFFF)
        , m_max_y(-0x7FFFFFFF)
    {
    }

    gfx_serialized_scanlines_adaptor_bin(const uint8_t* data,
                                         unsigned int size, scalar dx, scalar dy)
        : m_data(data)
        , m_end(data + size)
        , m_ptr(data)
        , m_dx(iround(dx))
        , m_dy(iround(dy))
        , m_min_x(0x7FFFFFFF)
        , m_min_y(0x7FFFFFFF)
        , m_max_x(-0x7FFFFFFF)
        , m_max_y(-0x7FFFFFFF)
    {
    }

    void init(const uint8_t* data, unsigned int size, scalar dx, scalar dy)
    {
        m_data = data;
        m_end = data + size;
        m_ptr = data;
        m_dx = iround(dx);
        m_dy = iround(dy);
        m_min_x = 0x7FFFFFFF;
        m_min_y = 0x7FFFFFFF;
        m_max_x = -0x7FFFFFFF;
        m_max_y = -0x7FFFFFFF;
    }

private:
    int read_int32(void)
    {
        int32_t val;
        ((uint8_t*)&val)[0] = *m_ptr++;
        ((uint8_t*)&val)[1] = *m_ptr++;
        ((uint8_t*)&val)[2] = *m_ptr++;
        ((uint8_t*)&val)[3] = *m_ptr++;
        return val;
    }

public:
    // iterate scanlines interface
    bool rewind_scanlines(void)
    {
        m_ptr = m_data;
        if (m_ptr < m_end) {
            m_min_x = read_int32() + m_dx;
            m_min_y = read_int32() + m_dy;
            m_max_x = read_int32() + m_dx;
            m_max_y = read_int32() + m_dy;
        }
        return m_ptr < m_end;
    }

    int min_x(void) const { return m_min_x; }
    int min_y(void) const { return m_min_y; }
    int max_x(void) const { return m_max_x; }
    int max_y(void) const { return m_max_y; }
    int x(void) const { return m_dx; }
    int y(void) const { return m_dy; }
    void setX(int x) { m_dx = x; }
    void setY(int y) { m_dy = y; }

    template <typename Scanline>
    bool sweep_scanline(Scanline& sl)
    {
        sl.reset_spans();
        for (;;) {
            if (m_ptr >= m_end)
                return false;

            int y = read_int32() + m_dy;
            unsigned int num_spans = read_int32();

            do {
                int x = read_int32() + m_dx;
                int len = read_int32();

                if (len < 0)
                    len = -len;
                sl.add_span(x, (unsigned int)len, cover_full);
            } while (--num_spans);

            if (sl.num_spans()) {
                sl.finalize(y);
                break;
            }
        }
        return true;
    }

    // specialization for embedded_scanline
    bool sweep_scanline(embedded_scanline& sl)
    {
        do {
            if (m_ptr >= m_end)
                return false;

            sl.init(m_ptr, m_dx, m_dy);

            // jump to the next scanline
            read_int32(); // Y
            int num_spans = read_int32(); // num_spans
            m_ptr += num_spans * sizeof(int32_t) * 2;
        } while(sl.num_spans() == 0);
        return true;
    }

private:
    const uint8_t* m_data;
    const uint8_t* m_end;
    const uint8_t* m_ptr;
    int m_dx;
    int m_dy;
    int m_min_x;
    int m_min_y;
    int m_max_x;
    int m_max_y;
};

}
#endif /*_GFX_SCANLINE_STORAGE_H_*/
