#pragma once

#include "config.h"

namespace agge
{
	class precise_delta
	{
	public:
		AGGE_INLINE precise_delta(int numerator, int denominator)
			: _acc(0), _exp(0)
		{
			const float q = static_cast<float>(numerator) / denominator;
			const int &iq = reinterpret_cast<const int &>(q);
			const int exp = (((iq & 0x7F800000)) >> 23) - 127;
			int m = (iq & 0x7FFFFF) | 0x800000;

			m--; // Sacrifice precision to be agnostic to the rounding mode: we must not overflow on increments!
			if (exp > 0x17)
				m <<= exp - 0x17;
			else if (exp >= 0x15)
				m >>= 0x17 - exp;
			else if (exp >= 0x15 - 0x1E)
				_exp = 0x15 - exp, m >>= 2;
			else
				_exp = 0x1E, m >>= 2 + 0x15 - 0x1E - exp;
			_quotient = (m ^ iq >> 31) + (static_cast<unsigned>(iq) >> 31);
		}

		void multiply(int k)
		{
			_delta_fraction = k * _quotient;
		}

		int next()
		{
			_acc += _delta_fraction;
			int delta = _acc >> _exp;
			_acc -= delta << _exp;
			return delta;
		}

	private:
		int _acc;
		int _quotient;
		int _delta_fraction;
		int _exp;
	};
}
