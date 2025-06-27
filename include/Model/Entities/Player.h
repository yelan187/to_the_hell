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
        : position(position),size(size),on_platform(false),on_platform_id(1),state(PlayerState::IDLE) {
            walking_speed = 100.0f;
            jumping_speed = 200.0f;
            gravity = sf::Vector2f(0,300.0f);
        }

    void updatePosition(float delta_time);
    void updateVelocity(float delta_time);

    void assocatedVelocity(sf::Vector2f v) {
        velocity += v;
    }
    
    void addAcceleration(sf::Vector2f a) {
        acceleration += a;
    }
    void update(float delta_time, std::map<int, Platform*>& platforms); // x += v * t + 1/2 * a * t * t, v += a * v, a = 0
    void adjustPosition(Platform* &platforms);
    void jump();
    void fall();
    void walkLeft();
    void walkRight();
    void stopLeft();
    void stopRight();

    PlayerState getState() const {return state;}
    sf::Vector2f getPosition() const { return position; }
    sf::Vector2f getSize() const { return size; }
    void setPosition(const sf::Vector2f& position) {
        this->position = position;
    }

private:
    bool on_platform;
    int on_platform_id;

    PlayerState state;

    sf::Vector2f size;
    sf::Vector2f position;
    sf::Vector2f acceleration;
    sf::Vector2f velocity;
    sf::Vector2f gravity;

    float walking_speed;
    float jumping_speed;
};

}
}