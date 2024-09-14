#ifndef COLOR_H
#define COLOR_H

#include "interval.h"
#include "vec3.h"


using color = vec3;

inline double linear_to_gamma(double linear_component)
{
	if (linear_component > 0)
		return std::sqrt(linear_component);

	return 0.0;
}

void write_color(FILE* pFile, const color& pixel_color)
{
	double r = pixel_color.x();
	double g = pixel_color.y();
	double b = pixel_color.z();

	r = linear_to_gamma(r);
	g = linear_to_gamma(g);
	b = linear_to_gamma(b);

	static const Interval intensity(0.000, 0.999);
	int ir = static_cast<int>(255.999 * intensity.clamp(r));
	int ig = static_cast<int>(255.999 * intensity.clamp(g));
	int ib = static_cast<int>(255.999 * intensity.clamp(b));

	fprintf(pFile, "%d %d %d\n", ir, ig, ib);
}

#endif // !COLOR_H
