#include "utility.h"

#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"

int main()
{
	HittableList world;
	world.add(std::make_shared<Sphere>(point3(0, 0, -1), 0.5));
	world.add(std::make_shared<Sphere>(point3(0, -100.5, -1), 100));

	Camera camera;
	camera.aspect_ratio = 16.0 / 9.0;
	camera.image_width = 400;
	camera.samples_per_pixel = 100;

	camera.render(world);

	return 0;
}