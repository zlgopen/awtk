/* Picasso - a vector graphics library
 *
 *  Copyright (C) 2013 Zhang Ji Peng.
 *  Contact: onecoolx@gmail.com
 */

#ifndef _SHARED_H_
#define _SHARED_H_

namespace picasso {

class shared
{
public:
    shared()
        :m_refCount(1)
    {
    }

    virtual ~shared()
    {
        //must be virtual for deref
    }

    void ref(void)
    {
        ++m_refCount;
    }

    void deref(void)
    {
        if (--m_refCount <= 0)
            removeThis();
    }

    int count(void)
    {
        return m_refCount;
    }
protected:
    virtual void removeThis(void)
    {
        //can be extend in sub classa for more clean up code
        delete this;
    }

private:
    shared(const shared&);
    shared& operator=(const shared&);
    int m_refCount;
};

}

using picasso::shared;

#endif /* _SHARED_H_ */
