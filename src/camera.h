#ifndef CAMERA_H
#define CAMERA_H

#include <thread>
#include <mutex>

#include "utility.h"
#include "material.h"
#include "hittable.h"

class Camera
{
public:
	double aspect_ratio = 1.0;
	int image_width = 100;
	int samples_per_pixel = 10;
	int max_depth = 10;

	double vfov = 90;
	point3 lookfrom = point3(0, 0, 0);
	point3 lookat = point3(0, 0, -1);
	vec3 vup = vec3(0, 1, 0);

	double defocus_angle = 0;
	double focus_dist = 10;

	void render(Hittable& world)
	{
		printf("Render start\n");
		initialize();

		configure_render_image(image_width, image_height);
		printf("Image params: %dx%d\n", image_height, image_width);
		std::vector<std::thread> threads;

		int num_threads = std::thread::hardware_concurrency();
		printf("Generating workers\n");
		int row_start = 0;
		for (int t = 0; t < num_threads; ++t)
		{
			threads.push_back(std::thread(&Camera::render_worker, this, std::ref(world)));
		}
		printf("All workers were generated\n");

		for (auto& t : threads)
		{
			t.join();
		}

		printf("Render done\n");
		write_file(image_width, image_height);
	}
private:
	int image_height;
	point3 center;
	point3 pixel100_loc;
	vec3 pixel_delta_u;
	vec3 pixel_delta_v;
	double pixel_samples_scale;
	vec3 u, v, w;
	vec3 defocus_disk_u;
	vec3 defocus_disk_v;

	int rows_rendered = 0;
	std::mutex m;

	void initialize()
	{
		image_height = static_cast<int>(image_width / aspect_ratio);
		image_height = (image_height < 1) ? 1 : image_height;

		center = lookfrom;

		double theta = degrees_to_radians(vfov);
		double h = std::tan(theta / 2);
		double viewport_height = 2 * h * focus_dist;
		double viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);

		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);

		vec3 viewport_u = viewport_width * u;
		vec3 viewport_v = viewport_height * -v;

		pixel_delta_u = viewport_u / image_width;
		pixel_delta_v = viewport_v / image_height;

		point3 viewport_upper_left = center - focus_dist * w - viewport_u / 2 - viewport_v / 2;
		pixel100_loc = viewport_upper_left + 0.5 * pixel_delta_u + 0.5 * pixel_delta_v;

		double defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
		defocus_disk_u = u * defocus_radius;
		defocus_disk_v = v * defocus_radius;

		pixel_samples_scale = 1.0 / samples_per_pixel;

	}
	color ray_color(const Ray& ray, int depth, Hittable& world) const
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

		point3 ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
		vec3 ray_direction = pixel_sample - ray_origin;

		return Ray(ray_origin, ray_direction);
	}

	vec3 sample_square()
	{
		return vec3(random_double() - 0.5, random_double() - 0.5, 0);
	}

	point3 defocus_disk_sample() const
	{
		vec3 p = random_in_unit_disk();
		return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
	}

	void render_worker(Hittable& world)
	{
		while (true)
		{
			int current_row = 0;
			m.lock();
			current_row = rows_rendered++;
			m.unlock();

			if (current_row >= image_height)
			{
				printf("\chunk is done\n");
				return;
			}
			printf("\rScanline: %d\n", current_row);
			for (int i = 0; i < image_width; ++i)
			{
				color pixel_color(0, 0, 0);
				for (int sample = 0; sample < samples_per_pixel; ++sample)
				{
					Ray ray = get_ray(i, current_row);
					pixel_color += ray_color(ray, max_depth, world);
				}
				rendered_image[current_row * image_width + i] = pixel_samples_scale * pixel_color;
			}
		}
	}
};

#endif // !CAMERA_H
