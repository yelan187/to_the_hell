#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

namespace Model {
namespace Entities {

class Platform;

enum class PlayerState {
    IDLE,
    WALKING,
    JUMPING
};
class Player {
public:
    Player(sf::Vector2f position,sf::Vector2f size)
        : position(position),size(size),on_platform(-1),walking_speed(3.0f) 
    {}

    void updatePosition(float delta_time);
    void updateVelocity(float delta_time);

    void assocatedVelocity(sf::Vector2f v) {
        velocity += v;
    }
    
    void addAcceleration(sf::Vector2f a) {
        acceleration += a;
    }
    void update(float delta_time); // x += v * t + 1/2 * a * t * t, v += a * v, a = 0
    bool onPlatform(Platform* platform);
    void jump();
    void walkLeft() {
        velocity.x -= walking_speed;
    }
    void walkRight() {
        velocity.x += walking_speed;
    }
    void stopLeft();
    void stopRight();

    PlayerState getState() const {return state;};
    sf::Vector2f getPosition() const { return position; };
    sf::Vector2f getSize() const { return size; };
    void setPosition(const sf::Vector2f& position);

    int on_platform; // -1 if player is not standing on any platform, else platform id  

private:
    PlayerState state;

    sf::Vector2f size;
    sf::Vector2f position;

    sf::Vector2f velocity;
    float walking_speed;

    sf::Vector2f acceleration;
};

}
}