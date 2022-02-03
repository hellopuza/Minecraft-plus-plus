#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "Ray.h"
#include "World.h"

namespace puza {

class Intersection final
{
public:

    explicit Intersection(const Ray& ray);

    bool intersect(const World& world, size_t max_depth);

    vec3f point() const;
    vec3f normal() const;
    vec3i block_pos() const;

private:

    Ray   ray_;
    vec3f normal_;
    vec3f point_;
    vec3f side_centre_;
    vec3i block_pos_;
};

} // namespace puza

#endif // INTERSECTION_H