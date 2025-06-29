#include "Model/Entities/Enemy.h"
#include "Model/GameModel.h"
#include <cmath>

using Model::Entities::Enemy;

Enemy::Enemy(int id, EnemyType type, sf::Vector2f position, sf::Vector2f size, GameModel* game_model)
    : id(id), type(type), position(position), size(size), game_model(game_model) {
    
    shoot_timer = 0.0f;
    shoot_interval = 2.0f; // 每2秒射击一次
    move_speed = 50.0f;
    velocity = sf::Vector2f(0, 0);
}

void Enemy::update(float delta_time) {
    updateMovement(delta_time);
    updateShooting(delta_time);
    position += velocity * delta_time;
}

void Enemy::updateMovement(float delta_time) {
    // 获取玩家位置
    sf::Vector2f player_pos = game_model->getPlayerPosition();
    sf::Vector2f direction = player_pos - position;
    
    // 计算方向向量的长度
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    
    if (length > 0) {
        // 标准化方向向量
        direction.x /= length;
        direction.y /= length;
        
        // 设置速度朝向玩家
        velocity = direction * move_speed;
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
            
            sf::Vector2f bullet_velocity = direction * 200.0f; // 子弹速度
            sf::Vector2f bullet_pos = position + size / 2.0f; // 从敌人中心发射
            
            game_model->createBullet(bullet_pos, bullet_velocity);
        }
        
        shoot_timer = 0.0f;
    }
}

bool Enemy::outOfWindow(sf::Vector2u window_size) const {
    return position.x + size.x < 0 || position.x > window_size.x ||
           position.y + size.y < 0 || position.y > window_size.y;
}
