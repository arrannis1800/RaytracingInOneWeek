#ifndef CAMERA_H
#define CAMERA_H

#include "material.h"
#include "hittable.h"

class Camera
{
public:
	double aspect_ratio = 1.0;
	int image_width = 100;
	int samples_per_pixel = 10;
	int max_depth = 10;

	void render(Hittable& world)
	{
		initialize();

		FILE* pFile = fopen("output.ppm", "w");

		if (!pFile)
		{
			printf("ERROR: file wasn't opened");
			return;
		}

		fprintf(pFile, "P3\n%d %d\n255\n", image_width, image_height);

		for (int j = 0; j < image_height; ++j)
		{
			printf("\rScanlines remaining: %d\n", image_height - j);
			for (int i = 0; i < image_width; ++i)
			{
				color pixel_color(0, 0, 0);
				for (int sample = 0; sample < samples_per_pixel; ++sample)
				{
					Ray ray = get_ray(i, j);
					pixel_color += ray_color(ray, max_depth, world);
				}
				write_color(pFile, pixel_samples_scale*pixel_color);
			}
		}
		
		fclose(pFile);
		printf("Render done");
	}
private:
	int image_height;
	point3 center;
	point3 pixel100_loc;
	vec3 pixel_delta_u;
	vec3 pixel_delta_v;
	double pixel_samples_scale;

	void initialize()
	{
		image_height = static_cast<int>(image_width / aspect_ratio);
		image_height = (image_height < 1) ? 1 : image_height;

		center = point3(0, 0, 0);

		double focal_length = 1.0;
		double viewport_height = 2.0;
		double viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);

		vec3 viewport_u = vec3(viewport_width, 0, 0);
		vec3 viewport_v = vec3(0, -viewport_height, 0);

		pixel_delta_u = viewport_u / image_width;
		pixel_delta_v = viewport_v / image_height;

		point3 viewport_upper_left = center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
		pixel100_loc = viewport_upper_left + 0.5 * pixel_delta_u + 0.5 * pixel_delta_v;

		pixel_samples_scale = 1.0 / samples_per_pixel;

	}
	color ray_color(Ray& ray, int depth, Hittable& world) const
	{
		if (depth <= 0)
			return color(0, 0, 0);

		HitRecord rec;
		if (world.hit(ray, Interval(0.001, infinity), rec)) {
			Ray scattered;
			color attenuation;
			if(rec.mat->scatter(ray, rec, attenuation, scattered))
				return attenuation * ray_color(scattered, --depth, world);
			return color(0,0,0);
		}


		vec3 unit_direction = unit_vector(ray.direction());
		double a = 0.5 * (unit_direction.y() + 1.0);
		color c = (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
		return c;
	}

	Ray get_ray(int i, int j)
	{
		vec3 offset = sample_square();
		point3 pixel_sample = pixel100_loc + (i + offset.x()) * pixel_delta_u + (j + offset.y()) * pixel_delta_v;

		point3 ray_origin = center;
		vec3 ray_direction = pixel_sample - ray_origin;

		return Ray(ray_origin, ray_direction);
	}

	vec3 sample_square()
	{
		return vec3(random_double() - 0.5, random_double() - 0.5, 0);
	}
};

#endif // !CAMERA_H
