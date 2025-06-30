#include "Model/Entities/Enemy.h"
#include "Model/GameModel.h"
#include <cmath>

using Model::Entities::Enemy;

Enemy::Enemy(int id, EnemyType type, sf::Vector2f position, sf::Vector2f size, GameModel* game_model)
    : id(id), type(type), position(position), size(size), game_model(game_model) {
    
    shoot_timer = 0.0f;
    shoot_interval = 2.0f; // 每2秒射击一次
    move_speed = 50.0f;    // 移动速度50像素/秒
    velocity = sf::Vector2f(0, 0);
    facing_direction = sf::Vector2f(1.0f, 0.0f);  // 默认面向右侧
}

void Enemy::update(float delta_time) {
    updateMovement(delta_time);
    updateShooting(delta_time);
    position += velocity * delta_time;
}

void Enemy::updateMovement(float delta_time) {
    sf::Vector2f player_pos = game_model->getPlayerPosition();
    sf::Vector2f direction = player_pos - position;
    
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    
    if (length > 0) {
        direction.x /= length;
        direction.y /= length;
        velocity = direction * move_speed;
        
        // 更新面向方向：根据移动方向设置面向
        if (direction.x > 0) {
            facing_direction = sf::Vector2f(1.0f, 0.0f);  // 面向右侧
        } else if (direction.x < 0) {
            facing_direction = sf::Vector2f(-1.0f, 0.0f); // 面向左侧
        }
        // 如果x方向为0，保持原来的面向方向
    }
}

void Enemy::updateShooting(float delta_time) {
    shoot_timer += delta_time;
}

bool Enemy::canShoot() const {
    return shoot_timer >= shoot_interval;
}

void Enemy::shoot() {
    if (canShoot()) {
        sf::Vector2f player_pos = game_model->getPlayerPosition();
        sf::Vector2f direction = player_pos - position;
        
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length > 0) {
            direction.x /= length;
            direction.y /= length;
            
            sf::Vector2f bullet_velocity = direction * 200.0f;
            sf::Vector2f bullet_pos = position + size / 2.0f;
            
            game_model->createBullet(bullet_pos, bullet_velocity, false);  // 敌人子弹
        }
        
        shoot_timer = 0.0f;
    }
}

bool Enemy::outOfWindow(sf::Vector2u window_size) const {
    return position.x + size.x < 0 ||
           position.x > window_size.x ||
           position.y + size.y < 0 ||
           position.y > window_size.y;
}

bool Enemy::collidesWith(sf::Vector2f target_pos, sf::Vector2f target_size) const {
    return !(position.x + size.x < target_pos.x ||
             position.x > target_pos.x + target_size.x ||
             position.y + size.y < target_pos.y ||
             position.y > target_pos.y + target_size.y);
}
