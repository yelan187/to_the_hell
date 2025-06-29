#pragma once
#include <SFML/Graphics.hpp>

namespace Model {
    class GameModel;
}

namespace Model {
namespace Entities {

enum class EnemyType {
    GHOST
};

class Enemy {
public:
    Enemy(int id, EnemyType type, sf::Vector2f position, sf::Vector2f size, GameModel* game_model);
    
    void update(float delta_time);
    bool outOfWindow(sf::Vector2u window_size) const;
    bool collidesWith(sf::Vector2f target_pos, sf::Vector2f target_size) const;
    
    sf::Vector2f getPosition() const { return position; }
    sf::Vector2f getSize() const { return size; }
    EnemyType getType() const { return type; }
    int getId() const { return id; }
    
    bool canShoot() const;
    void shoot();

private:
    int id;
    EnemyType type;
    sf::Vector2f position;
    sf::Vector2f size;
    sf::Vector2f velocity;
    GameModel* game_model;
    
    float shoot_timer;
    float shoot_interval;
    float move_speed;
    
    void updateMovement(float delta_time);
    void updateShooting(float delta_time);
};

}
}
