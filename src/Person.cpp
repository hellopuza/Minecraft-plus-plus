#include "Person.h"

namespace puza {

Person::Person(float camera_fov, float aspect_ratio, vec3f position) :
    camera_(camera_fov * PI / 180.0F, aspect_ratio),
    position_(position + vec3f(0.0F, 0.0F, HEIGHT_CORRECTION_))
{
    updateCameraPos();
}

void Person::turnHead(vec2f delta)
{
    camera_.direction_.x += delta.x * PI * 2.0F;
    camera_.direction_.y -= delta.y * PI / 2.0F;
    camera_.updateDirection();
}

void Person::setPosition(vec3f position)
{
    staying_ = false;
    position_ = position;
    velocity_ = vec3f(0.0F, 0.0F, 0.0F);
    updateCameraPos();
}

vec3f Person::getPosition() const
{
    return position_;
}

void Person::updateCameraPos()
{
    camera_.position_ = position_ + vec3f(0.0F, 0.0F, HEIGHT_);
}

void Person::shaking(vec3f movement)
{
    static float route = 0.0F;
    route = loop(route + movement.magnitute(), -PI, PI);
    camera_.position_ = position_ + vec3f(0.0F, 0.0F, HEIGHT_ + 0.05F * std::sin(3.0F * route));
}

void Person::move(const sf::Event& event, const World& world)
{
    bool wasd_control[4] = {};
    bool jump = false;
    bool boost = false;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))      wasd_control[0] = true;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))      wasd_control[1] = true;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))      wasd_control[2] = true;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))      wasd_control[3] = true;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))  jump = true;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) boost = true;

    if (event.type == sf::Event::KeyReleased)
    {
        if (event.key.code == sf::Keyboard::W)      wasd_control[0] = false;
        if (event.key.code == sf::Keyboard::A)      wasd_control[1] = false;
        if (event.key.code == sf::Keyboard::S)      wasd_control[2] = false;
        if (event.key.code == sf::Keyboard::D)      wasd_control[3] = false;
        if (event.key.code == sf::Keyboard::Space)  jump = false;
        if (event.key.code == sf::Keyboard::LShift) boost = false;
    }

    float speed = boost ? BOOST_SPEED_ : LINE_SPEED_;

    vec2f velocity;
    velocity.x = +(static_cast<float>(wasd_control[0]) - static_cast<float>(wasd_control[2])) * camera_.getForward().x
                 +(static_cast<float>(wasd_control[1]) - static_cast<float>(wasd_control[3])) * camera_.getRight().x;

    velocity.y = +(static_cast<float>(wasd_control[0]) - static_cast<float>(wasd_control[2])) * camera_.getForward().y
                 +(static_cast<float>(wasd_control[1]) - static_cast<float>(wasd_control[3])) * camera_.getRight().y;

    if (wasd_control[0] || wasd_control[1] || wasd_control[2] || wasd_control[3])
    {
        velocity.normalize();
    }

    velocity *= speed;
    velocity_ = vec3f(velocity.x, velocity.y, velocity_.z);

    if (jump && staying_)
    {
        velocity_.z = JUMP_SPEED_;
        jump = false;
        staying_ = false;
    }

    static sf::Clock delta_time_clock;
    float delta_time = delta_time_clock.restart().asSeconds();

    velocity_.z -= GRAVITY_ACCELERATION_ * delta_time;

    vec3f movement = velocity_ * delta_time - vec3f(0.0F, 0.0F, GRAVITY_ACCELERATION_) * delta_time * delta_time / 2.0F;

    collideRoof(world);
    collideFloor(world);
    if (staying_ && (movement.z < HEIGHT_CORRECTION_ * HEIGHT_CORRECTION_))
    {
        movement.z = 0.0F;
    }

    collideWalls(world, position_, movement);
    collideWalls(world, camera_.position_, movement);

    position_ += movement;

    if (staying_)
        shaking(movement);
    else
        updateCameraPos();
}

void Person::collideFloor(const World& world)
{
    Intersection intersection(Ray(position_, vec3f(0.0F, 0.0F, -1.0F)));
    if (intersection.intersect(world, 2) && (position_.z - intersection.point().z < HEIGHT_CORRECTION_ * 1.1F))
    {
        staying_ = true;
        position_.z = intersection.point().z + HEIGHT_CORRECTION_;
        velocity_.z = 0.0F;
    }
    else staying_ = false;
}

void Person::collideRoof(const World& world)
{
    Intersection intersection(Ray(camera_.position_, vec3f(0.0F, 0.0F, 1.0F)));
    if (intersection.intersect(world, 1) && (intersection.point().z - camera_.position_.z < HEIGHT_CORRECTION_))
    {
        position_.z = intersection.point().z - HEIGHT_ - HEIGHT_CORRECTION_;
        velocity_.z = 0.0F;
    }
}

void Person::collideWalls(const World& world, vec3f origin, vec3f& direction)
{
    Intersection intersection_x(Ray(origin, vec3f(direction.x, 0.0F, 0.0F).normalized()));
    Intersection intersection_y(Ray(origin, vec3f(0.0F, direction.y, 0.0F).normalized()));

    collideWall(world, intersection_x, origin, direction);
    collideWall(world, intersection_y, origin, direction);
}

void Person::collideWall(const World& world, Intersection intersection, vec3f origin, vec3f& direction)
{
    if (intersection.intersect(world, 1))
    {
        if ((intersection.point() - origin).magnitute() < direction.magnitute() + 0.1F)
        {
            direction += direction - 2.0F * dot(direction, intersection.normal()) * intersection.normal();
        }
    }
}

} // namespace puza