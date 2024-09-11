#ifndef COLOR_H
#define COLOR_H

#include "interval.h"
#include "vec3.h"


using color = vec3;

void write_color(FILE* pFile, const color& pixel_color)
{
	auto r = pixel_color.x();
	auto g = pixel_color.y();
	auto b = pixel_color.z();

	static const Interval intensity(0.000, 0.999);
	int ir = static_cast<int>(255.999 * intensity.clamp(r));
	int ig = static_cast<int>(255.999 * intensity.clamp(g));
	int ib = static_cast<int>(255.999 * intensity.clamp(b));

	fprintf(pFile, "%d %d %d\n", ir, ig, ib);
}

#endif // !COLOR_H
