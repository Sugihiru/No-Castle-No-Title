#include "Button.hpp"

// utiliser l'enum pour savoir quoi marquer ou ? ^^'
Button::Button(int x, int y, int width, int length, ButtonType type, bool draw, const std::string &w_write)
{
  sf::Vector2f		size(width, length);

  this->box = std::unique_ptr<sf::RectangleShape>(new sf::RectangleShape(size));
  box->setPosition(sf::Vector2f(x, y));
  box->setOutlineThickness(5);
  box->setOutlineColor(sf::Color(0, 0, 0));
  box->setFillColor(sf::Color(255, 255, 255));
  this->draw = draw;
  this->type = type;
  this->w_write = w_write;
}

Button::~Button() {}

void	Button::setDraw(const bool draw)
{ this->draw = draw; }

bool	Button::isDraw() const
{ return this->draw; }

ButtonType	Button::getType() const
{ return this->type; }

bool	Button::isPressed(const sf::Event &event) const
{
  sf::Vector2f pos = box->getPosition(), size = box->getSize();


  if (event.mouseButton.button == sf::Mouse::Left)
    {
      if (event.mouseButton.x >= pos.x && event.mouseButton.x <= pos.x + size.x &&
	  event.mouseButton.y >= pos.y && event.mouseButton.y <= pos.y + size.y)
	return true;
    }
  return false;
}

std::unique_ptr<sf::RectangleShape> &Button::getBox()
{ return this->box; }

void	Button::setWrite(const std::string &w_write)
{ this->w_write = w_write; }

std::string	Button::getWrite(void) const
{ return this->w_write; }
