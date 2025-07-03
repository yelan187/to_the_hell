#include "Model/Entities/Bullet.h"
#include "Common/Config/Config.h"

using Model::Entities::Bullet;

Bullet::Bullet(int id, sf::Vector2f position, sf::Vector2f velocity, sf::Vector2f size, bool is_player_bullet)
    : id(id), position(position), velocity(velocity), size(size), is_player_bullet(is_player_bullet) {
}

void Bullet::update(float delta_time) {
    position += velocity * delta_time;
    
    // 如果是玩家箭矢，抵消滚动影响，保持水平飞行
    if (is_player_bullet) {
        position.y -= Common::Config::GameConfig::SCROLL_SPEED * delta_time;  // 抵消向上滚动
    }
}

bool Bullet::outOfWindow(sf::Vector2u window_size) const {
    return position.x + size.x < 0 ||
           position.x > window_size.x ||
           position.y + size.y < 0 ||
           position.y > window_size.y;
}

bool Bullet::collidesWith(sf::Vector2f target_pos, sf::Vector2f target_size) const {
    return !(position.x + size.x < target_pos.x ||
             position.x > target_pos.x + target_size.x ||
             position.y + size.y < target_pos.y ||
             position.y > target_pos.y + target_size.y);
}
