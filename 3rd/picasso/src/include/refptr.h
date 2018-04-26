/* Picasso - a vector graphics library
 *
 *  Copyright (C) 2013 Zhang Ji Peng.
 *  Contact: onecoolx@gmail.com
 */

#ifndef _REFPTR_H_
#define _REFPTR_H_

namespace picasso {

template <typename T> class refptr
{
public:
    refptr()
        :m_ptr(0)
    {

    }

    explicit refptr(T * ptr)
        :m_ptr(ptr)
    {
        if (m_ptr)
            m_ptr->ref();
    }

    refptr(const refptr & o)
        :m_ptr(o.m_ptr)
    {
        if (m_ptr)
            m_ptr->ref();
    }

    ~refptr()
    {
        if (m_ptr)
            m_ptr->deref();
    }

public:
    bool operator!() const
    {
        return !m_ptr;
    }

    T* operator->() const
    {
        return m_ptr;
    }

    refptr& operator=(const refptr & o)
    {
        T* optr = o.m_ptr;
        if (optr)
            optr->ref();
        T* ptr = m_ptr;
        m_ptr = optr;
        if (ptr)
            ptr->deref();
        return *this;
    }

    refptr& operator=(T * optr)
    {
        if (optr)
            optr->ref();
        T* ptr = m_ptr;
        m_ptr = optr;
        if (ptr)
            ptr->deref();
        return *this;
    }

    T* getptr() const
    {
        return m_ptr;
    }

private:
    T *m_ptr;
};

template <typename T> inline bool operator!=(const refptr<T>& a, const refptr<T>& b)
{
    return a.getptr() != b.getptr();
}

template <typename T> inline bool operator==(const refptr<T>& a, const refptr<T>& b)
{
    return a.getptr() == b.getptr();
}

}

using picasso::refptr;

#endif /* _REFPTR_H_ */
