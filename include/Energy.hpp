#ifndef ENERGY_HPP
#define ENERGY_HPP

#include <SFML/Graphics.hpp>

class Energy
{
public:
    Energy(const sf::Texture &texture,
           const sf::Vector2f &pos);
    ~Energy();

    void draw(sf::RenderWindow &win);
    const sf::IntRect getHitbox() const;
    bool hasExpired();
    float getEnergyValue() const;

private:
    void _applyFading(float ratio);

    sf::Sprite m_sprite;
    sf::Time m_lifetime;
    sf::Clock m_clock;

    const float START_FADING_VALUE = 0.75;
    const float ENERGY_VALUE = 1000.0;
};

#endif // ENERGY_HPP
