#include <functional>
#include "Game.hpp"

Pause::Pause(int width, int length, const sf::Texture &texture)
{
  int           tmp_y = length / 3;

  m_pause.setTexture(texture);
  m_pause.setPosition(sf::Vector2f(0, 0));

  buttons.push_back(std::unique_ptr<Button>(new Button(width / 2 - 200, tmp_y + 50, 400, 50,
                                                       START, false, "BACK TO GAME")));
  tmp_y += 75;
  buttons.push_back(std::unique_ptr<Button>(new Button(width / 2 - 200, tmp_y + 50, 400, 50,
                                                       BACK_TO_MENU, false, "BACK TO MAIN MENU")));
}

Pause::~Pause() {}

void            Pause::drawButt(sf::RenderWindow &m_window, unsigned int idx, sf::Font &font)
{
  sf::Text      text;
  sf::Vector2f  pos = (buttons[idx]->getBox())->getPosition();
  sf::Vector2f  size = (buttons[idx]->getBox())->getSize();

  pos.x += size.x / 4;
  pos.y += size.y / 4;
  text.setCharacterSize(24);
  text.setFont(font);
  text.setString((buttons[idx]->getWrite()).c_str());
  text.setPosition(pos);
  m_window.draw(*(buttons[idx]->getBox()));
  // text.setOutlineColor(sf::Color(128, 128, 128));
  // text.setFillColor(sf::Color(128, 128, 128));
  text.setColor(sf::Color(128, 128, 128, 128));
  m_window.draw(text);
  // write text + change inside text -> utilité de la map
}

void    Pause::drawButtons(sf::RenderWindow &m_window, sf::Font &font)
{
  m_window.draw(m_pause);
  // draw les box
  for (unsigned int idx = 0; idx < buttons.size(); idx++)
    {
      if (buttons[idx]->isDraw() == true)
        drawButt(m_window, idx, font);
    }
}

void    Pause::pollEvents(const sf::Event &event)
{
  static std::vector<std::function<void(Pause *)>> _butt_func =
    {
      { &Pause::back_to_game },
      { &Pause::back_to_menu },
    };

  for (unsigned int idx = 0; idx < buttons.size(); idx++)
    {
      if (buttons[idx]->isDraw() == true && buttons[idx]->isPressed(event) == true)
        _butt_func[idx](this);
    }
}

void    Pause::pause()
{
  Game  *game = Game::getInstance();

  game->setWho(PAUSE);
  for (unsigned int idx = 0; idx < buttons.size(); idx++)
    buttons[idx]->setDraw(true);
}

void    Pause::back_to_game()
{
  Game  *game = Game::getInstance();

  game->setWho(GAME);
  for (unsigned int idx = 0; idx < buttons.size(); idx++)
    {
      if (buttons[idx]->isDraw() == true)
        buttons[idx]->setDraw(false);
    }
  game->setNextMobSpawnTime();
  game->setTimeInPause();
  game->incrementMobsNextFireTime();
}

void    Pause::back_to_menu()
{
  Game  *game = Game::getInstance();

  game->setWho(MENU);
  game->dispMenu();
  game->resetGame();
  for (unsigned int idx = 0; idx < buttons.size(); idx++)
    {
      if (buttons[idx]->isDraw() == true)
        buttons[idx]->setDraw(false);
    }
}
