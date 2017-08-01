#include <cmath>
#include "Game.hpp"
#include "Projectile.hpp"


Projectile::Projectile(const sf::Texture &texture,
                       const sf::Vector2f &pos,
                       Direction direction,
                       FiredBy fired_by,
                       int dmg): m_direction(direction),
                                 m_firedBy(fired_by),
                                 m_dmg(dmg)
{
    // Note : 32 == player_size / 2
    // Todo : get the real player/monster_size as parameter
    sf::Vector2f real_pos = pos + sf::Vector2f(32, 32);
    m_sprite.setOrigin(32, 32);
    m_sprite.setPosition(real_pos);
    m_sprite.setTexture(texture);
    if (m_firedBy == FiredBy::MONSTER)
        m_sprite.setColor(sf::Color::Red);
    _initDirectionalVector();
}


void Projectile::_initDirectionalVector()
{
    static TrajMapFunc traj_map_func =
    {
        {Direction::UP, &Projectile::trajUp},
        {Direction::DOWN, &Projectile::trajDown},
        {Direction::LEFT, &Projectile::trajLeft},
        {Direction::RIGHT, &Projectile::trajRight},
        {Direction::DIAGONAL, &Projectile::trajDiagonal},
    };

    m_directionalVector = traj_map_func[m_direction](this, m_sprite.getPosition());
    _rotate();
}


void Projectile::_rotate()
{
    double angle;
    const sf::Vector2f &pos = m_directionalVector;

    angle = (std::atan2(pos.y, pos.x) / M_PI) * 180.0;
    m_sprite.rotate(angle);
}

Projectile::~Projectile()
{
}


void Projectile::draw(sf::RenderWindow &win)
{
    if (!Game::getInstance()->isGameOver())
        _changePos();
    win.draw(m_sprite);
}


void Projectile::_changePos()
{
    sf::Vector2f pos = m_sprite.getPosition();
    pos -= m_directionalVector;
    m_sprite.setPosition(pos);
}


bool Projectile::isPlayerFire() const
{
    return m_firedBy == FiredBy::PLAYER;
}


const sf::IntRect Projectile::getHitbox() const
{
    const sf::IntRect texture_rect = m_sprite.getTextureRect();
    const sf::Vector2f pos = m_sprite.getPosition();
    sf::IntRect hitbox = sf::IntRect(pos.x, pos.y,
                                     texture_rect.width,
                                     texture_rect.height);
    return hitbox;
}


int Projectile::getDmg() const
{
    return m_dmg;
}


sf::Vector2f Projectile::trajUp(const sf::Vector2f &current_pos)
{
    (void)current_pos;
    return sf::Vector2f(0, PROJECTILE_SPD / 10.0);
}


sf::Vector2f Projectile::trajDown(const sf::Vector2f &current_pos)
{
    (void)current_pos;
    return sf::Vector2f(0, -PROJECTILE_SPD / 10.0);
}


sf::Vector2f Projectile::trajLeft(const sf::Vector2f &current_pos)
{
    (void)current_pos;
    return sf::Vector2f(PROJECTILE_SPD / 10.0, 0);
}


sf::Vector2f Projectile::trajRight(const sf::Vector2f &current_pos)
{
    (void)current_pos;
    return sf::Vector2f(-PROJECTILE_SPD / 10.0, 0);
}


sf::Vector2f Projectile::trajDiagonal(const sf::Vector2f &current_pos)
{
    // Get Directional Vector
    sf::Vector2f hero_pos = Game::getInstance()->getHeroPosition();
    // Hero size
    // Todo : put real hero size
    hero_pos += sf::Vector2f(32, 32);
    sf::Vector2f traj_vector = current_pos - hero_pos;

    traj_vector.x /= 500.0;
    traj_vector.y /= 500.0;

    // Adjust speed so speed == PROJECTILE_SPD
    float spd = sqrt(pow(traj_vector.x, 2) + pow(traj_vector.y, 2));
    traj_vector.x *= (PROJECTILE_SPD / 10.0 / spd);
    traj_vector.y *= (PROJECTILE_SPD / 10.0 / spd);

    return (traj_vector);
}
