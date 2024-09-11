#include "utility.h"

#include "hittable_list.h"
#include "sphere.h"

double aspect_ratio = 16.0 / 9.0;
int image_width = 400;

// Calculate the image height, and ensure that it's at least 1.
int image_height = (static_cast<int>(image_width / aspect_ratio) < 1) ? 1 : static_cast<int>(image_width / aspect_ratio);

double hitSphere(const point3& center, double sphere_radius, Ray& ray)
{
	vec3 oc = center - ray.origin();
	double a = ray.direction().length_squared();
	double h = dot(ray.direction(), oc);
	double c = oc.length_squared()  - sphere_radius * sphere_radius;
	double discriminant = h * h - a * c;
	if (discriminant < 0)
	{
		return -1.0;
	}
	else
	{
		return (h - std::sqrt(discriminant)) / a;
	}
}

color ray_color(Ray& ray, const Hittable& world) {
	HitRecord rec;
	if (world.hit(ray, Interval(0, infinity), rec)) {
		return 0.5 * (rec.normal + color(1, 1, 1));
	}


	vec3 unit_direction = unit_vector(ray.direction());
	double a = 0.5*(unit_direction.y() + 1.0);
	color c = (1.0 - a) * color(1.0, 1.0, 1.0) + a*color(0.5,0.7,1.0);
	return c;
}

// Viewport widths less than one are ok since they are real valued.
double focal_length = 1.0;
double viewport_height = 2.0;
double viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);
point3 camera_center = point3(0, 0, 0);

vec3 viewport_u = vec3(viewport_width, 0, 0);
vec3 viewport_v = vec3(0, -viewport_height, 0);

vec3 pixel_delta_u = viewport_u / image_width;
vec3 pixel_delta_v = viewport_v / image_height;

vec3 viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
vec3 pixel100_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

HittableList world;

void drawImage(FILE* pFile)
{
	fprintf(pFile, "P3\n%d %d\n255\n", image_width, image_height);

	for (int j = 0; j < image_height; ++j)
	{
		printf("\rScanlines remaining: %d\n", image_height - j);
		for (int i = 0; i < image_width; ++i)
		{
			//color pixel_color = color(static_cast<double>(i) / image_width, static_cast<double>(j) / image_height, 0);

			vec3 pixel_center = pixel100_loc + i * pixel_delta_u + j * pixel_delta_v;
			vec3 ray_direction = pixel_center - camera_center;
			Ray ray(camera_center, ray_direction);

			color pixel_color = ray_color(ray, world);
			write_color(pFile, pixel_color);
		}
	}


}

int main()
{
	FILE* pFile = nullptr;

	pFile = fopen("output.ppm", "w");

	if (!pFile)
	{
		printf("ERROR: file wasn't opened");
		return -1;
	}

	world.add(std::make_shared<Sphere>(point3(0, 0, -1), 0.5));
	world.add(std::make_shared<Sphere>(point3(0, -100.5, -1), 100));

	drawImage(pFile);

	fclose(pFile);

	vec3 v(1.0, 2.0, 0.0);
	printf("vec3: %s", vec3::toString(v).c_str());
	return 0;
}