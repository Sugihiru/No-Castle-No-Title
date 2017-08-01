#ifndef MONSTER_HPP
#define MONSTER_HPP

#include "MovingThing.hpp"
#include "Bar.hpp"

enum class MonsterDirection : char
{
    DOWN = 0,
    LEFT = 1,
    RIGHT = 2,
    UP = 3
};

class Monster: MovingThing
{
public:
    Monster(const sf::Texture &texture, int x, int y);
    ~Monster();

    bool isDead();
    void draw(sf::RenderWindow &win,
              const sf::Vector2f &player_pos);
    const sf::IntRect getHitbox() const;
    void takeDmg(int dmg);
    const sf::Vector2f &getPosition() const;
    void incrementNextFireTime(sf::Time time_to_add);

private:
    void _changePos(const sf::Vector2f &player_pos);
    void _shot();
    void _rotateSprite();
    void _updateSprite();

    sf::Sprite m_sprite;
    std::unique_ptr<Bar> m_hpBar;

    // Stats
    int m_maxHp;
    int m_hp;
    float m_mvtSpd;
    sf::Time m_pauseTime;
    sf::Time m_nextFireTime;

    // Sprite
    sf::Clock m_sprite_clock;
    unsigned int m_sprite_idx;
    MonsterDirection m_spriteDirection;

    const int HP_BAR_Y_DIFF = 3;

    const int SPRITE_WIDTH = 64;
    const int SPRITE_HEIGHT = 64;
    const sf::Time SPRITE_REFRESH_CD = sf::milliseconds(200);
    const int SPRITE_FRM_NB = 3;

};

#endif // MONSTER_HPP
