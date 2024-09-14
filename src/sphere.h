#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"


class Sphere : public Hittable
{
public:
	Sphere(const point3& center, double radius, std::shared_ptr<Material> mat) : center(center), radius(std::fmax(0, radius)), mat(mat) {}

	virtual bool hit(Ray& ray, Interval ray_t, HitRecord& rec) const override
	{
		vec3 oc = center - ray.origin();
		double a = ray.direction().length_squared();
		double h = dot(ray.direction(), oc);
		double c = oc.length_squared() - radius * radius;

		double discriminant = h * h - a * c;
		if (discriminant < 0)
		{
			return false;
		}

		double sqrtd = std::sqrt(discriminant);

		double root = (h - sqrtd) / a;
		if (!ray_t.surrounds(root))
		{
			root = (h + sqrtd) / a;
			if (!ray_t.surrounds(root))
				return false;
		}

		rec.t = root;
		rec.p = ray.at(rec.t);
		vec3 outward_normal = (rec.p - center) / radius;
		rec.set_face_normal(ray, outward_normal);
		rec.mat = mat;

		return true;
	}

private:
	point3 center;
	double radius;
	std::shared_ptr<Material> mat;
};

#endif // !SPHERE_H
