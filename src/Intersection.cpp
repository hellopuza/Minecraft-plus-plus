#include "Intersection.h"

namespace puza {

Intersection::Intersection(const Ray& ray) : ray_(ray) {}

bool Intersection::intersect(World& world, size_t max_depth)
{
    vec3f current_pos = ray_.origin;
    vec3f dir = abs(ray_.direction);
    vec3f pos_dir(static_cast<float>(ray_.direction.x > 0.0),
                  static_cast<float>(ray_.direction.y > 0.0),
                  static_cast<float>(ray_.direction.z > 0.0));

    vec3f step = vec3f(2.0, 2.0, 2.0) * pos_dir - vec3f(1.0, 1.0, 1.0);
    vec3f pos_step(static_cast<float>(step.x > 0.0F),
                   static_cast<float>(step.y > 0.0F),
                   static_cast<float>(step.z > 0.0F));

    vec3f step_less_one(static_cast<float>(step.x < 1.0F),
                        static_cast<float>(step.y < 1.0F),
                        static_cast<float>(step.z < 1.0F));

    vec3f t_max(((vec3f(2.0, 2.0, 2.0) * pos_step - vec3f(1.0, 1.0, 1.0)) * (vec3f(vec3i(current_pos)) - current_pos) + pos_step) / dir);

    for (size_t i = 0; i < max_depth; i++)
    {
        vec3f min_t_max(static_cast<float>((t_max.x < t_max.y) && (t_max.x < t_max.z)),
                        static_cast<float>((t_max.y < t_max.x) && (t_max.y < t_max.z)),
                        static_cast<float>((t_max.z < t_max.x) && (t_max.z < t_max.y)));

        current_pos += step * min_t_max;
        t_max += min_t_max / dir;

        vec3f block_pos = vec3f(vec3i(current_pos));

        if (world.getBlock(block_pos) > 0U)
        {
            normal_ = min_t_max;
            side_centre_ = block_pos + min_t_max * step_less_one + vec3f(0.5F, 0.5F, 0.5F) * (vec3f(1.0F, 1.0F, 1.0F) - min_t_max);

            float dot_product = dot(ray_.direction, normal_);
            point_ = ray_.point(dot(side_centre_ - ray_.origin, normal_) / dot_product);
            block_pos_ = block_pos;
            normal_ = normal_ * (1.0F - 2.0F * static_cast<float>(dot_product > 0.0F));
            return true;
        }
    }

    return false;
}

vec3f Intersection::point() const
{
    return point_;
}

vec3f Intersection::normal() const
{
    return normal_;
}

vec3i Intersection::block_pos() const
{
    return block_pos_;
}

} // namespace puza