#ifndef FPS_H
#define FPS_H

#include <SFML/Graphics.hpp>

namespace puza {

class FPS final
{
public:

    FPS(const sf::Font& font, sf::Vector2f position = sf::Vector2f(0.0f, 0.0f),
        unsigned int size = 20, sf::Color color = sf::Color::White);

    void update();
    void draw(sf::RenderWindow& window) const;

    float current_ = 0.0f;
    bool is_visible_ = true;

private:

    sf::Clock dt_clock_;
    sf::Text  fps_text_;
    const sf::Font& font_;

    size_t frame_num_ = 0;
};

} // namespace puza

#endif // FPS_H