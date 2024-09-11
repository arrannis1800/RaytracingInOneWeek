#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class Ray
{
public:
	Ray() = delete;

	Ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}

	const point3& origin() { return orig; };
	const vec3& direction() { return dir; };

	point3 at(double t) const
	{
		return orig + dir * t;
	}

private:
	point3 orig;
	vec3 dir;
};

#endif // !RAY_H
