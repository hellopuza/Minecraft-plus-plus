#include "FPS.h"

namespace puza {

FPS::FPS(const sf::Font& font, sf::Vector2f position, unsigned int size, sf::Color color) :
    font_(font)
{
    fps_text_.setCharacterSize(size);
    fps_text_.setStyle(sf::Text::Bold);
    fps_text_.setFillColor(color);
    fps_text_.setPosition(position);

    fps_text_.setFont(font_);
}

void FPS::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (is_visible_) target.draw(fps_text_, states);
}

void FPS::update()
{
    ++frame_num_;
    static float dt = 0;
    dt += dt_clock_.restart().asSeconds();

    if (frame_num_ == 8)
    {
        char fps_str[32] = "";
        current_ = static_cast<float>(frame_num_) / dt;
        sprintf(fps_str, "FPS: %.0lf\n", current_);
        fps_text_.setString(fps_str);
        frame_num_ = 0;
        dt = 0;
    }
}

float FPS::current() const
{
    return current_;
}

} // namespace puza