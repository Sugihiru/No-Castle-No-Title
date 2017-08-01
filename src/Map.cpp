#include <cstdlib>
#include <iostream>
#include <string>
#include <string.h>

#include "Map.hpp"

Map::Map(float scale, unsigned int x, unsigned int y, sf::Texture &bg, sf::Texture &wall)
{
  _scale = scale;
  _x = x / _scale;
  _y = y / _scale;
  _bg.setTexture(bg);
  _wall.setTexture(wall);
  createHitboxes();
}

Map::~Map()
{}


void                Map::dispMap(sf::RenderWindow &window)
{
  _bg.setPosition(0, 0);
  window.draw(_bg);
  for (unsigned int i = 0; i < _hitboxes.size(); i++)
  {
    _wall.setPosition(_hitboxes[i].left, _hitboxes[i].top);
    window.draw(_wall);
  }
}

void                Map::createHitboxes()
{
  sf::IntRect       tmp;
  int               proba = _x * _y / 10;

  for (unsigned int j = 1; j < _y - 1; j++)
  {
    for (unsigned int i = 1; i < _x - 2; i++)
    {
      if (rand() % proba <= 1
          && ((i <= (_x / 2) - 3) || (i >= (_x /2) + 2))
          && ((j <= (_y / 2) - 3) || (j >= (_y /2) + 2)))
      {
        tmp = sf::IntRect((float)(i * _scale + 32), (float)(j * _scale + 32), _scale, _scale);
        _hitboxes.push_back(tmp);
      }
    }
  }
}


void Map::deleteHitboxes()
{
  _hitboxes.clear();
}

const std::deque<sf::IntRect>   Map::getHitbox() const
{ return (_hitboxes); }
