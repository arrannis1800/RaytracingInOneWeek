#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

class Material
{
public:
	virtual ~Material() = default;
	virtual bool scatter(Ray& ray_in, HitRecord& rec, color& attenuation, Ray& scattered) const
	{
		return false;
	}
};

class Lambertian : public Material
{
public:
	Lambertian(const color& albedo) : albedo(albedo) {}

	bool scatter(Ray& ray_in, HitRecord& rec, color& attenuation, Ray& scattered) const override
	{
		vec3 scatter_direction = rec.normal + random_unit_vector();
		
		if (scatter_direction.near_zero())
			scatter_direction = rec.normal;

		scattered = Ray(rec.p, scatter_direction);
		attenuation = albedo;
		return true;
	}

private:
	color albedo;
};

class Metal : public Material
{
public:
	Metal(const color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz) {}

	bool scatter(Ray& ray_in, HitRecord& rec, color& attenuation, Ray& scattered) const override
	{
		vec3 reflected = reflect(ray_in.direction(), rec.normal);
		reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
		scattered = Ray(rec.p, reflected);
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal)>0);
	}

private:
	color albedo;
	double fuzz;
};

#endif // !MATERIAL_H
