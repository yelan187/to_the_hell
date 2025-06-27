#pragma once
#include <SFML/Graphics.hpp>

namespace Model{
namespace Entities {

enum class PlatformType {
    NORMAL
};
class Platform{
public:
    Platform(int id, PlatformType type, sf::Vector2f position, sf::Vector2f size, float scroll_speed) {};
    sf::Vector2f getPosition() const { return position; };
    sf::Vector2f getSize() const { return size; };

    int id;
    PlatformType type;
    sf::Vector2f size;
    sf::Vector2f position;
    sf::Vector2f velocity;
};
}
}