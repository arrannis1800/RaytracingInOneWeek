#include "utility.h"

#include "hittable_list.h"
#include "sphere.h"
#include "material.h"
#include "camera.h"

int main()
{
	start_timer();
	auto material_ground = std::make_shared<Lambertian>(color(0.8, 0.8, 0.0));
	auto material_center = std::make_shared<Lambertian>(color(0.1, 0.2, 0.5));
	auto material_left = std::make_shared<Dielectric>(1.5);
	auto material_bubble = std::make_shared<Dielectric>(1.0 / 1.5);
	auto material_right = std::make_shared<Metal>(color(0.8, 0.6, 0.2), 1);

	HittableList world;
	world.add(std::make_shared<Sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
	world.add(std::make_shared<Sphere>(point3(0.0, 0.0, -1.2), 0.5, material_center));
	world.add(std::make_shared<Sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
	world.add(std::make_shared<Sphere>(point3(-1.0, 0.0, -1.0), 0.4, material_bubble));
	world.add(std::make_shared<Sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

	Camera camera;
	camera.aspect_ratio = 16.0 / 9.0;
	camera.image_width = 400;
	camera.samples_per_pixel = 100;
	camera.max_depth = 50;

	camera.render(world);
	stop_timer();
	return 0;
}