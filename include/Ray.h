#ifndef RAY_H
#define RAY_H

#include "vec3.h"

namespace puza {

struct Ray final
{
    vec3f origin;
    vec3f direction;

    Ray () {};

    Ray (const vec3f& origin_, const vec3f& direction_) :
        origin    (origin_),
        direction (direction_)
    {}

    vec3f point (float t) const
    {
        return origin + t * direction;
    }
};

} // namespace puza

#endif // RAY_H