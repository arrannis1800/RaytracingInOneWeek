#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"

class Camera
{
public:
	double aspect_ratio = 1.0;
	int image_width = 100;


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
				//color pixel_color = color(static_cast<double>(i) / image_width, static_cast<double>(j) / image_height, 0);

				vec3 pixel_center = pixel100_loc + i * pixel_delta_u + j * pixel_delta_v;
				vec3 ray_direction = pixel_center - center;
				Ray ray(center, ray_direction);

				color pixel_color = ray_color(ray, world);
				write_color(pFile, pixel_color);
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

	}
	color ray_color(Ray& ray, Hittable& world) const
	{
		HitRecord rec;
		if (world.hit(ray, Interval(0, infinity), rec)) {
			return 0.5 * (rec.normal + color(1, 1, 1));
		}


		vec3 unit_direction = unit_vector(ray.direction());
		double a = 0.5 * (unit_direction.y() + 1.0);
		color c = (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
		return c;
	}
};

#endif // !CAMERA_H
