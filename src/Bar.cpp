#include "Bar.hpp"


Bar::Bar(const sf::Color color,
         const sf::Vector2f &pos, int y_diff): m_y_diff(y_diff)
{
    sf::Vector2f bar_pos = pos;
    bar_pos.y -= y_diff;

    m_outlineShape.setSize(OUTLINE_SIZE);
    m_outlineShape.setRotation(-90);
    m_outlineShape.setPosition(bar_pos);
    m_outlineShape.setOutlineColor(sf::Color::Black);
    m_outlineShape.setOutlineThickness(2);
    m_outlineShape.setFillColor(ALPHA_COLOR);

    m_fillShape.setSize(OUTLINE_SIZE);
    m_fillShape.setRotation(-90);
    m_fillShape.setPosition(bar_pos);
    m_fillShape.setFillColor(color);
    m_maxWidth = m_fillShape.getSize().y;
}


Bar::~Bar()
{

}


void Bar::draw(sf::RenderWindow &win)
{
    win.draw(m_fillShape);
    win.draw(m_outlineShape);
}


void Bar::setPosition(const sf::Vector2f &pos)
{
    sf::Vector2f bar_pos = pos;
    bar_pos.y -= m_y_diff;

    m_outlineShape.setPosition(bar_pos);
    m_fillShape.setPosition(bar_pos);
}

void Bar::setPercentage(const float percent)
{
    sf::Vector2f new_size = m_fillShape.getSize();
    float new_y = m_maxWidth * percent;

    if (new_size.y != new_y)
        new_size.y = new_y;
    m_fillShape.setSize(new_size);
}
