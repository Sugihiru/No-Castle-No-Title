#ifndef MAP_HPP
# define MAP_HPP

# include <string>
# include <deque>
# include <SFML/Graphics.hpp>


class Map
{
public:
    Map(float, unsigned int, unsigned int, sf::Texture &, sf::Texture &);
    ~Map();

    void                            dispMap(sf::RenderWindow &);
    void                            createHitboxes();
    void deleteHitboxes();
    const std::deque<sf::IntRect>   getHitbox() const;

private:
    unsigned int                     _x;
    unsigned int                     _y;
    float                   _scale;
    sf::Sprite              _bg;
    sf::Sprite              _wall;
    std::deque<sf::IntRect> _hitboxes;
};


#endif // MAP_HPP
