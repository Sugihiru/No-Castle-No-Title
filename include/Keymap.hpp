#ifndef KEYMAP_HPP
#define KEYMAP_HPP

#include <map>
#include <SFML/Graphics.hpp>

struct Key
{
    sf::Event::EventType eventType;
    sf::Keyboard::Key keyCode;
    sf::Mouse::Button mouseButton;
};

using Keymap = std::map<std::string, Key>;

#endif // KEYMAP_HPP
