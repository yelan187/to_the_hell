#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

namespace Model{
namespace Entities {

enum class PlatformType {
    NORMAL
};
class Platform{
public:
    Platform(int id, PlatformType type, sf::Vector2f position, sf::Vector2f size, float scroll_speed);

    bool outOfWindow(sf::Vector2u window_size) const {
        return position.y + size.y < 0;
    }
    sf::Vector2f getPosition() const    {return position;}
    sf::Vector2f getSize() const        {return size;}
    sf::Vector2f getVelocity() const    {return velocity;}
    void update(float delta_time)       {
        position = position + delta_time * velocity;
    }

    int id;
    PlatformType type;
    sf::Vector2f size;
    sf::Vector2f position;
    sf::Vector2f velocity;
};
}
}