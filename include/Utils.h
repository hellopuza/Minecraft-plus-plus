#ifndef UTILS_H
#define UTILS_H

#include "vec3.h"
#include "vec2.h"

#include <SFML/Graphics.hpp>

namespace puza {

const float PI = atanf(1.0f) * 4.0F;

typedef vec3f rgb;

inline bool isNIL (float num)
{
    return std::abs(num) < 1e-3F;
}

template <typename T>
inline const T& clamp (const T& value, const T& min, const T& max)
{
    if (value > max) return max;
    if (value < min) return min;
    return value;
}

template <typename T>
inline T loop (const T& value, const T& min, const T& max)
{
    T val = value;
    while (val > max) val = val - max + min;
    while (val < min) val = val + max - min;
    return val;
}

} // namespace puza

#endif // UTILS_H