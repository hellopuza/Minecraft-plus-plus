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
inline void loop (T& value, const T& min, const T& max)
{
    while (value > max) value = value - max + min;
    while (value < min) value = value + max - min;
}

} // namespace puza

#endif // UTILS_H