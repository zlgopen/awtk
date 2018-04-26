/* Picasso - a vector graphics library
 *
 * Copyright (C) 2014 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#ifndef _GFX_GAMMA_FUNCTION_H_
#define _GFX_GAMMA_FUNCTION_H_

#include "common.h"

namespace gfx {

// gamma none
class gamma_none
{
public:
    scalar operator()(scalar x) const { return x; }
};

// gamma power
class gamma_power
{
public:
    gamma_power() : m_gamma(FLT_TO_SCALAR(1.0f)) { }
    gamma_power(scalar g) : m_gamma(g) { }

    void gamma(scalar g) { m_gamma = g; }
    scalar gamma(void) const { return m_gamma; }

    scalar operator()(scalar x) const
    {
        return Pow(x, m_gamma);
    }

private:
    scalar m_gamma;
};

// gamma threshold
class gamma_threshold
{
public:
    gamma_threshold() : m_threshold(FLT_TO_SCALAR(0.5f)) { }
    gamma_threshold(scalar t) : m_threshold(t) { }

    void threshold(scalar t) { m_threshold = t; }
    scalar threshold(void) const { return m_threshold; }

    scalar operator()(scalar x) const
    {
        return (x < m_threshold) ? FLT_TO_SCALAR(0.0f) : FLT_TO_SCALAR(1.0f);
    }

private:
    scalar m_threshold;
};

// gamma linear
class gamma_linear
{
public:
    gamma_linear() : m_start(FLT_TO_SCALAR(0.0f)), m_end(FLT_TO_SCALAR(1.0f)) { }
    gamma_linear(scalar s, scalar e) : m_start(s), m_end(e) { }

    void set(scalar s, scalar e) { m_start = s; m_end = e; }
    void start(scalar s) { m_start = s; }
    void end(scalar e) { m_end = e; }
    scalar start(void) const { return m_start; }
    scalar end(void) const { return m_end; }

    scalar operator()(scalar x) const
    {
        if (x < m_start) return FLT_TO_SCALAR(0.0f);
        if (x > m_end) return FLT_TO_SCALAR(1.0f);
        return (x - m_start) / (m_end - m_start);
    }

private:
    scalar m_start;
    scalar m_end;
};

// gamma multiply
class gamma_multiply
{
public:
    gamma_multiply() : m_mul(FLT_TO_SCALAR(1.0f)) { }
    gamma_multiply(scalar v) : m_mul(v) { }

    void value(scalar v) { m_mul = v; }
    scalar value(void) const { return m_mul; }

    scalar operator()(scalar x) const
    {
        scalar y = x * m_mul;
        if (y > FLT_TO_SCALAR(1.0f)) y = FLT_TO_SCALAR(1.0f);
        return y;
    }

private:
    scalar m_mul;
};

}
#endif /*_GFX_GAMMA_FUNCTION_H_*/
