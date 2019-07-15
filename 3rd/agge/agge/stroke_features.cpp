#include <agge/stroke_features.h>
#include <math.h>
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

		void square::calc(points &output, real_t w, const point_r &v0, real_t d, const point_r &v1) const
		{
			d = w / d;
			float x = 0.0f, y = 0.0f, k = 0.0f;
			const real_t dx = (v1.y - v0.y) * d;
			const real_t dy = (v1.x - v0.x) * d;

			if (dy == 0)
			{
				x = 0;
				y = w * (dx > 0 ? -1 : 1);
			}
			else
			{
				k = (v1.y - v0.y) / (v1.x - v0.x);
				x = sqrt(w * w / (1 + k * k)) * (dy > 0 ? -1 : 1);
				y = sqrt(k * k * w * w / (1 + k * k)) * (dx > 0 ? -1 : 1);
			}

			output.push_back(create_point(v0.x - dx, v0.y + dy));
			output.push_back(create_point(v0.x - dx + x, v0.y + dy + y));
			output.push_back(create_point(v0.x + dx + x, v0.y - dy + y));
			output.push_back(create_point(v0.x + dx, v0.y - dy));
		}

		void round::calc(points &output, real_t w, const point_r &v0, real_t d, const point_r &v1) const
		{
			d = w / d;

			const real_t dx = (v1.y - v0.y) * d;
			const real_t dy = (v1.x - v0.x) * d;

			float step = pi / w;
			float offset_pi = dy > 0 ? pi : dy < 0 ? 0 : pi;
			float d1 = ::atan((v1.y - v0.y) / (v1.x - v0.x)) + offset_pi;
			float min_angle = d1 - pi / 2;
			float max_angle = pi + min_angle;

			for (float angle = min_angle; angle < max_angle; )
			{
				float x = w * cos(angle) + v0.x;
				float y = w * sin(angle) + v0.y;
				output.push_back(create_point(x, y));
				angle += step;
				if (angle > max_angle)
				{
					angle = max_angle;
				}
			}

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

		void get_lines_midpoint(const point_r &v0, real_t d01, const point_r &v1, real_t d12, const point_r &v2, float& m_x, float& m_y)
		{
			float dx = 0.0f, k = 0.0f, m_x1 = 0.0f, m_y1 = 0.0f, xx = 0.0f, yx = 0.0f, x_d = 0.0f, y_d = 0.0f;

			if (d01 > d12)
			{
				dx = d12;
				xx = v0.x;
				yx = v0.y;
				x_d = v2.x;
				y_d = v2.y;
			}
			else
			{
				dx = d01;
				xx = v2.x;
				yx = v2.y;
				x_d = v0.x;
				y_d = v0.y;
			}

			if (xx == v1.x)
			{
				m_x1 = 0;
				m_y1 = dx * (yx > v1.y ? 1 : -1);
			}
			else
			{
				k = (yx - v1.y) / (xx - v1.x);
				m_x1 = sqrt(dx * dx / (1 + k * k)) * (xx > v1.x ? 1 : -1);
				m_y1 = sqrt(k * k * dx * dx / (1 + k * k)) * (yx > v1.y ? 1 : -1);
			}
			m_x1 += v1.x;
			m_y1 += v1.y;

			m_x = (m_x1 + x_d) / 2;
			m_y = (m_y1 + y_d) / 2;
		}

		void round::calc(points &output, real_t w, const point_r &v0, real_t d01, const point_r &v1, real_t d12, const point_r &v2) const
		{
			const real_t d01_w = w / d01;
			const real_t d12_w = w / d12;

			const real_t dx1 = d01_w * (v1.y - v0.y);
			const real_t dy1 = d01_w * (v1.x - v0.x);
			const real_t dx2 = d12_w * (v2.y - v1.y);
			const real_t dy2 = d12_w * (v2.x - v1.x);

			float d1 = 0.0f;
			float angle = 0.0f;
			float step = pi / w;
			float min_angle = 0.0f;
			float max_angle = 0.0f;
			float m_x = 0.0f, m_y = 0.0f;

			get_lines_midpoint(v0, d01, v1, d12, v2, m_x, m_y);

			d1 = ::atan((m_y - v1.y) / (m_x - v1.x));

			if (m_y > v1.y)
			{
				if (d1 > 0)
				{
					d1 += (pi / 2);
				}
				else
				{
					d1 -= (pi / 2);
				}
			}
			else if (m_y < v1.y)
			{
				if (d1 > 0)
				{
					d1 -= (pi / 2);
				}
				else
				{
					d1 += (pi / 2);
				}
			}
			else
			{
				if (m_x > v1.x)
				{
					d1 += (pi / 2);
				}
				else
				{
					d1 -= (pi / 2);
				}
			}

			min_angle = d1;
			max_angle = pi + min_angle;

			output.push_back(create_point(v1.x + dx1, v1.y - dy1));

			for (angle = min_angle; angle < max_angle; )
			{
				output.push_back(create_point(w * cos(angle) + v1.x, w * sin(angle) + v1.y));
				angle += step;
				if (angle > max_angle)
				{
					angle = max_angle;
				}
			}

			output.push_back(create_point(w * cos(angle) + v1.x, w * sin(angle) + v1.y));

			output.push_back(create_point(v1.x + dx2, v1.y - dy2));
		}

		float get_new_edge_point(const point_r &v0, const point_r &v1, const point_r &v2, float m_x, float m_y, float x1, float y1, float w, float& x, float& y, bool& is_h)
		{
			float A = 0.0f;
			float B = 0.0f;
			float C = 0.0f;
			float k = 0.0f;
			float d1 = 0.0f;
			float d2 = 0.0f;
			float mod = 0.0f;
			float tmp_x1 = 0.0f;
			float tmp_x2 = 0.0f;
			float tmp_y1 = 0.0f;
			float tmp_y2 = 0.0f;

			if (v0.x == v1.x)
			{
				is_h = false;
				y = v0.y;

				if (v1.x < v0.x || (v1.x == v0.x && v0.x < v2.x))
				{
					x = x1 - w;
				}
				else
				{
					x = x1 + w;
				}
			}
			else if (v0.y == v1.y)
			{
				is_h = true;
				x = v0.x;

				if (v1.y < v0.y || (v1.y == v0.y && v0.y < v2.y))
				{
					y = y1 - w;
				}
				else
				{
					y = y1 + w;
				}
			}
			else
			{
				A = (v0.y - v1.y) / (v0.x - v1.x);
				k = -1 / A;
				B = -1.0f;
				mod = sqrt(A*A + B*B);
				x = sqrt(w * w / (1 + k * k));
				y = sqrt(k * k * w * w / (1 + k * k));


				if (k < 0)
				{
					tmp_x1 = x1 + x;
					tmp_y1 = y1 - y;

					tmp_x2 = x1 - x;
					tmp_y2 = y1 + y;
				}
				else
				{
					tmp_x1 = x1 - x;
					tmp_y1 = y1 - y;

					tmp_x2 = x1 + x;
					tmp_y2 = y1 + y;
				}

				d1 = fabs(A * m_x + B * m_y + tmp_y1 - A * tmp_x1) / mod;
				d2 = fabs(A * m_x + B * m_y + tmp_y2 - A * tmp_x2) / mod;
				if (d1 > d2)
				{
					x = tmp_x1;
					y = tmp_y1;
				}
				else
				{
					x = tmp_x2;
					y = tmp_y2;
				}
			}

			return A;
		}

		void miter::calc(points &output, real_t w, const point_r &v0, real_t d01, const point_r &v1, real_t d12, const point_r &v2) const
		{
			const real_t d01_w = w / d01;
			const real_t d12_w = w / d12;
			const real_t dx1 = d01_w * (v1.y - v0.y);
			const real_t dy1 = d01_w * (v1.x - v0.x);
			const real_t dx2 = d12_w * (v2.y - v1.y);
			const real_t dy2 = d12_w * (v2.x - v1.x);

			float x = 0.0f;
			float y = 0.0f;
			float x1 = 0.0f;
			float y1 = 0.0f;
			float x2 = 0.0f;
			float y2 = 0.0f;
			float k1 = 0.0f;
			float k2 = 0.0f;
			float b1 = 0.0f;
			float b2 = 0.0f;
			float m_x = 0.0f;
			float m_y = 0.0f;

			bool is_h1 = false;
			bool is_h2 = false;
			get_lines_midpoint(v0, d01, v1, d12, v2, m_x, m_y);

			k1 = get_new_edge_point(v0, v1, v2, m_x, m_y, v0.x, v0.y, w, x1, y1, is_h1);
			k2 = get_new_edge_point(v2, v1, v0, m_x, m_y, v2.x, v2.y, w, x2, y2, is_h2);

			if (k1 == 0 && k2 == 0 && is_h1 == is_h2)
			{
				return;
			}

			if (k1 != 0 && k2 != 0)
			{
				b1 = y1 - k1 * x1;
				b2 = y2 - k2 * x2;

				x = (b2 - b1) / (k1 - k2);
				y = k1 * x + b1;
			}
			else
			{
				if (k1 == 0)
				{
					if (is_h1)
					{
						y = y1;
						if (is_h2 || k2 == 0)
						{
							x = x2;
						}
						else
						{
							b2 = y2 - k2 * x2;
							x = (y + b2) / k2;
						}
					}
					else
					{
						x = x1;
						if (is_h2 || k2 == 0)
						{
							y = y2;
						}
						else
						{
							b2 = y2 - k2 * x2;
							y = k2 * x + b2;
						}
					}
				}
				else if (k2 == 0)
				{
					if (is_h2)
					{
						y = y2;
						if (is_h1 || k1 == 0)
						{
							x = x1;
						}
						else
						{
							b1 = y1 - k1 * x1;
							x = (y + b1) / k1;
						}
					}
					else
					{
						x = x2;
						if (is_h1 || k1 == 0)
						{
							y = y1;
						}
						else
						{
							b1 = y1 - k1 * x1;
							y = k1 * x + b1;
						}
					}
				}
			}

			output.push_back(create_point(v1.x + dx1, v1.y - dy1));
			output.push_back(create_point(x, y));
			output.push_back(create_point(v1.x + dx2, v1.y - dy2));
		}
	}
}
