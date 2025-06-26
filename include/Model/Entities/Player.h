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
    Player();
    void update(float deltaTime);
    void jump();
    void moveLeft();
    void moveRight();
    void stopLeft();
    void stopRight();

    PlayerState getState() const;
    sf::Vector2f getPosition() const;
    void setPosition(const sf::Vector2f& position);

private:
    PlayerState state;
    sf::Vector2f position;
    sf::Vector2f velocity;
};

}
}