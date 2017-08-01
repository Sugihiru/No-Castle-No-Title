#ifndef PAUSE_HPP
# define PAUSE_HPP

#include "Button.hpp"
#include <vector>

class Pause
{
public:
  Pause(int width, int length, const sf::Texture &texture);
  ~Pause();
  void drawButtons(sf::RenderWindow &m_window, sf::Font &font);
  void pollEvents(const sf::Event &event);
  void pause();
  void back_to_game();
  void back_to_menu();

private:
  void drawButt(sf::RenderWindow &m_window, unsigned int idx, sf::Font &font);
  
private:
  std::vector<std::unique_ptr<Button>> buttons;
  sf::Sprite m_pause;
};

#endif // PAUSE_HPP
