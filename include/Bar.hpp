#ifndef BAR_HPP
#define BAR_HPP

#include <SFML/Graphics.hpp>

class Bar
{
public:
    Bar(const sf::Color color,
        const sf::Vector2f &pos, int y_diff = 0);
    ~Bar();

    void draw(sf::RenderWindow &win);
    void setPosition(const sf::Vector2f &pos);
    void setPercentage(const float percent);

private:
    sf::RectangleShape m_outlineShape;
    sf::RectangleShape m_fillShape;

    int m_y_diff;
    unsigned int m_maxWidth;

    const sf::Vector2f OUTLINE_SIZE = sf::Vector2f(5, 64);
    const sf::Color ALPHA_COLOR = sf::Color(0, 0, 0, 0);
};

#endif // BAR_HPP
