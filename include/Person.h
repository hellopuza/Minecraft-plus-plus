#ifndef PERSON_H
#define PERSON_H

#include "Intersection.h"
#include "Camera.h"
#include "World.h"

namespace puza {

class Person final
{
public:

    Person(float camera_fov, float aspect_ratio, vec3f position);

    void turnHead(vec2f delta);
    void move(World& world);
    void setPosition(vec3f position);
    vec3f getPosition() const;

    Camera camera_;

private:

    void updateCameraPos();
    void shaking(vec3f movement);
    void collideFloor(World& world);
    void collideRoof(World& world);
    static void collideWalls(World& world, vec3f origin, vec3f& direction);
    static void collideWall(World& world, Intersection intersection, vec3f origin, vec3f& direction);

    const float HEIGHT_ = 1.7F;
    const float LINE_SPEED_ = 3.6F;
    const float BOOST_SPEED_ = 1.8F * LINE_SPEED_;
    const float JUMP_SPEED_ = 5.0F;
    const float GRAVITY_ACCELERATION_ = 12.0F;
    const float HEIGHT_CORRECTION_ = 0.05F;

    vec3f position_;
    vec3f velocity_;
    bool  staying_ = true;
};

} // namespace puza

#endif // PERSON_H