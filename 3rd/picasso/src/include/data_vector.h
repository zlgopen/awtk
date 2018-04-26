/* Picasso - a vector graphics library
 *
 * Copyright (C) 2011 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#ifndef _DATA_VECTOR_H_
#define _DATA_VECTOR_H_

#include <string.h>
#include "common.h"

namespace picasso {

template <typename T> struct pod_allocator
{
    static T*   allocate(unsigned int num)       { return new T [num]; }
    static void deallocate(T* ptr, unsigned int) { delete [] ptr;      }
};

// pod array
template <typename T> class pod_array
{
public:
    pod_array()
        : m_size(0)
        , m_capacity(0)
        , m_array(0)
    {
    }

    ~pod_array()
    {
        pod_allocator<T>::deallocate(m_array, m_capacity);
    }

    pod_array(unsigned int size)
        : m_size(size)
        , m_capacity(size)
        , m_array(pod_allocator<T>::allocate(m_capacity))
    {
    }

    pod_array(const pod_array<T>& o)
        : m_size(o.m_size)
        , m_capacity(o.m_size) // copy constrcut size == capacity
        , m_array(pod_allocator<T>::allocate(o.m_size))
    {
        mem_copy(m_array, o.m_array, sizeof(T) * m_size);
    }

    void resize(unsigned int size)
    {
        if (size > m_size) { // resize will invalid the old data.
            if (size > m_capacity) {
                pod_allocator<T>::deallocate(m_array, m_capacity);
                m_array = pod_allocator<T>::allocate(size);
                m_capacity = size;
            }
            m_size = size;
        } else {
            m_size = size;
        }
    }
    const pod_array<T>& operator = (const pod_array<T>& o)
    {
        if (this == &o)
            return *this;

        resize(o.size());
        mem_copy(m_array, o.m_array, sizeof(T) * m_size);
        return *this;
    }

    unsigned int size(void) const { return m_size; }

    const T& operator [] (unsigned i) const { return m_array[i]; }
    T& operator [] (unsigned i) { return m_array[i]; }

    const T& at(unsigned i) const { return m_array[i]; }
    T& at(unsigned i) { return m_array[i]; }

    T value_at(unsigned i) const { return m_array[i]; }

    const T* data() const { return m_array; }
    T* data(void) { return m_array; }
private:
    unsigned int m_size;
    unsigned int m_capacity;
    T*           m_array;
};


// pod vector
template <typename T> class pod_vector
{
public:
    enum {
       max_limit = 16384,
    };

    pod_vector()
        : m_size(0)
        , m_capacity(0)
        , m_array(0)
    {
    }

    ~pod_vector()
    {
        pod_allocator<T>::deallocate(m_array, m_capacity);
    }

    pod_vector(unsigned int cap);

    // Copying
    pod_vector(const pod_vector<T>&);

    const pod_vector<T>& operator = (const pod_vector<T>&);

    // Set new capacity. All data is lost, size is set to zero.
    void capacity(unsigned int cap);
    unsigned int capacity(void) const { return m_capacity; }
    unsigned int size(void) const { return m_size; }

    // Allocate n elements. All data is lost,
    // but elements can be accessed in range 0...size-1.
    void allocate(unsigned int size);
    // Resize keeping the content.
    void resize(unsigned int new_size);

    bool push_back(const T& v);

    void zero(void) { memset(m_array, 0, sizeof(T) * m_size); }

    bool is_full(void) const { return m_size == m_capacity;}

    bool insert_at(unsigned int pos, const T& val);

    bool set_data(unsigned int num, T* data);

    const T& operator [] (unsigned int i) const { return m_array[i]; }
          T& operator [] (unsigned int i)       { return m_array[i]; }

    const T* data(void) const { return m_array; }
          T* data(void)       { return m_array; }

    void clear(void)              { m_size = 0; }
    void remove_last(void) { if (m_size) --m_size; }
    void cut_at(unsigned int num) { if (num < m_size) m_size = num; }

protected:
    unsigned int m_size;
    unsigned int m_capacity;
    T*           m_array;
};

template <typename T> pod_vector<T>::pod_vector(unsigned int cap)
    : m_size(0)
    , m_capacity(cap)
    , m_array(pod_allocator<T>::allocate(m_capacity))
{
}

template <typename T> pod_vector<T>::pod_vector(const pod_vector<T>& v)
    : m_size(v.m_size)
    , m_capacity(v.m_capacity)
    , m_array(v.m_capacity ? pod_allocator<T>::allocate(v.m_capacity) : 0)
{
    if (m_array)
        mem_copy(m_array, v.m_array, sizeof(T) * v.m_size);
}

template <typename T>
inline void pod_vector<T>::capacity(unsigned int cap)
{
    m_size = 0;
    if (cap > m_capacity) {
        pod_allocator<T>::deallocate(m_array, m_capacity);
        m_capacity = cap;
        m_array = m_capacity ? pod_allocator<T>::allocate(m_capacity) : 0;
    }
}

template <typename T>
inline void pod_vector<T>::resize(unsigned int new_size)
{
    if (new_size > m_size) {
        if (new_size > m_capacity) {
            T* data = pod_allocator<T>::allocate(new_size);
            mem_copy(data, m_array, m_size * sizeof(T));
            pod_allocator<T>::deallocate(m_array, m_capacity);
            m_capacity = new_size;
            m_array = data;
        }
    } else {
        m_size = new_size;
    }
}

template <typename T>
inline void pod_vector<T>::allocate(unsigned int new_size)
{
    capacity(new_size);
    m_size = new_size;
}

template <typename T>
inline const pod_vector<T>& pod_vector<T>::operator = (const pod_vector<T>& v)
{
    if (this == &v)
        return *this;

    if (v.m_size > m_size) {
        if ((v.m_size > m_capacity) || (m_capacity > max_limit)) {
            pod_allocator<T>::deallocate(m_array, m_capacity);
            m_capacity = v.m_capacity;
            m_array = 0;

            if (m_capacity)
                m_array = pod_allocator<T>::allocate(m_capacity);
        }
        m_size = v.m_size;
    } else {
        m_size = v.m_size;
    }

    if (m_size)
        mem_copy(m_array, v.m_array, sizeof(T) * v.m_size);

    return *this;
}

template <typename T>
inline bool pod_vector<T>::push_back(const T& v)
{
    if (unlikely(m_size >= m_capacity))
        return false;

    m_array[m_size++] = v;
    return true;
}

template <typename T>
inline bool pod_vector<T>::insert_at(unsigned int pos, const T& val)
{
    if (pos >= m_capacity)
        return false;

    if (pos >= m_size) {
        m_array[m_size] = val;
    } else {
        mem_deep_copy(m_array + pos + 1, m_array + pos, (m_size - pos) * sizeof(T));
        m_array[pos] = val;
    }
    ++m_size;
    return true;
}

template <typename T>
inline bool pod_vector<T>::set_data(unsigned int num, T* data)
{
    if (!num || !data)
        return false;

    if (num > m_capacity)
        return false;

    m_size = num;
    mem_copy(m_array, data, sizeof(T) * m_size);
    return true;
}


// pod_bvector  default S = 5, mean block size 32
template <typename T, unsigned int S = 5> class pod_bvector
{
public:
    enum {
        block_shift = S,
        block_size  = 1 << block_shift,
        block_mask  = block_size - 1,
    };

    typedef T value_type;

    pod_bvector();
    pod_bvector(unsigned int block_ptr_inc);
    pod_bvector(const pod_bvector<T, S>& o);
    ~pod_bvector();

    const pod_bvector<T, S>& operator = (const pod_bvector<T, S>& o);

    void add(const T& v);
    void modify_last(const T& v);
    void remove_all(void);

    T& at(unsigned int i)
    {
        return m_blocks[i >> block_shift][i & block_mask];
    }

    const T& operator [] (unsigned int i) const
    {
        return m_blocks[i >> block_shift][i & block_mask];
    }

    T& operator [] (unsigned int i)
    {
        return m_blocks[i >> block_shift][i & block_mask];
    }

    const T& curr(unsigned int idx) const
    {
        return (*this)[idx];
    }

    T& curr(unsigned int idx)
    {
        return (*this)[idx];
    }

    const T& prev(unsigned int idx) const
    {
        return (*this)[(idx + m_size - 1) % m_size];
    }

    T& prev(unsigned int idx)
    {
        return (*this)[(idx + m_size - 1) % m_size];
    }

    const T& next(unsigned int idx) const
    {
        return (*this)[(idx + 1) % m_size];
    }

    T& next(unsigned int idx)
    {
        return (*this)[(idx + 1) % m_size];
    }

    const T& last(void) const
    {
        return (*this)[m_size - 1];
    }

    T& last(void)
    {
        return (*this)[m_size - 1];
    }

    unsigned int size(void) const { return m_size; }
    unsigned int capacity(void) const { return m_num_blocks * block_size; }
    void clear(void) { m_size = 0; }
    void remove_last(void) { if (m_size) --m_size; }
    void cut_at(unsigned int size) { if (size < m_size) m_size = size; }
private:
    void allocate_block(unsigned int num_block);

    unsigned int m_size;
    unsigned int m_num_blocks;
    unsigned int m_max_blocks;
    T**          m_blocks;
    unsigned int m_block_ptr_inc;
};

template <typename T, unsigned int S> pod_bvector<T,S>::~pod_bvector()
{
    if (m_num_blocks) {
        T** block = m_blocks + m_num_blocks - 1;
        while (m_num_blocks--) {
            pod_allocator<T>::deallocate(*block, block_size);
            --block;
        }
    }
    if (m_blocks) {
        pod_allocator<T*>::deallocate(m_blocks, m_max_blocks);
    }
}

template <typename T, unsigned int S> pod_bvector<T, S>::pod_bvector()
    : m_size(0)
    , m_num_blocks(0)
    , m_max_blocks(0)
    , m_blocks(0)
    , m_block_ptr_inc(block_size)
{
}

template <typename T, unsigned int S> pod_bvector<T, S>::pod_bvector(unsigned int block_ptr_inc)
    : m_size(0)
    , m_num_blocks(0)
    , m_max_blocks(0)
    , m_blocks(0)
    , m_block_ptr_inc(block_ptr_inc)
{
}

template <typename T, unsigned int S>
pod_bvector<T, S>::pod_bvector(const pod_bvector<T, S>& o)
    : m_size(o.m_size)
    , m_num_blocks(o.m_num_blocks)
    , m_max_blocks(o.m_max_blocks)
    , m_blocks(o.m_max_blocks ? pod_allocator<T*>::allocate(o.m_max_blocks) : 0)
    , m_block_ptr_inc(o.m_block_ptr_inc)
{
    for (unsigned int i = 0; i < o.m_num_blocks; ++i) {
        m_blocks[i] = pod_allocator<T>::allocate(block_size);
        mem_copy(m_blocks[i], o.m_blocks[i], block_size * sizeof(T));
    }
}

template <typename T, unsigned int S>
const pod_bvector<T, S>& pod_bvector<T, S>::operator = (const pod_bvector<T, S>& o)
{
    if (this == &o)
        return *this;

    remove_all();

    m_size = o.m_size;
    m_num_blocks = o.m_num_blocks;
    m_max_blocks = o.m_max_blocks;
    m_blocks = o.m_max_blocks ? pod_allocator<T*>::allocate(o.m_max_blocks) : 0;
    m_block_ptr_inc = o.m_block_ptr_inc;

    for (unsigned int i = 0; i < o.m_num_blocks; ++i) {
        m_blocks[i] = pod_allocator<T>::allocate(block_size);
        mem_copy(m_blocks[i], o.m_blocks[i], block_size * sizeof(T));
    }

    return *this;
}

template <typename T, unsigned int S>
inline void pod_bvector<T, S>::add(const T& v)
{
    register unsigned int nb = m_size >> block_shift;
    if (unlikely(nb >= m_num_blocks)) {
        allocate_block(nb);
    }

    T* val = m_blocks[nb] + (m_size & block_mask);
    *val = v;
    ++m_size;
}

template <typename T, unsigned int S>
inline void pod_bvector<T, S>::allocate_block(unsigned int nb)
{
    if (unlikely(nb >= m_max_blocks)) {
        T** new_blocks = pod_allocator<T*>::allocate(m_max_blocks + m_block_ptr_inc);

        if (m_blocks) {
            mem_copy(new_blocks, m_blocks, m_num_blocks * sizeof(T*));
            pod_allocator<T*>::deallocate(m_blocks, m_max_blocks);
        }

        m_blocks = new_blocks;
        m_max_blocks += m_block_ptr_inc;
    }
    m_blocks[nb] = pod_allocator<T>::allocate(block_size);
    m_num_blocks++;
}

template <typename T, unsigned int S>
inline void pod_bvector<T, S>::modify_last(const T& v)
{
    register unsigned int idx = m_size - 1;
    T& val = m_blocks[idx >> block_shift][idx & block_mask];
    val = v;
}

template <typename T, unsigned int S>
inline void pod_bvector<T, S>::remove_all(void)
{
    if (m_num_blocks) {
        while(m_num_blocks > 0) {
            pod_allocator<T>::deallocate(m_blocks[--m_num_blocks], block_size);
        }

        pod_allocator<T*>::deallocate(m_blocks, m_max_blocks);
        m_blocks = 0;
        m_max_blocks = 0;
        m_size = 0;
    }
}


// block_allocator
class block_allocator
{
    typedef struct {
        byte*        data;
        unsigned int size;
    } block_type;

public:

    block_allocator(unsigned block_size, unsigned block_ptr_inc = 256-8)
        : m_block_size(block_size)
        , m_block_ptr_inc(block_ptr_inc)
        , m_num_blocks(0)
        , m_max_blocks(0)
        , m_blocks(0)
        , m_buf_ptr(0)
        , m_remain_size(0)
    {
#if _DEBUG
        m_all_mem = 0;
#endif
    }

    ~block_allocator()
    {
        remove_all();
    }

    byte* allocate(unsigned int size, unsigned int alignment = 1)
    {
        if (!size)
            return 0;

        if (size <= m_remain_size) {
            byte* ptr = m_buf_ptr;
            if (alignment > 1) {
                unsigned int align = (unsigned int)((alignment - ((intptr_t)(ptr)) % alignment) % alignment);

                size += align;
                ptr += align;

                if (size <= m_remain_size) {
                    m_remain_size -= size;
                    m_buf_ptr += size;
                    return ptr;
                }

                allocate_block(size);
                return allocate(size - align, alignment);
            }

            m_remain_size -= size;
            m_buf_ptr += size;
            return ptr;
        }
        allocate_block(size + alignment - 1);
        return allocate(size, alignment);
    }

    void remove_all(void)
    {
        if (m_num_blocks) {
            block_type* blk = m_blocks + m_num_blocks - 1;
            while (m_num_blocks--) {
                pod_allocator<byte>::deallocate(blk->data, blk->size);
                --blk;
            }
            pod_allocator<block_type>::deallocate(m_blocks, m_max_blocks);
        }

        m_num_blocks = 0;
        m_max_blocks = 0;
        m_blocks = 0;
        m_buf_ptr = 0;
        m_remain_size = 0;
#if _DEBUG
        m_all_mem = 0;
#endif
    }

#if _DEBUG
    unsigned int all_mem_used(void) const { return m_all_mem;}
#endif

private:
    block_allocator(const block_allocator&);
    block_allocator& operator=(block_allocator&);

    void allocate_block(unsigned int size)
    {
        if (size < m_block_size)
            size = m_block_size;

        if (m_num_blocks >= m_max_blocks) {
            block_type* new_blocks = pod_allocator<block_type>::allocate(m_max_blocks + m_block_ptr_inc);

#if _DEBUG
            m_all_mem += sizeof(block_type)*(m_block_ptr_inc);
#endif

            if (m_blocks) {
                mem_copy(new_blocks, m_blocks, m_num_blocks * sizeof(block_type));
                pod_allocator<block_type>::deallocate(m_blocks, m_max_blocks);
            }

            m_blocks = new_blocks;
            m_max_blocks += m_block_ptr_inc;
        }

        m_blocks[m_num_blocks].size = size;
        m_blocks[m_num_blocks].data = m_buf_ptr = pod_allocator<byte>::allocate(size);

        m_num_blocks++;
        m_remain_size = size;

#if _DEBUG
        m_all_mem += m_remain_size;
#endif
    }

    unsigned int m_block_size;
    unsigned int m_block_ptr_inc;
    unsigned int m_num_blocks;
    unsigned int m_max_blocks;
    block_type*  m_blocks;
    byte*        m_buf_ptr;
    unsigned int m_remain_size;
#if _DEBUG
    unsigned int m_all_mem;
#endif
};


    //------------------------------------------------------------------------


// swap_elements
template <typename T> inline void swap_elements(T& a, T& b)
{
    T temp = a;
    a = b;
    b = temp;
}

const int quick_sort_threshold = 9;

// quick_sort
template <typename Array, typename LessFunc>
void quick_sort(Array& array, LessFunc less)
{
    if (array.size() < 2)
        return;

    typename Array::value_type* e1;
    typename Array::value_type* e2;

    int stack[80];
    int* top = stack;
    int limit = array.size();
    int base = 0;

    while (true) {
        int len = limit - base;

        int i;
        int j;
        int pivot;

        if (len > quick_sort_threshold) {
            // we use base + len/2 as the pivot
            pivot = base + len / 2;
            swap_elements(array[base], array[pivot]);

            i = base + 1;
            j = limit - 1;

            // now ensure that *i <= *base <= *j
            e1 = &(array[j]);
            e2 = &(array[i]);
            if (less(*e1, *e2))
                swap_elements(*e1, *e2);

            e1 = &(array[base]);
            e2 = &(array[i]);
            if (less(*e1, *e2))
                swap_elements(*e1, *e2);

            e1 = &(array[j]);
            e2 = &(array[base]);
            if (less(*e1, *e2))
                swap_elements(*e1, *e2);

            while (true) {
                do { i++; } while (less(array[i], array[base]) );
                do { j--; } while (less(array[base], array[j]) );

                if ( i > j ) {
                    break;
                }

                swap_elements(array[i], array[j]);
            }

            swap_elements(array[base], array[j]);

            // now, push the largest sub-array
            if (j - base > limit - i) {
                top[0] = base;
                top[1] = j;
                base   = i;
            } else {
                top[0] = i;
                top[1] = limit;
                limit  = j;
            }
            top += 2;
        } else {
            // the sub-array is small, perform insertion sort
            j = base;
            i = j + 1;

            for (; i < limit; j = i, i++) {
                for (; less(*(e1 = &(array[j + 1])), *(e2 = &(array[j]))); j--) {
                    swap_elements(*e1, *e2);
                    if (j == base) {
                        break;
                    }
                }
            }

            if (top > stack) {
                top  -= 2;
                base  = top[0];
                limit = top[1];
            } else {
                break;
            }
        }
    }
}

// remove_duplicates
// Remove duplicates from a sorted array. It doesn't cut the
// tail of the array, it just returns the number of remaining elements.
template <typename Array, typename EqualFunc>
unsigned remove_duplicates(Array& array, EqualFunc equal)
{
    if (array.size() < 2)
        return array.size();

    unsigned int i, j;
    for (i = 1, j = 1; i < array.size(); i++) {
        typename Array::value_type& e = array[i];
        if (!equal(e, array[i - 1])) {
            array[j++] = e;
        }
    }
    return j;
}

}

using picasso::pod_array;
using picasso::pod_vector;
using picasso::pod_bvector;

#endif /*_DATA_VECTOR_H_*/

