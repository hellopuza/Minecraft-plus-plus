#include "Game.h"

namespace puza {

Game::Game() :
    winsizes_     ({ DEFAULT_WIDTH, DEFAULT_HEIGHT }),
    window_       (sf::VideoMode(winsizes_.x, winsizes_.y), TITLE_STRING),
    world_        (DEFAULT_WORLD_NAME),
    person_       (DEFAULT_CAMERA_FOV, static_cast<float>(winsizes_.x) / static_cast<float>(winsizes_.y),
                   START_PERSON_POSITION),
    ray_tracer_   (winsizes_, &world_, &person_.camera_, DEFAULT_RAY_DEPTH, DEFAULT_RAY_RANGE),
    fps_          (font_, sf::Vector2f(0.0F, 0.0F), 16, sf::Color::White),
    mouse_visible_(false),
    current_block_(BLOCK_GRASS)
{
    window_.setMouseCursorVisible(mouse_visible_);

    font_.loadFromFile(FONT_DESTINATION);

    ray_tracer_.addLight(vec3f(900.0F, -3500.0F, 2800.0F), rgb(1.0F, 1.0F, 1.0F), 1.0F, 1.0F);
    ray_tracer_.addLight(vec3f(42.5F, 54.5F, 3.5F), rgb(1.0F, 1.0F, 1.0F), 1.0F, 1.0F);

    world_.update();

    ray_tracer_.makeShader();
}

void Game::run()
{
    while (window_.isOpen())
    {
        sf::Event event;
        while (window_.pollEvent(event))
        {
            //Close window
            if ((event.type == sf::Event::Closed) ||
                ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape) && (mouse_visible_)))
            {
                window_.close();
                break;
            }

            //Toggle mouse visibility
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
            {
                mouse_visible_ = true;
                window_.setMouseCursorVisible(mouse_visible_);
            }
            else if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && mouse_visible_)
            {
                mouse_visible_ = false;
                window_.setMouseCursorVisible(mouse_visible_);
            }

            //Toggle fullscreen
            else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::F11))
            {
                toggleFullScreen();
            }

            //Resize window
            else if (event.type == sf::Event::Resized)
            {
                sf::FloatRect visible_area(0.0F, 0.0F, static_cast<float>(event.size.width), static_cast<float>(event.size.height));
                window_.setView(sf::View(visible_area));
                updateWinSizes(vec2u(window_.getSize().x, window_.getSize().y));
            }

            //Camera rotation
            else if ((event.type == sf::Event::MouseMoved) && (not mouse_visible_))
            {
                static sf::Vector2i last_mouse_pos = sf::Mouse::getPosition();
                sf::Vector2i delta_pos = sf::Mouse::getPosition() - last_mouse_pos;

                sf::Mouse::setPosition(sf::Vector2i(window_.getSize() / 2U), window_);
                last_mouse_pos = sf::Mouse::getPosition();

                person_.turnHead(vec2f(static_cast<float>(delta_pos.x) / static_cast<float>(SCREEN_SIZE.x),
                                       static_cast<float>(delta_pos.y) / static_cast<float>(SCREEN_SIZE.y)));
            }
            else if ((event.type == sf::Event::MouseLeft) && (not mouse_visible_))
            {
                sf::Mouse::setPosition(sf::Vector2i(window_.getSize() / 2U), window_);
            }

            //Set block
            else if ((event.type == sf::Event::MouseButtonPressed) && (event.mouseButton.button == sf::Mouse::Left))
            {
                Intersection cam_intersection(Ray(person_.camera_.position_, person_.camera_.getForward()));
                if (cam_intersection.intersect(world_, MAX_BLOCK_SET_DISTANCE))
                {
                    vec3f relative_point = cam_intersection.point() - vec3f(cam_intersection.block_pos());
                    vec3i relative_block_center;

                    if (isNIL(relative_point.y - 0.0F)) relative_block_center = vec3i( 0, -1,  0);
                    if (isNIL(relative_point.x - 1.0F)) relative_block_center = vec3i( 1,  0,  0);
                    if (isNIL(relative_point.y - 1.0F)) relative_block_center = vec3i( 0,  1,  0);
                    if (isNIL(relative_point.x - 0.0F)) relative_block_center = vec3i(-1,  0,  0);
                    if (isNIL(relative_point.z - 1.0F)) relative_block_center = vec3i( 0,  0,  1);
                    if (isNIL(relative_point.z - 0.0F)) relative_block_center = vec3i( 0,  0, -1);

                    vec3i block_pos = cam_intersection.block_pos() + relative_block_center;

                    if (block_pos != vec3i(person_.getPosition()) && block_pos != vec3i(person_.camera_.position_))
                    {
                        world_.setBlock(cam_intersection.block_pos() + relative_block_center, current_block_);
                        world_.update();
                    }
                }
            }
            else if ((event.type == sf::Event::MouseButtonPressed) && (event.mouseButton.button == sf::Mouse::Right))
            {
                Intersection cam_intersection(Ray(person_.camera_.position_, person_.camera_.getForward()));
                if (cam_intersection.intersect(world_, MAX_BLOCK_SET_DISTANCE))
                {
                    world_.setBlock(cam_intersection.block_pos(), BLOCK_AIR);
                    world_.update();
                }
            }

            //Change block
            else if (event.type == sf::Event::MouseWheelMoved)
            {
                int current_block = event.mouseWheel.delta + current_block_;

                while (current_block > static_cast<int>(MATERIALS_NUM) - 1)
                    current_block = current_block - static_cast<int>(MATERIALS_NUM) + 1;

                while (current_block_ < 1)
                    current_block = current_block + static_cast<int>(MATERIALS_NUM) - 1;

                current_block_ = static_cast<block_t>(current_block);
            }

            else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::R))
            {
                person_.setPosition(START_PERSON_POSITION);
            }
        }
        static float t = 0;
        t += 1 / (fps_.current() + 1.0F);
        ray_tracer_.setLightColor(rgb(powf(sinf(11.0F * t), 2.0F), powf(sinf(3.0F * t), 2.0F), powf(sinf(5.0F * t), 2.0F)), 1);

        if (not mouse_visible_) person_.move(event, world_);

        ray_tracer_.draw(window_);
        fps_.update();
        window_.draw(fps_);

        drawCross();

        window_.display();
    }

    world_.save(DEFAULT_WORLD_NAME);
}

void Game::updateWinSizes(const vec2u& size)
{
    winsizes_ = size;
    ray_tracer_.updateWinSizes(winsizes_);
    person_.camera_.updateAspectRatio(static_cast<float>(winsizes_.x) / static_cast<float>(winsizes_.y));
}

void Game::toggleFullScreen()
{
    if ((winsizes_.x == sf::VideoMode::getDesktopMode().width) && (winsizes_.y == sf::VideoMode::getDesktopMode().height))
    {
        window_.create(sf::VideoMode(DEFAULT_WIDTH, DEFAULT_HEIGHT), TITLE_STRING);
        updateWinSizes(vec2u(DEFAULT_WIDTH, DEFAULT_HEIGHT));
    }
    else
    {
        window_.create(sf::VideoMode::getDesktopMode(), TITLE_STRING, sf::Style::Fullscreen);
        updateWinSizes(vec2u(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height));
    }

    window_.setMouseCursorVisible(mouse_visible_);
}

void Game::savePicture()
{
    static int  shot_num = 0;
    std::string filename = "screenshot(" + std::to_string(shot_num++) + ")" + ".png";

    sf::Texture screen;
    screen.create(window_.getSize().x, window_.getSize().y);
    screen.update(window_);

    sf::RectangleShape rectangle;
    rectangle.setPosition(0, 0);
    rectangle.setSize(sf::Vector2f(window_.getSize()));

    if (screen.copyToImage().saveToFile(filename))
        rectangle.setFillColor(sf::Color(10, 10, 10, 150));  // grey screen if ok
    else
        rectangle.setFillColor(sf::Color(255, 10, 10, 200)); // red screen if error

    window_.draw(rectangle);
    window_.display();

    sf::sleep(sf::milliseconds(300));

    sf::Sprite screen_sprite(screen);
    window_.draw(screen_sprite);
    window_.display();
}

void Game::drawCross()
{
    const float size = 7.0F;
    sf::Vertex line1[] =
    {
        sf::Vertex(sf::Vector2f(static_cast<float>(winsizes_.x) / 2.0F - size,
                                static_cast<float>(winsizes_.y) / 2.0F), sf::Color(200, 200, 200)),

        sf::Vertex(sf::Vector2f(static_cast<float>(winsizes_.x) / 2.0F + size,
                                static_cast<float>(winsizes_.y) / 2.0F), sf::Color(200, 200, 200))
    };

    sf::Vertex line2[] =
    {
        sf::Vertex(sf::Vector2f(static_cast<float>(winsizes_.x) / 2.0F,
                                static_cast<float>(winsizes_.y) / 2.0F - size), sf::Color(200, 200, 200)),

        sf::Vertex(sf::Vector2f(static_cast<float>(winsizes_.x) / 2.0F,
                                static_cast<float>(winsizes_.y) / 2.0F + size), sf::Color(200, 200, 200))
    };

    window_.draw(line1, 2, sf::Lines);
    window_.draw(line2, 2, sf::Lines);

    sf::Text position;

    position.setCharacterSize(16);
    position.setStyle(sf::Text::Bold);
    position.setFillColor(sf::Color::White);
    position.setPosition(sf::Vector2f(100.0F, 0.0F));

    position.setFont(font_);

    char pos[512] = "";
    sprintf(pos, "x: %.2f, y: %.2f, z: %.2f\n", person_.getPosition().x, person_.getPosition().y, person_.getPosition().z);
    position.setString(pos);
    window_.draw(position);
}

} // namespace puza