#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

namespace Model {
namespace Entities {

class Player {
public:
    enum class State {
        IDLE,
        WALKING,
        JUMPING
    };

    Player();
    void update(float deltaTime);
    void jump();
    void moveLeft();
    void moveRight();
    void stopLeft();
    void stopRight();

    State getState() const;
    sf::Vector2f getPosition() const;
    void setPosition(const sf::Vector2f& position);

private:
    State state;
    sf::Vector2f position;
    sf::Vector2f velocity;
};

}
}