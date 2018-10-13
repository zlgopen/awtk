#include <agge/stroke_features.h>

namespace agge
{
	namespace caps
	{
		void butt::calc(points &output, real_t w, const point_r &v0, real_t d, const point_r &v1) const
		{
			d = w / d;
			
			const real_t dx = (v1.y - v0.y) * d;
			const real_t dy = (v1.x - v0.x) * d;

			output.push_back(create_point(v0.x - dx, v0.y + dy));
			output.push_back(create_point(v0.x + dx, v0.y - dy));
		}


		triangle::triangle(real_t tip_extension)
			: _tip_extension(tip_extension)
		{	}

		void triangle::calc(points &output, real_t w, const point_r &v0, real_t d, const point_r &v1) const
		{
			d = w / d;
			
			const real_t dx = (v1.y - v0.y) * d;
			const real_t dy = (v1.x - v0.x) * d;

			output.push_back(create_point(v0.x - dx, v0.y + dy));
			output.push_back(create_point(v0.x - dy * _tip_extension, v0.y - dx * _tip_extension));
			output.push_back(create_point(v0.x + dx, v0.y - dy));
		}
	}

	namespace joins
	{
		void bevel::calc(points &output, real_t w, const point_r &v0, real_t d01, const point_r &v1, real_t d12, const point_r &v2) const
		{
			d01 = w / d01;
			d12 = w / d12;
				
			const real_t dx1 = d01 * (v1.y - v0.y);
			const real_t dy1 = d01 * (v1.x - v0.x);
			const real_t dx2 = d12 * (v2.y - v1.y);
			const real_t dy2 = d12 * (v2.x - v1.x);

			output.push_back(create_point(v1.x + dx1, v1.y - dy1));
			output.push_back(create_point(v1.x + dx2, v1.y - dy2));
		}
	}
}
