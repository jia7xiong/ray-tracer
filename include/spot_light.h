#ifndef __SPOT_LIGHT_H__
#define __SPOT_LIGHT_H__

#include <math.h>
#include <vector>
#include <iostream>
#include <limits>
#include "vec.h"
#include "light.h"
#include <math.h>

class Spot_Light : public Light
{
public:
    double min_cos_angle; // cos(theta), where theta is the angle of the
                          // spotlight's cone.
    double falloff_exponent; // exponent that controls how quickly the spotlight
                             // falls off with angle from the cone's axis.
    vec3 direction; // Direction of the cone's axis.

    Spot_Light(const vec3& position,const vec3& color,double brightness,
        double max_angle,double falloff_exponent,const vec3& direction)
        :Light(position,color,brightness),min_cos_angle(cos(max_angle*pi/180)),
        falloff_exponent(falloff_exponent),direction(direction.normalized())
    {}

    vec3 Emitted_Light(const vec3& vector_to_light) const
    {
        // compute the emitted light for the spotlight.  Don't forget the
        // factor of 4*pi and the falloff (same as point light).
		double cos_angle = dot(vector_to_light.normalized(), direction.normalized());
		if (cos_angle >= min_cos_angle)
			return color * brightness* pow(cos_angle, falloff_exponent) / (4 * pi*vector_to_light.magnitude_squared());
		else
			return vec3();
    }
};
#endif
