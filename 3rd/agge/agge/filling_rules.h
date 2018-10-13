#pragma once

#include "vector_rasterizer.h"

namespace agge
{
	template <agge::uint8_t area_factor_shift = vector_rasterizer::_1_shift>
	struct winding
	{
		uint8_t operator ()(int area) const;
	};



	template <agge::uint8_t area_factor_shift>
	inline uint8_t winding<area_factor_shift>::operator ()(int area) const
	{
		area >>= 1 + area_factor_shift;
		if (area < 0)
			area = -area;
		if (area > vector_rasterizer::_1_mask)
			area = vector_rasterizer::_1_mask;
		return static_cast<uint8_t>(area);
	}
}
