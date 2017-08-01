#include "Energy.hpp"


Energy::Energy(const sf::Texture &texture,
               const sf::Vector2f &pos): m_lifetime(sf::seconds(5.0))
{
    m_sprite.setTexture(texture);
    m_sprite.setPosition(pos);
}


Energy::~Energy()
{
}


void Energy::draw(sf::RenderWindow &win)
{
    win.draw(m_sprite);
}


const sf::IntRect Energy::getHitbox() const
{
    const sf::IntRect texture_rect = m_sprite.getTextureRect();
    const sf::Vector2f pos = m_sprite.getPosition();
    sf::IntRect hitbox = sf::IntRect(pos.x + 32, pos.y + 32,
                                     texture_rect.width,
                                     texture_rect.height);
    return hitbox;
}


bool Energy::hasExpired()
{
    float ratio = m_clock.getElapsedTime().asSeconds() / m_lifetime.asSeconds();

    if (ratio >= START_FADING_VALUE)
        _applyFading(ratio);

    return m_clock.getElapsedTime() >= m_lifetime;
}


void Energy::_applyFading(float ratio)
{
    sf::Color end_color = m_sprite.getColor();
    int alpha_value = 255 - (255 * (ratio - START_FADING_VALUE) * 4) ;

    end_color.a = alpha_value;
    m_sprite.setColor(end_color);
}


float Energy::getEnergyValue() const
{
    return ENERGY_VALUE;
}
