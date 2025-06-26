#pragma once
#include <SFML/Graphics.hpp>

namespace Model{
class Platform{
public:
    enum class Type {
        NORMAL
    };
    Platform(int id, Type type, sf::Vector2f position, sf::Vector2f size);

    int id;
    Type type;
    sf::Vector2f size;
    sf::Vector2f position;
};
}