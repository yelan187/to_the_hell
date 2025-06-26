#pragma once
#include <SFML/Graphics.hpp>

namespace Model{
namespace Entities {

enum class PlatformType {
    NORMAL
};
class Platform{
public:
    Platform(int id, PlatformType type, sf::Vector2f position, sf::Vector2f size);

    int id;
    PlatformType type;
    sf::Vector2f size;
    sf::Vector2f position;
};
}
}