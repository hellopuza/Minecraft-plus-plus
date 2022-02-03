#ifndef GAME_H
#define GAME_H

#include "RayTracer.h"
#include "Person.h"
#include "FPS.h"

namespace puza {

const vec2u SCREEN_SIZE = vec2u(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height);

constexpr size_t DEFAULT_WIDTH  = 800;
constexpr size_t DEFAULT_HEIGHT = 450;

constexpr unsigned int DEFAULT_RAY_DEPTH = 4;
constexpr unsigned int DEFAULT_RAY_RANGE = 64;
constexpr float        DEFAULT_CAMERA_FOV = 33.0F;
constexpr size_t       MAX_BLOCK_SET_DISTANCE = 16;

const vec3f START_PERSON_POSITION = vec3f(static_cast<float>(START_CHUNK_POS.x) + 0.4F,
                                          static_cast<float>(START_CHUNK_POS.y) + 0.4F, 2.0F);

const vec3f SUN_POSITION = vec3f(900.0F, -3500.0F, 2800.0F);

const char * const DEFAULT_WORLD_NAME = "assets/worlds/world.dat";
const char * const FONT_DESTINATION   = "assets/fonts/arial.ttf";

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