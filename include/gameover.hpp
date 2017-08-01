#ifndef GAMEOVER_HPP
# define GAMEOVER_HPP

#include "Button.hpp"
#include <vector>

class Gameover
{
public:
  Gameover(int width, int length, const sf::Texture &texture);
  ~Gameover();
  void drawButtons(sf::RenderWindow &m_window, sf::Font &font,
                   unsigned int score);
  void pollEvents(const sf::Event &event);
  void pause();
  void new_game();
  void back_to_menu();

private:
  void drawButt(sf::RenderWindow &m_window, unsigned int idx, sf::Font &font);
  void _printScore(sf::RenderWindow &win, sf::Font &font, unsigned int score);

private:
  std::vector<std::unique_ptr<Button>> buttons;
  sf::Sprite m_gameover;
};

#endif // GAMEOVER_HPP
