#ifndef LIGHT_H
#define LIGHT_H

#include "vec3.h"
#include "vec2.h"

namespace puza {

struct Light final
{
    vec3f position;
    rgb   color;
    float diffused_intensity;
    float specular_intensity;

    Light (vec3f pos, rgb col, float dif_intensity = 1.0f, float spec_intensity = 1.0f) :
        position (pos),
        color    (col),
        diffused_intensity (dif_intensity),
        specular_intensity (spec_intensity)
    {}
};

} // namespace puza

#endif //LIGHT_H