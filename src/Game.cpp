#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>

#include "exceptions.hpp"
#include "Game.hpp"

Game *Game::instance = nullptr;

Game::Game(): m_nextMobSpawnTime(sf::seconds(1)),
	      m_nextEnergySpawnTime(sf::seconds(10)),
              m_gameOver(false), m_survivedTime(sf::Time::Zero),
              m_timeInPause(sf::Time::Zero)
{
    srand(static_cast<unsigned int>(time(0)));

    m_window.create(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "LudumDare39",
    sf::Style::Fullscreen);
    _initFont();
    _initTextures();
    _initSounds();
    _initPlayerOneKeymap();

    m_window.setFramerateLimit(60);

    m_player = std::unique_ptr<Hero>(new Hero(m_texture_map["Hero"],
                                              m_playerOneKeymap,
                                              WIN_WIDTH, WIN_HEIGHT));
    m_map = std::unique_ptr<Map>(new Map(64, WIN_WIDTH, WIN_HEIGHT,
                                         m_texture_map["MapBg"],
                                         m_texture_map["Wall"]));

    this->menu = std::unique_ptr<Menu>(new Menu(WIN_WIDTH, WIN_HEIGHT, m_texture_map["Menu"]));
    this->pause = std::unique_ptr<Pause>(new Pause(WIN_WIDTH, WIN_HEIGHT,
                                                   m_texture_map["Pause"]));
    this->over = std::unique_ptr<Gameover>(new Gameover(WIN_WIDTH, WIN_HEIGHT, m_texture_map["GameOver"]));
    this->who = MENU;

    m_score = 0;
}

void Game::_initFont()
{
    if (!font.loadFromFile("./ressource/font/DejaVuSans.ttf"))
      throw LoadFromFileException(PLAYER_TEXTURE_PATH);
}

// Load all textures at the initialization to avoid loading during the game
// Todo: For loop this function
void Game::_initTextures()
{
    m_texture_map.insert(TexturePair("Hero", sf::Texture()));
    if (!m_texture_map["Hero"].loadFromFile(PLAYER_TEXTURE_PATH))
        throw LoadFromFileException(PLAYER_TEXTURE_PATH);

    m_texture_map.insert(TexturePair("Monster", sf::Texture()));
    if (!m_texture_map["Monster"].loadFromFile(MONSTER_TEXTURE_PATH))
        throw LoadFromFileException(MONSTER_TEXTURE_PATH);

    m_texture_map.insert(TexturePair("Projectile", sf::Texture()));
    if (!m_texture_map["Projectile"].loadFromFile(PROJECTILE_TEXTURE_PATH))
        throw LoadFromFileException(PROJECTILE_TEXTURE_PATH);

    m_texture_map.insert(TexturePair("Energy", sf::Texture()));
    if (!m_texture_map["Energy"].loadFromFile(ENERGY_TEXTURE_PATH))
        throw LoadFromFileException(ENERGY_TEXTURE_PATH);

    m_texture_map.insert(TexturePair("Menu", sf::Texture()));
    if (!m_texture_map["Menu"].loadFromFile(MENU_BACKGROUND))
        throw LoadFromFileException(MENU_BACKGROUND);

    m_texture_map.insert(TexturePair("MapBg", sf::Texture()));
    if (!m_texture_map["MapBg"].loadFromFile(MAP_BG_TEXTURE_PATH))
        throw LoadFromFileException(MAP_BG_TEXTURE_PATH);

    m_texture_map.insert(TexturePair("Wall", sf::Texture()));
    if (!m_texture_map["Wall"].loadFromFile(WALL_TEXTURE_PATH))
        throw LoadFromFileException(WALL_TEXTURE_PATH);

    m_texture_map.insert(TexturePair("GameOver", sf::Texture()));
    if (!m_texture_map["GameOver"].loadFromFile(GAMEOVER_TEXTURE_PATH))
        throw LoadFromFileException(GAMEOVER_TEXTURE_PATH);

    m_texture_map.insert(TexturePair("Pause", sf::Texture()));
    if (!m_texture_map["Pause"].loadFromFile(PAUSE_TEXTURE_PATH))
        throw LoadFromFileException(PAUSE_TEXTURE_PATH);

    m_texture_map.insert(TexturePair("Well", sf::Texture()));
    if (!m_texture_map["Well"].loadFromFile(WELL_TEXTURE_PATH))
        throw LoadFromFileException(WELL_TEXTURE_PATH);

}


void Game::_initSounds()
{
    m_sound_map.insert(SoundPair("LaserShot",
                                 std::make_pair(sf::SoundBuffer(), sf::Sound())));
    if (!m_sound_map["LaserShot"].first.loadFromFile(LASER_SHOT_SOUND))
        throw LoadFromFileException(LASER_SHOT_SOUND);
    m_sound_map["LaserShot"].second.setBuffer(m_sound_map["LaserShot"].first);

    m_sound_map.insert(SoundPair("GameOver",
                                 std::make_pair(sf::SoundBuffer(), sf::Sound())));
    if (!m_sound_map["GameOver"].first.loadFromFile(GAME_OVER_SOUND))
        throw LoadFromFileException(GAME_OVER_SOUND);
    m_sound_map["GameOver"].second.setBuffer(m_sound_map["GameOver"].first);

    if (!m_music.openFromFile(MUSIC_SOUND))
        throw LoadFromFileException(MUSIC_SOUND);
    m_music.setLoop(true);
    m_music.play();
}


void Game::_initPlayerOneKeymap()
{
    const std::map<std::string, sf::Keyboard::Key> tab =
    {
        {"MoveUp", sf::Keyboard::Up},
        {"MoveDown", sf::Keyboard::Down},
        {"MoveLeft", sf::Keyboard::Left},
        {"MoveRight", sf::Keyboard::Right},
        {"FireUp", sf::Keyboard::W},
        {"FireDown", sf::Keyboard::S},
        {"FireLeft", sf::Keyboard::A},
        {"FireRight", sf::Keyboard::D}
    };

    for (auto &elem : tab)
        m_playerOneKeymap[elem.first] = \
            _initKey(sf::Event::KeyPressed, elem.second);
}

Key Game::_initKey(sf::Event::EventType event_type,
                   sf::Keyboard::Key keycode)
{
    Key key;

    key.eventType = event_type;
    key.keyCode = keycode;
    return key;
}

Game::~Game() {}


Game *Game::getInstance()
{
    if (!instance)
        instance = new Game;
    return instance;
}


bool Game::isRunning() const
{ return m_window.isOpen(); }


void Game::display()
{
    m_window.clear(sf::Color::White);

    m_music.setVolume(m_option.volume);
    if (who == MENU || who == HELP)
        menu->drawButtons(m_window, font);
    else if (who == PAUSE)
        pause->drawButtons(m_window, font);
    else
    {
        if (m_gameOver == true)
            over->drawButtons(m_window, font, m_score);
        else
        {
            m_map->dispMap(m_window);
	    _well();
            _displayMonsters();
            m_player->draw(m_window);
            _displayProjectiles();
            _displayEnergy();
        }
    }
    m_window.display();
}


void Game::_displayMonsters()
{
    sf::Vector2f player_pos = m_player->getPosition();

    for (auto &mob : m_monsters)
       mob->draw(m_window, player_pos);
}


void Game::incrementMobsNextFireTime()
{
    for (auto &mob : m_monsters)
        mob->incrementNextFireTime(m_clock.getElapsedTime() - m_pauseTime);
}


void Game::_displayProjectiles()
{
    for (auto &proj : m_projectiles)
        proj->draw(m_window);
}


void Game::_displayEnergy()
{
    for (auto &energy : m_energy)
        energy->draw(m_window);
}


void Game::pollEvents()
{
    sf::Event event;

    while (m_window.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::Closed:
            {
                m_window.close();
                break;
            }

            case sf::Event::KeyPressed:
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    if (who == MENU)
                        m_window.close();
                    else if (who == HELP)
                        menu->help_to_menu();
                    else if (who == GAME && !m_gameOver)
                    {
                        pause->pause();
                        m_pauseTime = m_clock.getElapsedTime();
                    }
                    else
                        pause->back_to_game();
                    break;
                }
            }

            case sf::Event::MouseButtonPressed:
            {
                if (who == MENU)
                  menu->pollEvents(event);
                else if (who == PAUSE)
                  pause->pollEvents(event);
                else if (who == HELP)
                  menu->pollhelpEvents(event);
                else if (who == GAME)
                  over->pollEvents(event);
            }
            default:
                break;
        }
    if (!m_gameOver)
        m_player->checkInput();
    }
    if (isInGame())
    {
        _checkSpawnTimer();
        _checkProjectiles();
        _checkEnergy();
        m_gameOver = isGameOver();
        // End of the game
        if (m_gameOver)
        {
            m_sound_map["GameOver"].second.play();
            m_survivedTime = m_clock.getElapsedTime() - m_timeInPause;
            // 3000 == Random score multiplier
            // Todo: Calculate a real multiplier
            if (m_survivedTime.asSeconds() > 0)
                m_score = log(m_survivedTime.asSeconds()) * 3000;
            m_score *= (1 + (static_cast<int>(getLevel() - 1) / 10.0));
        }
    }
}


void Game::spawnProjectile(const sf::Vector2f &pos,
                           Direction direction,
                           FiredBy fired_by,
                           int dmg)
{
    m_projectiles.emplace_back(std::unique_ptr<Projectile>(new Projectile(m_texture_map["Projectile"],
                                                                          pos,
                                                                          direction,
                                                                          fired_by,
                                                                          dmg)));
    if (fired_by == FiredBy::PLAYER)
        m_sound_map["LaserShot"].second.play();
}


void Game::_checkSpawnTimer()
{
    if (m_clock.getElapsedTime() > m_nextMobSpawnTime)
    {
        _spawnMob();
        m_nextMobSpawnTime = m_clock.getElapsedTime() + \
                             sf::milliseconds(rand() % 1000 + 2000);
    }
    if (m_clock.getElapsedTime() > m_nextEnergySpawnTime)
    {
        _spawnEnergy();
        m_nextEnergySpawnTime = m_clock.getElapsedTime() + \
                             sf::seconds(rand() % 5 + 10);
    }
}


void Game::_checkProjectiles()
{
    bool deleted;

    do
    {
        deleted = false;
        for (auto proj = m_projectiles.begin();
             proj != m_projectiles.end(); ++proj)
        {
            if (_projHasHit(**proj) || _projIsOffscreen(**proj))
            {
                m_projectiles.erase(proj);
                deleted = true;
                break;
            }
        }
    } while (deleted);
}


void Game::_checkEnergy()
{
    bool deleted;

    do
    {
        deleted = false;
        for (auto energy = m_energy.begin();
             energy != m_energy.end(); ++energy)
        {
            if ((*energy)->hasExpired() || m_player->gotEnergy(**energy))
            {
                m_energy.erase(energy);
                deleted = true;
                break;
            }
        }
    } while (deleted);
}


bool Game::isGameOver() const
{ return m_player->isDead(); }


bool Game::isInGame() const
{ return !m_gameOver && who == GAME; }


bool Game::collideWithWall(const sf::IntRect &hitbox) const
{
    for (auto &wall : m_map->getHitbox())
        if (hitbox.intersects(wall))
            return true;
    return false;
}


const sf::Vector2u Game::getWinSize() const
{
    return sf::Vector2u(WIN_WIDTH, WIN_HEIGHT);
}


sf::Vector2f Game::getHeroPosition() const
{ return m_player->getPosition(); }


sf::Time Game::getClockElapsedTime() const
{ return m_clock.getElapsedTime(); }


void Game::setNextMobSpawnTime()
{ m_nextMobSpawnTime += m_clock.getElapsedTime() - m_pauseTime; }

void Game::setNextEnergyTime()
{ m_nextEnergySpawnTime += m_clock.getElapsedTime() - m_pauseTime; }

void Game::setTimeInPause()
{ m_timeInPause += m_clock.getElapsedTime() - m_pauseTime; }

bool Game::_projHasHit(const Projectile &proj)
{
    sf::IntRect proj_hitbox = proj.getHitbox();

    if (collideWithWall(proj_hitbox))
        return true;
    if (proj.isPlayerFire())
    {
        for (auto mob = m_monsters.begin();
             mob != m_monsters.end(); ++mob)
        {
            if ((*mob)->getHitbox().intersects(proj_hitbox))
            {
                (*mob)->takeDmg(proj.getDmg());
                if ((*mob)->isDead())
                {
                    m_energy.emplace_back(std::unique_ptr<Energy>(new Energy(m_texture_map["Energy"],
                                                                             (*mob)->getPosition())));
                    m_monsters.erase(mob);
                    // Random value
                    // Todo: find a better value
                    m_score += 1000;
                }
                return true;
            }
        }
    }
    else
    {
        if (m_player->getHitbox().intersects(proj_hitbox))
        {
            m_player->takeDmg(proj.getDmg());
            return true;
        }
    }
    return false;
}


bool Game::_projIsOffscreen(const Projectile &proj)
{
    sf::IntRect proj_hitbox = proj.getHitbox();

    return (proj_hitbox.left >= static_cast<int>(WIN_WIDTH) ||
            proj_hitbox.top >= static_cast<int>(WIN_HEIGHT) ||
            proj_hitbox.left + proj_hitbox.width <= 0 ||
            proj_hitbox.top + proj_hitbox.height <= 0);
}


void Game::_spawnMob()
{
    int    side;
    float value;
    float x;
    float y;
    const sf::Vector2u mob_size = m_texture_map["Monster"].getSize();

    side = rand() % 4;
    if (side == 0)
    {
      //spawn mob on the top side
      value = rand() % WIN_WIDTH;
      x = value;
      y = 0;
    }
    else if (side == 1)
    {
      //spawn mob on the bottom side
      value = rand() % WIN_WIDTH;
      x = value;
      y = WIN_HEIGHT - mob_size.y;
    }
    else if (side == 2)
    {
      //spawn mob on the left side
      value = rand() % WIN_HEIGHT;
      x = 0;
      y = value;
    }
    else
    {
      //spawn mob on the right side
      value = rand() % WIN_HEIGHT;
      x = WIN_WIDTH - mob_size.x;
      y = value;
    }
    m_monsters.emplace_back(std::unique_ptr<Monster>(new Monster(m_texture_map["Monster"],
                                                                 x, y)));
}

void Game::_spawnEnergy()
{
  m_energy.emplace_back(std::unique_ptr<Energy>(new Energy(m_texture_map["Energy"],
							   sf::Vector2f(WIN_WIDTH / 2, WIN_HEIGHT / 2))));
}

void    Game::setWho(Whoisrunning who)
{ this->who = who; }

Whoisrunning Game::getWho(void) const
{ return this->who; }

void    Game::setLevel(const difficulty level)
{
  this->m_option.level = level;
  if (level == LUNATIC)
    m_player->setSpeed(150);
  else
    m_player->setSpeed(50);
}

void    Game::setSound(const bool sound)
{
  if (sound == false)
    this->m_music.pause();
  else
    this->m_music.play();
}

void    Game::setVolume(const int volume)
{
  if (volume >= 0 && volume <= 100)
    {
      this->m_option.volume = volume;
      this->m_music.setVolume(this->m_option.volume);
    }
}

int     Game::getVolume(void)
{ return this->m_option.volume; }

void    Game::closewin()
{ m_window.close(); }

void    Game::dispMenu()
{ menu->pause_to_menu(); }

void Game::resetGame()
{
    m_monsters.clear();
    m_projectiles.clear();
    m_energy.clear();
    m_player->reset(WIN_WIDTH, WIN_HEIGHT);
    m_map->deleteHitboxes();
    m_map->createHitboxes();
    m_gameOver = false;
    m_score = 0;
    m_survivedTime = sf::Time::Zero;
    m_timeInPause = sf::Time::Zero;
    m_pauseTime = sf::Time::Zero;
    m_nextMobSpawnTime = sf::seconds(1);
    m_clock.restart();
}


difficulty Game::getLevel() const
{
    return m_option.level;
}

void		Game::_well()
{
  sf::Sprite	m_well;

  m_well.setTexture(m_texture_map["Well"]);
  m_well.setPosition(sf::Vector2f(WIN_WIDTH / 2, WIN_HEIGHT / 2));
  m_window.draw(m_well);
}
