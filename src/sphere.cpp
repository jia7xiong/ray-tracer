#include "sphere.h"
#include "ray.h"

// Determine if the ray intersects with the sphere
Hit Sphere::Intersection(const Ray& ray, int part) const
{
	double a = 1; //dot(ray.direction, ray.direction);
	double b = 2 * dot(ray.endpoint - center, ray.direction);
	double d = dot(ray.endpoint - center, ray.endpoint - center) - radius * radius;
	double det = b * b - 4 * a*d;
	if (det >= 0)
	{
		if (det==0)
		{
			double dist = -b / (2 * a);
			if (dist > small_t)
				return { this,dist,part };
			else
				return { NULL,0,part };
		}
		else
		{
			double dist1 = (-b + sqrt(det)) / (2 * a);
			if (dist1 < small_t)
				return { NULL,0,part };
			else
			{
				double dist2 = (-b - sqrt(det)) / (2 * a);
				if (dist1 < small_t)
					return { this,dist1,part };
				else 
					return { this,dist2,part };
			}
		}
	}
	else
		return {NULL,0,part};
}

vec3 Sphere::Normal(const vec3& point, int part) const
{
    vec3 normal;
    // compute the normal direction
	normal = (point - center).normalized();
    return normal;
}

Box Sphere::Bounding_Box(int part) const
{
    Box box;
    // calculate bounding box
	vec3 r;
	r.fill(radius);
	box.hi = center + r;
	box.lo = center - r;
    return box;
}
