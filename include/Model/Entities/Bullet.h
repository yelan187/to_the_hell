#pragma once
#include <SFML/Graphics.hpp>

namespace Model {
namespace Entities {

class Bullet {
public:
    Bullet(int id, sf::Vector2f position, sf::Vector2f velocity, sf::Vector2f size);
    
    void update(float delta_time);
    bool outOfWindow(sf::Vector2u window_size) const;
    bool collidesWith(sf::Vector2f target_pos, sf::Vector2f target_size) const;
    
    sf::Vector2f getPosition() const { return position; }
    sf::Vector2f getSize() const { return size; }
    int getId() const { return id; }

private:
    int id;
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Vector2f size;
};

}
}
