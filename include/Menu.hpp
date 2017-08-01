#ifndef MENU_HPP
# define MENU_HPP

#include "Button.hpp"
#include <vector>

enum difficulty
  {
    NORMAL = 1,
    DIFFICULT = 2,
    INSANE = 4,
    LUNATIC = 10,
  };

typedef struct	s_option
{
  s_option(): level(NORMAL), volume(50) {};
  difficulty	level;
  int           volume;
}		Option;

class Menu
{
public:
  Menu(int x, int y, const sf::Texture &texture);
  ~Menu();
  void	drawButtons(sf::RenderWindow &m_window, sf::Font &font);
  void  pollEvents(const sf::Event &event);
  void  pollhelpEvents(const sf::Event &event);
  void	pause_to_menu();
  void  help_to_menu();

private:
  template<class C>
  void	drawButt(sf::RenderWindow &m_window, unsigned int idx, sf::Font &font,
		 std::vector<std::unique_ptr<C>> &buttons);
  void  Option_buttons(int x, int y);

private:
  void  start(unsigned int);
  void  option(unsigned int);
  void  help(unsigned int);
  void  quit(unsigned int);
  void  difficulty(unsigned int);
  void  sound_on_off(unsigned int);
  void  sound_down(unsigned int);
  void  sound_up(unsigned int);
  void  back_to_menu(unsigned int);
  void  help_contents(sf::RenderWindow &m_window, sf::Font &font);
  void  drawhelpbutt(sf::RenderWindow &m_window, sf::Font &font);

private:
  std::vector<std::unique_ptr<Button>> m_butt;
  std::vector<std::unique_ptr<Button>> op_butt;
  std::unique_ptr<Button> help_quit;
  sf::Sprite m_menu;
};

#endif // MENU_HPP
