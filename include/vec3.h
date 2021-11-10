#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <functional>

namespace puza {

template <typename T>
struct vec3 final
{
    T x = 0;
    T y = 0;
    T z = 0;

    vec3 () {}

    vec3 (T x_, T y_, T z_) : x(x_), y(y_), z(z_) {}
    
    vec3 operator + (const vec3& v) const
    {
        return vec3(x + v.x,
                    y + v.y,
                    z + v.z);
    }
    
    vec3 operator - (const vec3& v) const
    {
        return vec3(x - v.x,
                    y - v.y,
                    z - v.z);
    }
    
    vec3 operator * (const vec3& v) const
    {
        return vec3(x * v.x,
                    y * v.y,
                    z * v.z);
    }

    vec3 operator / (const vec3& v) const
    {
        return vec3(x / v.x,
                    y / v.y,
                    z / v.z);
    }
    
    vec3 operator * (T k) const
    {
        return vec3(k * x,
                    k * y,
                    k * z);
    }

    vec3 operator / (T k) const
    {
        return vec3(x / k,
                    y / k,
                    z / k);
    }
    
    vec3& operator += (const vec3& v)
    {
        return *this = *this + v;
    }
    
    vec3& operator -= (const vec3& v)
    {
        return *this = *this - v;
    }

    vec3& operator *= (const vec3& v)
    {
        return *this = *this * v;
    }

    vec3& operator /= (const vec3& v)
    {
        return *this = *this / v;
    }
    
    vec3& operator *= (T k)
    {
        return *this = *this * k;
    }
    
    vec3& operator /= (T k)
    {
        return *this = *this / k;
    }
    
    vec3 operator - ()
    {
        return vec3(-x, -y, -z);
    }

    bool operator == (const vec3& obj) const
    {
        return (x == obj.x) && (y == obj.y) && (z == obj.z);
    }

    bool operator != (const vec3& obj) const
    {
        return (x != obj.x) || (y != obj.y) || (z != obj.z);
    }
    
    T magnitute2() const
    {
        return x * x + y * y + z * z;
    }
    
    T magnitute() const
    {
        return std::sqrt(magnitute2());
    }
    
    vec3& normalize()
    {
        return *this /= magnitute();
    }

    vec3 normalized()
    {
        return *this / magnitute();
    }

    template <typename TYPE>
    operator vec3<TYPE>() const
    {
        return vec3<TYPE>(static_cast<TYPE>(x), static_cast<TYPE>(y), static_cast<TYPE>(z));
    }
};

template <typename T>
inline vec3<T> cross (const vec3<T>& u, const vec3<T>& v)
{
    return vec3<T>(u.y * v.z - u.z * v.y,
                   u.z * v.x - u.x * v.z,
                   u.x * v.y - u.y * v.x);
}

template <typename T>
inline T dot (const vec3<T>& u, const vec3<T>& v)
{
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

template <typename T>
inline vec3<T> operator * (T k, const vec3<T>& v)
{
    return v * k;
}

template <typename T>
inline vec3<T> sign (const vec3<T>& v)
{
    const T ZERO    = static_cast<T>(0);
    const T POS_ONE = static_cast<T>(+1);
    const T NEG_ONE = static_cast<T>(-1);

    return vec3<T>((v.x > ZERO) ? POS_ONE : (v.x < ZERO) ? NEG_ONE : ZERO,
                   (v.y > ZERO) ? POS_ONE : (v.y < ZERO) ? NEG_ONE : ZERO,
                   (v.z > ZERO) ? POS_ONE : (v.z < ZERO) ? NEG_ONE : ZERO);
}

template <typename T>
inline vec3<T> round (const vec3<T>& v)
{
    return vec3<T>(round(v.x), round(v.y), round(v.z));
}

template <typename T>
inline vec3<T> abs(const vec3<T>& v)
{
    return vec3<T>(std::abs(v.x), std::abs(v.y), std::abs(v.z));
}

typedef vec3<int>      vec3i;
typedef vec3<float>    vec3f;
typedef vec3<double>   vec3d;
typedef vec3<unsigned> vec3u;

} // namespace puza

namespace std {

template <typename T>
struct hash<puza::vec3<T>>
{
    size_t operator()(const puza::vec3<T>& key) const
    {
        size_t x = hash<T>()(key.x);
        size_t y = hash<T>()(key.y);
        size_t z = hash<T>()(key.z);
        return (x * y * z) + (x ^ y ^ z);
    }
};

template <typename T>
struct equal_to<puza::vec3<T>>
{
    constexpr bool operator()(const puza::vec3<T>& lhs, const puza::vec3<T>& rhs) const
    {
        return lhs == rhs;
    }
};

} // namespace std

#endif // VEC3_H