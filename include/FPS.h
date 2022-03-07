#ifndef FPS_H
#define FPS_H

#include <SFML/Graphics.hpp>

namespace puza {

class FPS final : public sf::Drawable
{
public:
    explicit FPS(const sf::Font& font, sf::Vector2f position = sf::Vector2f(0.0f, 0.0f),
        unsigned int size = 20, sf::Color color = sf::Color::White);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void update();
    float current() const;

    bool is_visible_ = true;

private:
    sf::Clock dt_clock_;
    sf::Text  fps_text_;
    const sf::Font& font_;

    float current_ = 0.0f;
    size_t frame_num_ = 0;
};

} // namespace puza

#endif // FPS_H