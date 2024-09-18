#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <vector>

#include "utility.h"
#include "hittable.h"

class HittableList : public Hittable
{
public:
	std::vector<std::shared_ptr<Hittable>> objects;

	HittableList() = default;
	HittableList(std::shared_ptr<Hittable> object) { add(object); }
	
	void clear() { objects.clear(); }

	void add(std::shared_ptr<Hittable> object)
	{
		objects.push_back(object);
	}

	virtual bool hit(const Ray& ray, Interval ray_t, HitRecord& rec) const override
	{
		HitRecord temp_rec;
		bool hit_anything = false;
		double closest_so_far = ray_t.max;

		for (auto& object : objects)
		{
			if (object->hit(ray, Interval(ray_t.min, closest_so_far), temp_rec))
			{
				hit_anything = true;
				closest_so_far = temp_rec.t;
				rec = temp_rec;
			}
		}
		return hit_anything;
	}
};

#endif // !HITTABLE_LIST_H
