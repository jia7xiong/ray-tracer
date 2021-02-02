#include "mesh.h"
#include "plane.h"
#include <fstream>
#include <string>
#include <limits>

// Consider a triangle to intersect a ray if the ray intersects the plane of the
// triangle with barycentric weights in [-weight_tolerance, 1+weight_tolerance]
//static const double weight_tolerance = 1e-4;

// Read in a mesh from an obj file.  Populates the bounding box and registers
// one part per triangle (by setting number_parts).
void Mesh::Read_Obj(const char* file, std::string s0)
{
	s0 = "../" + s0;
    std::ifstream fin(file);
	std::ifstream fin1(s0.c_str());
    if(!fin && !fin1)
    {
        exit(EXIT_FAILURE);
    }
    std::string line;
    ivec3 e;
    vec3 v;
    box.Make_Empty();
    while(fin)
    {
        getline(fin,line);

        if(sscanf(line.c_str(), "v %lg %lg %lg", &v[0], &v[1], &v[2]) == 3)
        {
            vertices.push_back(v);
            box.Include_Point(v);
        }

        if(sscanf(line.c_str(), "f %d %d %d", &e[0], &e[1], &e[2]) == 3)
        {
            for(int i=0;i<3;i++) e[i]--;
            triangles.push_back(e);
        }
    }

	while(fin1)
    {
        getline(fin1,line);

        if(sscanf(line.c_str(), "v %lg %lg %lg", &v[0], &v[1], &v[2]) == 3)
        {
            vertices.push_back(v);
            box.Include_Point(v);
        }

        if(sscanf(line.c_str(), "f %d %d %d", &e[0], &e[1], &e[2]) == 3)
        {
            for(int i=0;i<3;i++) e[i]--;
            triangles.push_back(e);
        }
    }

    number_parts=triangles.size();
}

// Check for an intersection against the ray.  See the base class for details.
Hit Mesh::Intersection(const Ray& ray, int part) const
{
	double dist;
	if (part >= 0)
	{
		if(Intersect_Triangle(ray,part,dist))
			return { this,dist,part };
		else return { NULL,0,-1 };
	}
	else
	{
		int closest_hit = 0;
		bool hit = 0;
		double min_t = std::numeric_limits<double>::max();
		for (int i = 0; i < number_parts; i++)
		{
			if (Intersect_Triangle(ray, i, dist))
				if (dist<min_t)
				{
					min_t = dist;
					closest_hit = i;
					hit = 1;
				}
		}
		if (hit)
			return { this,min_t,closest_hit };
		else
			return { NULL,0,-1 };
	}
}

// Compute the normal direction for the triangle with index part.
vec3 Mesh::Normal(const vec3& point, int part) const
{
    assert(part>=0);
	vec3 A = vertices[triangles[part][0]];
	vec3 B = vertices[triangles[part][1]];
	vec3 C = vertices[triangles[part][2]];
	vec3 normal = cross((B - A),(C - A)).normalized();
	return normal;
}

// This is a helper routine whose purpose is to simplify the implementation
// of the Intersection routine.  It should test for an intersection between
// the ray and the triangle with index tri.  If an intersection exists,
// record the distance and return true.  Otherwise, return false.
// This intersection should be computed by determining the intersection of
// the ray and the plane of the triangle.  From this, determine (1) where
// along the ray the intersection point occurs (dist) and (2) the barycentric
// coordinates within the triangle where the intersection occurs.  The
// triangle intersects the ray if dist>small_t and the barycentric weights are
// larger than -weight_tolerance.  The use of small_t avoid the self-shadowing
// bug, and the use of weight_tolerance prevents rays from passing in between
// two triangles.
bool Mesh::Intersect_Triangle(const Ray& ray, int tri, double& dist) const
{
	vec3 point = vertices[triangles[tri][0]];
	vec3 normal = Normal(point, tri);
	Plane plane(point, normal);
	Hit hit = plane.Intersection(ray, -1);
	if(hit.object == NULL)
		return false;
	else
	{
		vec3 u = ray.direction;
		vec3 y = ray.endpoint-point;
		vec3 v = vertices[triangles[tri][1]] - point;
		vec3 w = vertices[triangles[tri][2]] - point;
		double gamma = dot(cross(u, v), y) / dot(cross(u, v), w);
		double beta = dot(cross(w, u), y) / dot(cross(w, u), v);
		double alpha = 1 - gamma - beta;
		if (gamma >= -weight_tol && beta >= -weight_tol && alpha >= -weight_tol)
		{
			dist = hit.dist;
			return true;
		}
		else return false;
	}
}

// Compute the bounding box.  Return the bounding box of only the triangle whose
// index is part.
Box Mesh::Bounding_Box(int part) const
{
    Box b;
	vec3 A = vertices[triangles[part][0]];
	vec3 B = vertices[triangles[part][1]];
	vec3 C = vertices[triangles[part][2]];
	b.hi = componentwise_max(componentwise_max(A, B), C);
	b.lo = componentwise_min(componentwise_min(A, B), C);
    return b;
}
