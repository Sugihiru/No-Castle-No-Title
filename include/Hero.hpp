#ifndef HERO_HPP
#define HERO_HPP

#include "MovingThing.hpp"
#include "Bar.hpp"
#include "Keymap.hpp"
#include "Energy.hpp"

using PressedKeysMap = std::map<std::string, bool>;
using PressedKeysPair = std::pair<std::string, bool>;

enum class HeroDirection : char
{
    UP = 0,
    RIGHT = 1,
    DOWN = 2,
    LEFT = 3
};

class Hero: MovingThing
{
public:
    Hero(const sf::Texture &texture, const Keymap keymap,
         const unsigned int map_width,
         const unsigned int map_height);
    ~Hero();

    void reset(const unsigned int map_width,
               const unsigned int map_height);
    void checkInput();
    void takeDmg(int dmg);
    bool isDead();
    void draw(sf::RenderWindow &win);
    void move(const float x, const float y,
              const sf::Vector2u win_size);
    bool gotEnergy(const Energy &energy);
    sf::Vector2f getPosition() const;
    const sf::IntRect getHitbox() const;
    void setSpeed(const float spd);

private:
    void _initPressedKeys();
    bool _testEvent(Key keys) const;
    void _changePos(sf::Vector2u &win_size);
    void _posGoUp(const sf::Vector2u &win_size);
    void _posGoDown(const sf::Vector2u &win_size);
    void _posGoLeft(const sf::Vector2u &win_size);
    void _posGoRight(const sf::Vector2u &win_size);
    void _fireUp(const sf::Vector2u &win_size);
    void _fireDown(const sf::Vector2u &win_size);
    void _fireLeft(const sf::Vector2u &win_size);
    void _fireRight(const sf::Vector2u &win_size);
    void _upEnergy(const float energy_value);
    void _lowerEnergy(const float energy_value);
    void _updateSprite();

    const Keymap m_keymap;
    PressedKeysMap m_pressedKey;
    sf::Sprite m_sprite;
    std::unique_ptr<Bar> m_hpBar;
    std::unique_ptr<Bar> m_energyBar;

    // Stats of the hero
    int m_maxHp;
    int m_hp;
    float m_maxEnergy;
    float m_energy;
    float m_mvtEnergyCost;
    float m_mvtSpd;
    sf::Time m_fireCooldown;
    float m_fireEnergyCost;
    sf::Clock m_clock;
    bool m_hasAlreadyFired = false;

    sf::Clock m_energyLoseClock;

    // Sprite
    sf::Clock m_sprite_clock;
    unsigned int m_sprite_idx;
    HeroDirection m_spriteDirection;

    const int HP_BAR_Y_DIFF = 10;
    const int STAMINA_BAR_Y_DIFF = 3;
    const int STAMINA_LOSS_BY_TIME = 20;

    const int SPRITE_WIDTH = 64;
    const int SPRITE_HEIGHT = 64;
    const sf::Time SPRITE_REFRESH_CD = sf::milliseconds(200);
    const int SPRITE_FRM_NB = 3;
};

#endif // HERO_HPP
