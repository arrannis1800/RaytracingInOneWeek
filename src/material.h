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

class Dielectric : public Material
{
public:
	Dielectric(double refraction_index) : refraction_index(refraction_index) {}

	bool scatter(Ray& ray_in, HitRecord& rec, color& attenuation, Ray& scattered) const override
	{
		attenuation = color(1.0, 1.0, 1.0);
		double ri = rec.front_face ? (1.0 / refraction_index) : refraction_index;

		vec3 unit_direction = unit_vector(ray_in.direction());
		double cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
		double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

		bool cannot_refract = ri * sin_theta > 1.0;
		vec3 direction;
		if (cannot_refract || reflectance(cos_theta, ri) > random_double())
		{
			direction = reflect(unit_direction, rec.normal);
		}
		else
		{
			direction = refract(unit_direction, rec.normal, ri);
		}

		scattered = Ray(rec.p, direction);
		return true;
	}

private:
	double refraction_index;

	static double reflectance(double cosine, double refraction_index)
	{
		double r0 = (1 - refraction_index) / (1 + refraction_index);
		r0 = r0 * r0;
		return r0 * (1 - r0) * std::pow((1 - cosine), 5);
	}
};

#endif // !MATERIAL_H
