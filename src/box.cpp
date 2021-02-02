#include <limits>
#include "box.h"

// Return whether the ray intersects this box.
bool Box::Intersection(const Ray& ray) const
{
	double txl = (lo[0] - ray.endpoint[0]) / ray.direction[0];
	double txh = (hi[0] - ray.endpoint[0]) / ray.direction[0];
	double tyl = (lo[1] - ray.endpoint[1]) / ray.direction[1];
	double tyh = (hi[1] - ray.endpoint[1]) / ray.direction[1];
	double tzl = (lo[2] - ray.endpoint[2]) / ray.direction[2];
	double tzh = (hi[2] - ray.endpoint[2]) / ray.direction[2];
	if ((txl <= tyl && txh >= tyl) || (tyl <= txl && tyh >= txl))
		return false;
	else
	{
		double tlmin = std::min(txl, tyl);
		double tlmax = std::max(txl, tyl);
		double thmin = std::min(txh, tyh);
		double thmax = std::max(txh, tyh);
		if ((tzl <= tlmin && tzh >= tlmin) || (tzl <= tlmax && tzl >= thmin && tzh >= tlmax) || (tzl >= tlmax && tzl <= thmax))
			return false;
		else return true;
	}
}

// Compute the smallest box that contains both *this and bb.
Box Box::Union(const Box& bb) const
{
    Box box;
	box.hi = componentwise_max(bb.hi, this->hi);
	box.lo = componentwise_min(bb.lo, this->lo);
    return box;
}

// Enlarge this box (if necessary) so that pt also lies inside it.
void Box::Include_Point(const vec3& pt)
{
	hi = componentwise_max(pt, hi);
	lo = componentwise_min(pt, lo);
}

// Create a box to which points can be correctly added using Include_Point.
void Box::Make_Empty()
{
    lo.fill(std::numeric_limits<double>::infinity());
    hi=-lo;
}
