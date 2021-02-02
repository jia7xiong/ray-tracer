#include "reflective_shader.h"
#include "ray.h"
#include "render_world.h"

vec3 Reflective_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& normal,int recursion_depth) const
{
    vec3 color;
    // determine the color
	color = (1 - reflectivity)*shader->Shade_Surface(ray, intersection_point, normal, 1);
	Ray reflected_ray;
	reflected_ray.endpoint = intersection_point;
	reflected_ray.direction = ray.direction - 2 * dot(ray.direction, normal) * normal;
	if (recursion_depth>0&&recursion_depth < world.recursion_depth_limit)
	{
		recursion_depth += 1;
		color = color + reflectivity * world.Cast_Ray(reflected_ray, recursion_depth);
	}
    return color;
}
