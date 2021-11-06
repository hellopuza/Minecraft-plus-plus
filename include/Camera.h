#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.h"
#include "vec2.h"
#include "Utils.h"

namespace puza {

class Camera final
{
public:
    Camera(float fov, float aspect_ratio);

    void updateDirection();
    void updateAspectRatio(float aspect_ratio);

    vec3f getForward() const;
    vec3f getRight() const;
    vec3f getUp() const;

    vec2f direction_;
    vec3f position_;
    
    float fov_;
    vec2f sizes_;

private:

    vec3f forward_;
    vec3f right_;
    vec3f up_;
};

} // namespace puza

#endif // CAMERA_H