#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include <functional>

#include <SFML/Graphics.hpp>

#include "Direction.hpp"

enum class FiredBy
{
    PLAYER,
    MONSTER
};

const float PROJECTILE_SPD = 75.0;

class Projectile;
using TrajFunc = std::function<sf::Vector2f(Projectile *proj,
                                            const sf::Vector2f &pos)>;
using TrajMapFunc = std::map<Direction, TrajFunc>;

class Projectile
{
public:
    Projectile(const sf::Texture &texture,
               const sf::Vector2f &pos,
               Direction direction,
               FiredBy fired_by,
               int dmg = 10);
    ~Projectile();

    void draw(sf::RenderWindow &win);
    bool isPlayerFire() const;
    const sf::IntRect getHitbox() const;
    int getDmg() const;

    sf::Vector2f trajUp(const sf::Vector2f &current_pos);
    sf::Vector2f trajDown(const sf::Vector2f &current_pos);
    sf::Vector2f trajLeft(const sf::Vector2f &current_pos);
    sf::Vector2f trajRight(const sf::Vector2f &current_pos);
    sf::Vector2f trajDiagonal(const sf::Vector2f &current_pos);

private:
    void _changePos();
    void _initDirectionalVector();
    void _rotate();

    sf::Sprite m_sprite;
    Direction m_direction;
    sf::Vector2f m_directionalVector;
    // std::function<sf::Vector2f(sf::Vector2f pos)> m_trajFunc;
    FiredBy m_firedBy;
    int m_dmg;
};

#endif // PROJECTILE_HPP
