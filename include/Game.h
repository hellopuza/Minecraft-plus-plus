#ifndef GAME_H
#define GAME_H

#include "fonts/ArialFont.hpp"
#include "RayTracer.h"
#include "Person.h"
#include "FPS.h"

namespace puza {

const vec2u SCREEN_SIZE = vec2u(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height);

constexpr size_t DEFAULT_WIDTH  = 800;
constexpr size_t DEFAULT_HEIGHT = 450;

const vec3u DEFAULT_WORLD_SIZE = vec3u(256, 256, 128);

constexpr unsigned int DEFAULT_RAY_DEPTH = 2;
constexpr unsigned int DEFAULT_RAY_RANGE = 64;
constexpr float        DEFAULT_CAMERA_FOV = 33.0F;
constexpr size_t       MAX_BLOCK_SET_DISTANCE = 16;

const vec3f START_PERSON_POSITION = vec3f(20.5F, 40.5F, 2.0F);

const char * const DEFAULT_WORLD_NAME = "assets/worlds/world.dat";

const sf::String TITLE_STRING = "Minecraft++";

class Game final
{
public:

    Game();
    void run();

private:

    void toggleFullScreen();
    void updateWinSizes(const vec2u& size);
    void savePicture();
    void drawCross();

    vec2u            winsizes_;
    sf::RenderWindow window_;
    sf::Font         font_;
    World            world_;
    Person           person_;
    RayTracer        ray_tracer_;
    FPS              fps_;

    bool  mouse_visible_;
    block_t current_block_;
};

} // namespace puza

#endif // GAME_H