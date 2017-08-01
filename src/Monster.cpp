#include "Game.hpp"
#include "Monster.hpp"


Monster::Monster(const sf::Texture &texture, int x, int y): m_maxHp(100),
                                                            m_hp(100),
                                                            m_mvtSpd(0.5),
                                                            m_sprite_idx(0),
                                                            m_spriteDirection(MonsterDirection::DOWN)
{
    m_sprite.setTexture(texture);
    m_sprite.setTextureRect(sf::IntRect(0, 0,
                                        SPRITE_WIDTH, SPRITE_HEIGHT));
    m_sprite.setPosition(x, y);
    m_hpBar = std::unique_ptr<Bar>(new Bar(sf::Color::Red,
                                           m_sprite.getPosition(),
                                           HP_BAR_Y_DIFF));

    m_nextFireTime = Game::getInstance()->getClockElapsedTime() + \
        sf::milliseconds(500);
}


Monster::~Monster()
{}


bool Monster::isDead()
{
    return m_hp <= 0;
}


void Monster::draw(sf::RenderWindow &win,
                   const sf::Vector2f &player_pos)
{
    if (!Game::getInstance()->isGameOver())
    {
        _changePos(player_pos);
        _shot();
    }

    m_hpBar->setPosition(m_sprite.getPosition());
    m_hpBar->setPercentage(static_cast<float>(m_hp) /
                           static_cast<float>(m_maxHp));

    _updateSprite();
    win.draw(m_sprite);
    m_hpBar->draw(win);
}


// Common between Hero and Monster
// Todo : move it to MovingThing
const sf::IntRect Monster::getHitbox() const
{
    const sf::IntRect texture_rect = m_sprite.getTextureRect();
    const sf::Vector2f pos = m_sprite.getPosition() + \
                             sf::Vector2f(texture_rect.width / 2,
                                          texture_rect.height / 2);
    sf::IntRect hitbox = sf::IntRect(pos.x, pos.y,
                                     texture_rect.width,
                                     texture_rect.height);
    return hitbox;
}


void Monster::takeDmg(int dmg)
{
    if (m_hp > 0)
        m_hp -= dmg;
}


const sf::Vector2f &Monster::getPosition() const
{
    return m_sprite.getPosition();
}


void Monster::incrementNextFireTime(sf::Time time_to_add)
{
    m_nextFireTime += time_to_add;
}

// Using Bresenham line algorithm to determine monster next position
void Monster::_changePos(const sf::Vector2f &player_pos)
{
    sf::Vector2f new_pos = m_sprite.getPosition();
    float dx;
    float dy;
    int incr_x;
    int incr_y;
    int err;

    dx = (player_pos.x - new_pos.x);
    dx = dx < 0 ? -dx : dx;
    dy = (player_pos.y - new_pos.y);
    dy = dy < 0 ? -dy : dy;
    incr_x = new_pos.x < player_pos.x ? 1 : -1;
    incr_y = new_pos.y < player_pos.y ? 1 : -1;
    err = (dx > dy ? dx : -dy) / 2;

    if (err > -dx)
        new_pos.x = new_pos.x + (incr_x / m_mvtSpd);
    if (err < dy)
        new_pos.y = new_pos.y + (incr_y / m_mvtSpd);

    const sf::IntRect &hitbox = getHitbox();
    sf::IntRect new_hitbox(new_pos.x, new_pos.y,
                           hitbox.width, hitbox.height);
    if (!Game::getInstance()->collideWithWall(new_hitbox))
        m_sprite.setPosition(new_pos);

    _rotateSprite();
}


void Monster::_shot()
{
    Game *game = Game::getInstance();
    int tor = (game->getLevel() == NORMAL) ? 0 : (game->getLevel() == DIFFICULT) ? 250 :
      (game->getLevel() == INSANE) ? 750 : 1500;
    int mul = (game->getLevel() == LUNATIC) ? 999 : tor;
    int dmg = 10 * static_cast<int>(game->getLevel());

    if (game->getClockElapsedTime() > m_nextFireTime)
    {
        game->spawnProjectile(m_sprite.getPosition(),
                              Direction::DIAGONAL,
                              FiredBy::MONSTER, dmg);
        m_nextFireTime = game->getClockElapsedTime() + \
                         sf::milliseconds(rand() % (1000 - mul) + 1500 - tor);
    }
}


void Monster::_rotateSprite()
{
    sf::Vector2f hero_pos = Game::getInstance()->getHeroPosition();
    // Hero size
    // Todo : put real hero size
    hero_pos += sf::Vector2f(32, 32);
    sf::Vector2f traj_vector = m_sprite.getPosition() - hero_pos;
    int abs_x = (traj_vector.x >= 0) ? traj_vector.x : -traj_vector.x;
    int abs_y = (traj_vector.y >= 0) ? traj_vector.y : -traj_vector.y;

    if (abs_x > abs_y)
    {
        if (traj_vector.x >= 0)
            m_spriteDirection = MonsterDirection::LEFT;
        else
            m_spriteDirection = MonsterDirection::RIGHT;
    }
    else
    {
        if (traj_vector.y >= 0)
            m_spriteDirection = MonsterDirection::UP;
        else
            m_spriteDirection = MonsterDirection::DOWN;
    }
}


void Monster::_updateSprite()
{
    if (m_sprite_clock.getElapsedTime() > SPRITE_REFRESH_CD)
    {
        m_sprite.setTextureRect(sf::IntRect((SPRITE_WIDTH * m_sprite_idx) %
                                            (SPRITE_WIDTH * SPRITE_FRM_NB),
                                            static_cast<int>(m_spriteDirection) * SPRITE_HEIGHT,
                                            SPRITE_WIDTH, SPRITE_HEIGHT));
        m_sprite_clock.restart();
        ++m_sprite_idx;
    }
}
