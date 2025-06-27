#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

namespace Model {
namespace Entities {

enum class PlayerState {
    IDLE,
    WALKING,
    JUMPING
};
class Player {
public:
    Player(sf::Vector2f position,sf::Vector2f size);
    void update(float deltaTime);
    void jump();
    void moveLeft();
    void moveRight();
    void stopLeft();
    void stopRight();

    PlayerState getState() const;
    sf::Vector2f getPosition() const;
    sf::Vector2f getSize() const;
    void setPosition(const sf::Vector2f& position);

private:
    PlayerState state;
    sf::Vector2f size;
    sf::Vector2f position;

    sf::Vector2f velocity;
    
    
    sf::Vector2f acceleration;
};

}
}