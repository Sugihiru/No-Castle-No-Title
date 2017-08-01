#ifndef BUTTON_HPP
# define BUTTON_HPP

#include <SFML/Graphics.hpp>

enum ButtonType
  {
    START,
    OPTION,
    MHELP,
    QUIT,
    DIFFICULTY,
    SOUND,
    SOUND_UP,
    SOUND_DOWN,
    FULL_SCREEN,
    BACK_TO_MENU,
  };

class Button
{

public:
  Button(int x, int y, int width, int length, ButtonType type, bool draw, const std::string &w_write);
  ~Button();

  bool		isPressed(const sf::Event &event) const;
  void		setDraw(const bool draw);
  bool		isDraw() const;
  ButtonType	getType() const;
  std::unique_ptr<sf::RectangleShape> &getBox();
  void		setWrite(const std::string &w_write);
  std::string	getWrite(void) const;

private:
  std::unique_ptr<sf::RectangleShape>	box;
  ButtonType				type;
  bool					draw;
  std::string				w_write;
};

#endif // BUTTON_HPP
