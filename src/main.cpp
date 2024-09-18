#include "utility.h"

#include "hittable_list.h"
#include "sphere.h"
#include "material.h"
#include "camera.h"

int main()
{
	start_timer();
	HittableList world;

	auto material_ground = std::make_shared<Lambertian>(color(0.5, 0.5, 0.5));
	world.add(std::make_shared<Sphere>(point3(0.0, -1000.0, 0.0), 1000.0, material_ground));

	for (int a = -11; a < 11; ++a)
	{
		for (int b = -11; b < 11; ++b)
		{
			double choose_mat = random_double();
			point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

			if ((center - point3(4, 0.2, 0)).length() > 0.9)
			{
				std::shared_ptr<Material> sphere_material;

				if (choose_mat < 0.8)
				{
					color albedo = color::random() * color::random();
					sphere_material = std::make_shared<Lambertian>(albedo);
				}
				else if (choose_mat < 0.95)
				{
					color albedo = color::random(0.5, 1);
					double fuzz = random_double(0, 0.5);
					sphere_material = std::make_shared<Metal>(albedo, fuzz);
				}
				else
				{
					sphere_material = std::make_shared<Dielectric>(1.5);
				}

				world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
			}
		}
	}
	auto material1 = std::make_shared<Dielectric>(1.5);
	world.add(std::make_shared<Sphere>(point3(0, 1, 0), 1.0, material1));

	auto material2 = std::make_shared<Lambertian>(color(0.4, 0.2, 0.1));
	world.add(std::make_shared<Sphere>(point3(-4, 1, 0), 1.0, material2));

	auto material3 = std::make_shared<Metal>(color(0.7, 0.6, 0.5), 0.0);
	world.add(std::make_shared<Sphere>(point3(4, 1, 0), 1.0, material3));

	Camera camera;
	camera.aspect_ratio = 16.0 / 9.0;
	camera.image_width = 400;
	camera.samples_per_pixel = 100;
	camera.max_depth = 50;
	camera.vfov = 20;
	camera.lookfrom = point3(13, 2, 3);
	camera.lookat = point3(0, 0, 0);
	camera.vup = vec3(0, 1, 0);
	camera.defocus_angle = 0.6;
	camera.focus_dist = 10.0;

	camera.render(world);
	stop_timer();
	return 0;
}