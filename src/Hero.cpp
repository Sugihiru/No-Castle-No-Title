#include <functional>
#include "Game.hpp"
#include "Hero.hpp"
#include "Projectile.hpp"


// Todo: create a Sprite class
Hero::Hero(const sf::Texture &texture,
           const Keymap keymap,
           const unsigned int map_width,
           const unsigned int map_height) : m_keymap(keymap),
                                            m_maxHp(100),
                                            m_hp(m_maxHp),
                                            m_maxEnergy(10000.0),
                                            m_energy(m_maxEnergy),
                                            m_mvtEnergyCost(0.5),
                                            m_mvtSpd(50),
                                            m_fireCooldown(sf::seconds(0.5)),
                                            m_fireEnergyCost(200.0),
                                            m_sprite_idx(0),
                                            m_spriteDirection(HeroDirection::DOWN)
{
    m_sprite.setTexture(texture);
    m_sprite.setTextureRect(sf::IntRect(0, SPRITE_HEIGHT * 2,
                                        SPRITE_WIDTH, SPRITE_HEIGHT));

    m_sprite.setPosition(map_width / 2, map_height / 2);
    m_hpBar = std::unique_ptr<Bar>(new Bar(sf::Color::Red,
                                           m_sprite.getPosition(),
                                           HP_BAR_Y_DIFF));
    m_energyBar = std::unique_ptr<Bar>(new Bar(sf::Color::Green,
                                               m_sprite.getPosition(),
                                               STAMINA_BAR_Y_DIFF));
    _initPressedKeys();
}

void Hero::setSpeed(const float spd)
{ m_mvtSpd = spd; }

void Hero::_initPressedKeys()
{
    const char *tab[] =
    {
        "MoveUp", "MoveDown", "MoveLeft", "MoveRight",
        "FireUp", "FireDown", "FireLeft", "FireRight"
    };

    for (auto &str_id : tab)
        m_pressedKey.insert(PressedKeysPair(str_id, false));
}


// You should not use this to initialize Hero
void Hero::reset(const unsigned int map_width,
                 const unsigned int map_height)
{
  Game  *game = Game::getInstance();

    m_sprite.setPosition(map_width / 2, map_height / 2);
    m_maxHp = 100;
    m_hp = m_maxHp;
    m_maxEnergy = 10000.0;
    m_energy = m_maxEnergy;
    m_mvtEnergyCost = 0.5;
    m_mvtSpd = (game->getLevel() == LUNATIC) ? 150 : 50;
    m_fireCooldown = sf::seconds(0.5);
    m_fireEnergyCost = 200.0;

    for (auto &it: m_pressedKey)
        it.second = false;
}

Hero::~Hero()
{
}


bool Hero::_testEvent(Key keys) const
{
    if (sf::Keyboard::isKeyPressed(keys.keyCode))
        return true;
    return false;
}

void Hero::checkInput()
{
    for (auto &key : m_keymap)
        m_pressedKey[key.first] = _testEvent(key.second);
}


void Hero::takeDmg(int dmg)
{
    if (m_hp > 0)
        m_hp -= dmg;
}


bool Hero::isDead()
{
    return m_hp <= 0 || m_energy <= 0;
}


void Hero::draw(sf::RenderWindow &win)
{
    sf::Vector2u win_size = win.getSize();

    if (m_energyLoseClock.getElapsedTime() > sf::milliseconds(100))
    {
        _lowerEnergy(STAMINA_LOSS_BY_TIME);
        m_energyLoseClock.restart();
    }

    if (!Game::getInstance()->isGameOver())
        _changePos(win_size);
    m_hpBar->setPosition(m_sprite.getPosition());
    m_hpBar->setPercentage(static_cast<float>(m_hp) /
                           static_cast<float>(m_maxHp));
    m_energyBar->setPosition(m_sprite.getPosition());
    m_energyBar->setPercentage(static_cast<float>(m_energy) /
                               static_cast<float>(m_maxEnergy));

    _updateSprite();
    win.draw(m_sprite);
    m_hpBar->draw(win);
    m_energyBar->draw(win);
}


void Hero::move(const float x, const float y,
                const sf::Vector2u win_size)
{
    const sf::IntRect &rect = m_sprite.getTextureRect();
    const sf::IntRect new_hitbox(x, y,
                                 rect.width, rect.height);

    if (x > 0 && y > 0 &&
        x < (win_size.x - rect.width) &&
        y < (win_size.y - rect.height) &&
        !Game::getInstance()->collideWithWall(new_hitbox))
        m_sprite.setPosition(x, y);
}


bool Hero::gotEnergy(const Energy &energy)
{
    const sf::IntRect hero_hitbox = getHitbox();
    bool got_energy = hero_hitbox.intersects(energy.getHitbox());

    if (got_energy)
        _upEnergy(energy.getEnergyValue());
    return (got_energy);
}


sf::Vector2f Hero::getPosition() const
{
    return m_sprite.getPosition();
}


const sf::IntRect Hero::getHitbox() const
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


void Hero::_changePos(sf::Vector2u &win_size)
{
    static std::map<std::string,
                    std::function<void(Hero *, sf::Vector2u &)> > _funcs = \
    {
        {"MoveUp", &Hero::_posGoUp},
        {"MoveDown", &Hero::_posGoDown},
        {"MoveLeft", &Hero::_posGoLeft},
        {"MoveRight", &Hero::_posGoRight},
        {"FireUp", &Hero::_fireUp},
        {"FireDown", &Hero::_fireDown},
        {"FireLeft", &Hero::_fireLeft},
        {"FireRight", &Hero::_fireRight}
    };

    for (auto &key: m_pressedKey)
    {
        if (key.second)
            _funcs[key.first](this, win_size);
    }
}


void Hero::_posGoUp(const sf::Vector2u &win_size)
{
    sf::Vector2f pos = m_sprite.getPosition();

    move(pos.x, pos.y - (m_mvtSpd / 10.0), win_size);
    _lowerEnergy(m_mvtEnergyCost);
    m_spriteDirection = HeroDirection::UP;
}


void Hero::_posGoDown(const sf::Vector2u &win_size)
{
    sf::Vector2f pos = m_sprite.getPosition();

    move(pos.x, pos.y + (m_mvtSpd / 10.0), win_size);
    _lowerEnergy(m_mvtEnergyCost);
    m_spriteDirection = HeroDirection::DOWN;
}


void Hero::_posGoLeft(const sf::Vector2u &win_size)
{
    sf::Vector2f pos = m_sprite.getPosition();

    move(pos.x - (m_mvtSpd / 10.0), pos.y, win_size);
    _lowerEnergy(m_mvtEnergyCost);
    m_spriteDirection = HeroDirection::LEFT;
}


void Hero::_posGoRight(const sf::Vector2u &win_size)
{
    sf::Vector2f pos = m_sprite.getPosition();

    move(pos.x + (m_mvtSpd / 10.0), pos.y, win_size);
    _lowerEnergy(m_mvtEnergyCost);
    m_spriteDirection = HeroDirection::RIGHT;
}


// Winsize is not used in this function
void Hero::_fireUp(const sf::Vector2u &win_size)
{
    Game *game = Game::getInstance();

    if ((m_clock.getElapsedTime() > m_fireCooldown || !m_hasAlreadyFired) &&
        m_energy >= m_fireEnergyCost)
    {
        game->spawnProjectile(m_sprite.getPosition(),
                              Direction::UP,
                              FiredBy::PLAYER, 20);
        m_clock.restart();
        m_hasAlreadyFired = true;
        _lowerEnergy(m_fireEnergyCost);
    }
    (void)win_size;
}


void Hero::_fireDown(const sf::Vector2u &win_size)
{
    Game *game = Game::getInstance();

    if ((m_clock.getElapsedTime() > m_fireCooldown || !m_hasAlreadyFired) &&
        m_energy >= m_fireEnergyCost)
    {
        game->spawnProjectile(m_sprite.getPosition(),
                              Direction::DOWN,
                              FiredBy::PLAYER, 20);
        m_clock.restart();
        m_hasAlreadyFired = true;
        _lowerEnergy(m_fireEnergyCost);
    }
    (void)win_size;
}


void Hero::_fireLeft(const sf::Vector2u &win_size)
{
    Game *game = Game::getInstance();

    if ((m_clock.getElapsedTime() > m_fireCooldown || !m_hasAlreadyFired) &&
        m_energy >= m_fireEnergyCost)
    {
        game->spawnProjectile(m_sprite.getPosition(),
                              Direction::LEFT,
                              FiredBy::PLAYER, 20);
        m_clock.restart();
        m_hasAlreadyFired = true;
        _lowerEnergy(m_fireEnergyCost);
    }
    (void)win_size;
}


void Hero::_fireRight(const sf::Vector2u &win_size)
{
    Game *game = Game::getInstance();

    if ((m_clock.getElapsedTime() > m_fireCooldown || !m_hasAlreadyFired) &&
        m_energy >= m_fireEnergyCost)
    {
        game->spawnProjectile(m_sprite.getPosition(),
                              Direction::RIGHT,
                              FiredBy::PLAYER, 20);
        m_clock.restart();
        m_hasAlreadyFired = true;
        _lowerEnergy(m_fireEnergyCost);
    }
    (void)win_size;
}


void Hero::_upEnergy(const float energy_value)
{
    float new_energy = m_energy + energy_value;
    m_energy = (new_energy <= m_maxEnergy) ? new_energy : m_maxEnergy;
}


void Hero::_lowerEnergy(const float energy_value)
{
    float new_energy = m_energy - energy_value;
    m_energy = (new_energy >= 0) ? new_energy : 0;
}


void Hero::_updateSprite()
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
