#ifndef VEC2_H
#define VEC2_H

#include <cmath>
#include <functional>

namespace puza {

template <typename T>
struct vec2 final
{
    T x = 0;
    T y = 0;

    vec2 () {}

    vec2 (T x_, T y_) : x(x_), y(y_) {}
    
    vec2 operator + (const vec2& v) const
    {
        return vec2(x + v.x,
                    y + v.y);
    }
    
    vec2 operator - (const vec2& v) const
    {
        return vec2(x - v.x,
                    y - v.y);
    }
    
    vec2 operator * (const vec2& v) const
    {
        return vec2(x * v.x,
                    y * v.y);
    }

    vec2 operator / (const vec2& v) const
    {
        return vec2(x / v.x,
                    y / v.y);
    }
    
    vec2 operator * (T k) const
    {
        return vec2(k * x,
                    k * y);
    }

    vec2 operator / (T k) const
    {
        return vec2(x / k,
                    y / k);
    }

    vec2& operator += (const vec2& v)
    {
        return *this = *this + v;
    }
    
    vec2& operator -= (const vec2& v)
    {
        return *this = *this - v;
    }

    vec2& operator *= (const vec2& v)
    {
        return *this = *this * v;
    }

    vec2& operator /= (const vec2& v)
    {
        return *this = *this / v;
    }
    
    vec2& operator *= (T k)
    {
        return *this = *this * k;
    }
    
    vec2& operator /= (T k)
    {
        return *this = *this / k;
    }
    
    vec2 operator - ()
    {
        return vec2(-x, -y);
    }

    bool operator == (const vec2& obj) const
    {
        return (x == obj.x) && (y == obj.y);
    }

    bool operator != (const vec2& obj) const
    {
        return (x != obj.x) || (y != obj.y);
    }
    
    T magnitute2 () const
    {
        return x * x + y * y;
    }
    
    T magnitute () const
    {
        return std::sqrt(magnitute2());
    }
    
    vec2& normalize()
    {
        return *this /= magnitute();
    }

    vec2 normalized()
    {
        return *this / magnitute();
    }

    template <typename TYPE>
    operator vec2<TYPE>() const
    {
        return vec2<TYPE>(static_cast<TYPE>(x), static_cast<TYPE>(y));
    }
};

template <typename T>
inline vec2<T> operator * (T k, const vec2<T>& v)
{
    return v * k;
}

template <typename T>
inline T dot (const vec2<T>& u, const vec2<T>& v)
{
    return u.x * v.x + u.y * v.y;
}

template <typename T>
inline vec2<T> sign(const vec2<T>& v)
{
    const T ZERO    = static_cast<T>(0);
    const T POS_ONE = static_cast<T>(+1);
    const T NEG_ONE = static_cast<T>(-1);

    return vec2<T>((v.x > ZERO) ? POS_ONE : (v.x < ZERO) ? NEG_ONE : ZERO,
                   (v.y > ZERO) ? POS_ONE : (v.y < ZERO) ? NEG_ONE : ZERO);
}

template <typename T>
inline vec2<T> round(const vec2<T>& v)
{
    return vec2<T>(round(v.x), round(v.y));
}

template <typename T>
inline vec2<T> abs(const vec2<T>& v)
{
    return vec2<T>(std::abs(v.x), std::abs(v.y));
}

typedef vec2<int>      vec2i;
typedef vec2<float>    vec2f;
typedef vec2<double>   vec2d;
typedef vec2<unsigned> vec2u;

} // namespace puza

namespace std {

template <typename T>
struct hash<puza::vec2<T>>
{
    size_t operator()(const puza::vec2<T>& key) const
    {
        size_t x = hash<T>()(key.x);
        size_t y = hash<T>()(key.y);
        return (x * y) + (x ^ y);
    }
};

template <typename T>
struct equal_to<puza::vec2<T>>
{
    constexpr bool operator()(const puza::vec2<T>& lhs, const puza::vec2<T>& rhs) const
    {
        return lhs == rhs;
    }
};

} // namespace std

#endif // VEC2_H