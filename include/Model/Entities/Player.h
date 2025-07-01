#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

#include "Common/Config/Config.h"

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
};

class Player {
public:
    Player(sf::Vector2f position, sf::Vector2f size, GameModel* game_model)
        : position(position), size(size), on_platform(false), on_platform_id(-1), 
          state(PlayerState::IDLE), game_model(game_model), is_dead(false) {
            walking_speed = Common::Config::GameConfig::PLAYER_WALK_SPEED;
            jumping_speed = Common::Config::GameConfig::PLAYER_JUMP_FORCE;
            gravity = sf::Vector2f(0, Common::Config::GameConfig::PLAYER_GRAVITY);
            collision_direction = CollisionDirection::NONE;
            prev_collision_direction = CollisionDirection::NONE;
            facing_direction = sf::Vector2f(1.0f, 0.0f);  // 默认面向右侧
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
    
    // 处理平台特殊效果
    void handlePlatformEffects(Platform* platform);
    void bounce(float bounce_force); // 弹跳效果
    void kill() { is_dead = true; }

    PlayerState getState() const { return state; }
    sf::Vector2f getPosition() const { return position; }
    sf::Vector2f getSize() const { return size; }
    
    // 添加面向方向支持
    sf::Vector2f getFacingDirection() const { return facing_direction; }
    void setFacingDirection(sf::Vector2f direction) { facing_direction = direction; }
    
    bool isDead() const { return is_dead; }
    void setDead(bool dead) { is_dead = dead; }

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
    bool is_dead;

    PlayerState state;

    sf::Vector2f size;
    sf::Vector2f position;
    sf::Vector2f acceleration;
    sf::Vector2f velocity;
    sf::Vector2f gravity;

    // 添加面向方向
    sf::Vector2f facing_direction;

    CollisionDirection collision_direction;
    CollisionDirection prev_collision_direction;

    float walking_speed;
    float jumping_speed;
};

}
}