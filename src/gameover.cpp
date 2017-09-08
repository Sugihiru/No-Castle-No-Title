#include <functional>
#include "Game.hpp"

Gameover::Gameover(int width, int length, const sf::Texture &texture)
{
  int           tmp_y = length / 3;

  m_gameover.setTexture(texture);
  m_gameover.setPosition(sf::Vector2f(0, 0));

  buttons.push_back(std::unique_ptr<Button>(new Button(width / 2 - 200, tmp_y + 50, 400, 50,
                                                       START, false, "RESTART GAME")));
  tmp_y += 75;
  buttons.push_back(std::unique_ptr<Button>(new Button(width / 2 - 200, tmp_y + 50, 400, 50,
                                                       BACK_TO_MENU, false, "BACK TO MAIN MENU")));
}

Gameover::~Gameover() {}

void            Gameover::drawButt(sf::RenderWindow &m_window, unsigned int idx, sf::Font &font)
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
  text.setOutlineColor(sf::Color(128, 128, 128));
  text.setFillColor(sf::Color(128, 128, 128));
  // text.setColor(sf::Color(128, 128, 128, 128));
  m_window.draw(text);
  // write text + change inside text -> utilité de la map
}

void    Gameover::drawButtons(sf::RenderWindow &m_window, sf::Font &font,
                              unsigned int score)
{
  m_window.draw(m_gameover);

  // draw les boxs
  for (unsigned int idx = 0; idx < buttons.size(); idx++)
    buttons[idx]->setDraw(true);
  for (unsigned int idx = 0; idx < buttons.size(); idx++)
    {
      if (buttons[idx]->isDraw() == true)
        drawButt(m_window, idx, font);
    }
  _printScore(m_window, font, score);
}


void Gameover::_printScore(sf::RenderWindow &win, sf::Font &font,
                           unsigned int score)
{
  sf::Text      text;
  sf::Vector2u win_size = win.getSize();

  text.setCharacterSize(24);
  text.setFont(font);
  std::string score_txt = "Score: " + std::to_string(score);
  text.setString(score_txt);

  // Center text on x
  sf::FloatRect textRect = text.getLocalBounds();
  text.setOrigin(textRect.left + textRect.width / 2.0f, 0);
  text.setPosition(sf::Vector2f(win_size.x / 2.0, 350));
  text.setOutlineColor(sf::Color(255, 255, 255));
  text.setFillColor(sf::Color(255, 255, 255));
  // text.setColor(sf::Color(255, 255, 255, 255));
  win.draw(text);
}


void    Gameover::pollEvents(const sf::Event &event)
{
  static std::vector<std::function<void(Gameover *)>> _butt_func =
    {
      { &Gameover::new_game },
      { &Gameover::back_to_menu },
    };

  for (unsigned int idx = 0; idx < buttons.size(); idx++)
    {
      if (buttons[idx]->isDraw() == true && buttons[idx]->isPressed(event) == true)
        _butt_func[idx](this);
    }
}

void    Gameover::new_game()
{
  Game  *game = Game::getInstance();

  game->setWho(GAME);
  game->resetGame();
  for (unsigned int idx = 0; idx < buttons.size(); idx++)
    buttons[idx]->setDraw(false);
}

void    Gameover::back_to_menu()
{
  Game  *game = Game::getInstance();

  game->setWho(MENU);
  game->dispMenu();
  game->resetGame();
  for (unsigned int idx = 0; idx < buttons.size(); idx++)
    buttons[idx]->setDraw(false);
}
