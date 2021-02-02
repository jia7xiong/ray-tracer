#include "light.h"
#include "phong_shader.h"
#include "ray.h"
#include "render_world.h"
#include "object.h"
//#include <math.h>
//#include <algorithm>

vec3 Phong_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& normal,int recursion_depth) const
{
    vec3 color;
    //determine the color
	color = color_ambient * world.ambient_color* world.ambient_intensity;
	for (int i = 0; i < world.lights.size(); i++) 
	{
		Ray shadow_ray;
		shadow_ray.direction = (world.lights[i]->position - intersection_point).normalized();
		shadow_ray.endpoint = intersection_point;
        Hit hit=world.Closest_Intersection(shadow_ray);
        vec3 vec_to_light=world.lights[i]->position - intersection_point;
		if (hit.object == NULL||hit.dist>vec_to_light.magnitude()||!world.enable_shadows)
		{
			color += color_diffuse * world.lights[i]->Emitted_Light(vec_to_light)* std::max(dot(normal, shadow_ray.direction), 0.0);
			vec3 reflection = shadow_ray.direction - 2 * dot(shadow_ray.direction, normal) * normal;
			color += color_specular * world.lights[i]->Emitted_Light(vec_to_light)* pow(std::max(dot(ray.direction, reflection), 0.0), specular_power);
		}	
	}
    return color;
}
