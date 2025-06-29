#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

namespace Model {
    class GameModel;
}

namespace Model {
namespace Entities {

class Platform;

enum class PlayerState {
    IDLE,
    WALKING,
    JUMPING_IDLE,
    JUMPING_WALKING
};

enum class CollisionDirection {
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT
};// which direction **of** player is colliding with the platform

enum class FacingDirection {
    LEFT,
    RIGHT
};

class Player {
public:
    Player(sf::Vector2f position,sf::Vector2f size,GameModel* game_model)
        : position(position),size(size),on_platform(false),on_platform_id(-1),state(PlayerState::IDLE),game_model(game_model) {
            walking_speed = 150.0f;
            jumping_speed = 350.0f;
            gravity = sf::Vector2f(0,500.0f);
            collision_direction = CollisionDirection::NONE;
            prev_collision_direction = CollisionDirection::NONE;
            facing_direction = FacingDirection::RIGHT; // 默认面向右
        }

    void updatePosition(float delta_time);
    void updateVelocity(float delta_time);
    void updateAcceleration(float delta_time);

    void setVelocity(sf::Vector2f v) {
        velocity = v;
    }
    
    void setAcceleration(sf::Vector2f a) {
        acceleration = a;
    }
    void update(float delta_time);
    
    sf::Vector2f findCollisionPosition(Platform* platform, sf::Vector2f prev_position, float delta_time, bool by_time = true);
    void handleCollision(Platform* platform, sf::Vector2f prev_position, float delta_time);
    bool collisionDetection(Platform* platform);
    bool collisionDetection(Platform* platform, sf::Vector2f position);
    void jump(float scroll_speed);
    void fall();
    void walkLeft();
    void walkRight();
    void stopLeft();
    void stopRight();

    PlayerState getState() const {return state;}
    sf::Vector2f getPosition() const { return position; }
    sf::Vector2f getSize() const { return size; }
    FacingDirection getFacingDirection() const { return facing_direction; }

    int getOnPlatformId() const {
        return on_platform_id;
    }

    void setPosition(float x, float y) {
        position.x = x;
        position.y = y;
    }
    void setPosition(const sf::Vector2f& position) {
        this->position = position;
    }

private:
    GameModel* game_model;

    bool on_platform;
    int on_platform_id;

    PlayerState state;

    sf::Vector2f size;
    sf::Vector2f position;
    sf::Vector2f acceleration;
    sf::Vector2f velocity;
    sf::Vector2f gravity;

    CollisionDirection collision_direction;
    CollisionDirection prev_collision_direction;
    
    FacingDirection facing_direction;

    float walking_speed;
    float jumping_speed;
};

}
}