#include "Camera.h"

namespace puza {

Camera::Camera(float fov, float aspect_ratio) :
    fov_(fov)
{
    sizes_.y = std::tan(fov);
    sizes_.x = sizes_.y * aspect_ratio;
}

void Camera::updateDirection()
{
    direction_.y = clamp(direction_.y, -PI / 2.0F + 0.01F, PI / 2.0F - 0.01F);
    loop(direction_.x, -PI, PI);

    forward_ = vec3f(std::cos(direction_.y) * std::cos(direction_.x),
                     std::cos(direction_.y) * std::sin(-direction_.x),
                     std::sin(direction_.y));

    right_ = vec3f(std::sin(direction_.x), std::cos(direction_.x), 0.0F);

    up_ = cross(forward_, right_);
}

void Camera::updateAspectRatio(float aspect_ratio)
{
    sizes_.x = sizes_.y * aspect_ratio;
}

vec3f Camera::getForward() const
{
    return forward_;
}

vec3f Camera::getRight() const
{
    return right_;
}

vec3f Camera::getUp() const
{
    return up_;
}

} // namespace puza