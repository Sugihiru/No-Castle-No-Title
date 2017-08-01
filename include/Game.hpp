#ifndef GAME_HPP
#define GAME_HPP

#include <map>
#include <deque>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Menu.hpp"
#include "Map.hpp"
#include "Hero.hpp"
#include "Monster.hpp"
#include "Projectile.hpp"
#include "Keymap.hpp"
#include "Energy.hpp"
#include "Pause.hpp"
#include "gameover.hpp"
#include "Direction.hpp"

using TextureMap = std::map<std::string, sf::Texture>;
using TexturePair = std::pair<std::string, sf::Texture>;
using SoundMap = std::map<std::string, std::pair<sf::SoundBuffer, sf::Sound> >;
using SoundPair = std::pair<std::string, std::pair<sf::SoundBuffer, sf::Sound> >;

enum Whoisrunning
  {
    MENU,
    GAME,
    PAUSE,
    HELP
  };


// Main class
// Contains everything needed : textures, musics, font, and every element of the game
// Is a singleton
class Game
{
public:
    static Game *instance;
    static Game *getInstance();
    Game(const Game &other) = delete;
    Game &operator=(const Game &other) = delete;
    ~Game();

    bool isRunning() const;
    void display();
    void pollEvents();
    void spawnProjectile(const sf::Vector2f &pos,
                         Direction direction,
                         FiredBy fired_by,
                         int dmg = 10);
    void resetGame();
    bool isInGame() const;
    bool collideWithWall(const sf::IntRect &hitbox) const;
    const sf::Vector2u getWinSize() const;

    bool isGameOver() const;
    sf::Vector2f getHeroPosition() const;
    sf::Time getClockElapsedTime() const;
    void setNextMobSpawnTime();
    void setNextEnergyTime();
    void setTimeInPause();
    void incrementMobsNextFireTime();

    void setWho(Whoisrunning who);
    Whoisrunning getWho(void) const;
    void dispMenu();
    void setLevel(const difficulty level);
    difficulty getLevel() const;
    void setSound(const bool sound);
    void setVolume(const int volume);
    void closewin();
    int getVolume(void);

private:
    Game();
    // Initialization
    void _initFont();
    void _initTextures();
    void _initSounds();
    void _initPlayerOneKeymap();
    Key _initKey(sf::Event::EventType event_type,
                 sf::Keyboard::Key keycode);

    // Monsters
    void _checkSpawnTimer();
    void _spawnMob();
    void _displayMonsters();

    // Projectiles
    bool _projHasHit(const Projectile &proj);
    void _checkProjectiles();
    void _displayProjectiles();
    bool _projIsOffscreen(const Projectile &proj);

    // Energy
    void _checkEnergy();
    void _displayEnergy();
    void _spawnEnergy();
    void _well();

    // Initialization elements
    TextureMap m_texture_map;
    SoundMap m_sound_map;
    Keymap m_playerOneKeymap;

    // Game main elements
    sf::RenderWindow m_window;
    sf::Font font;
    sf::Music m_music;
    Option m_option;
    std::unique_ptr<Hero> m_player;
    std::unique_ptr<Map> m_map;

    // Projectiles
    std::deque<std::unique_ptr<Projectile> > m_projectiles;

    // Monsters
    sf::Clock m_clock;
    sf::Time m_pauseTime;
    sf::Time m_nextMobSpawnTime;
    sf::Time m_nextEnergySpawnTime;
    std::deque<std::unique_ptr<Monster> > m_monsters;

    // Energy
    std::deque<std::unique_ptr<Energy> > m_energy;

    bool m_gameOver;

    // Score
    sf::Time m_survivedTime;
    sf::Time m_timeInPause;
    unsigned int m_score;

    // Window settings
    const unsigned int WIN_WIDTH = 1920;
    const unsigned int WIN_HEIGHT = 1080;

    // Loaded file paths
    const std::string PLAYER_TEXTURE_PATH = "./ressource/img/hero.png";
    const std::string MONSTER_TEXTURE_PATH = "./ressource/img/monster.png";
    const std::string PROJECTILE_TEXTURE_PATH = "./ressource/img/projectile.png";
    const std::string ENERGY_TEXTURE_PATH = "./ressource/img/powerorb.png";
    const std::string MENU_BACKGROUND = "./ressource/img/mainmenuscreen.png";
    const std::string MAP_BG_TEXTURE_PATH = "./ressource/img/game_bg.png";
    const std::string WALL_TEXTURE_PATH = "./ressource/img/wall.png";
    const std::string GAMEOVER_TEXTURE_PATH = "./ressource/img/gameoverscreen.png";
    const std::string PAUSE_TEXTURE_PATH = "./ressource/img/pausescreen.png";
    const std::string WELL_TEXTURE_PATH = "./ressource/img/well.png";

    const std::string MUSIC_SOUND = "./ressource/sound/maintheme.ogg";

    const std::string LASER_SHOT_SOUND = "./ressource/sound/laser_shot.ogg";
    const std::string GAME_OVER_SOUND = "./ressource/sound/gameover.ogg";

    std::unique_ptr<Menu> menu;
    std::unique_ptr<Pause> pause;
    std::unique_ptr<Gameover> over;
    Whoisrunning who;

};

#endif // GAME_HPP
