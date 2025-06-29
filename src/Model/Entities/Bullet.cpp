#include "Model/Entities/Bullet.h"

using Model::Entities::Bullet;

Bullet::Bullet(int id, sf::Vector2f position, sf::Vector2f velocity, sf::Vector2f size)
    : id(id), position(position), velocity(velocity), size(size) {
}

void Bullet::update(float delta_time) {
    position += velocity * delta_time;
}

bool Bullet::outOfWindow(sf::Vector2u window_size) const {
    return position.x + size.x < 0 || position.x > window_size.x ||
           position.y + size.y < 0 || position.y > window_size.y;
}

bool Bullet::collidesWith(sf::Vector2f target_pos, sf::Vector2f target_size) const {
    sf::Vector2f bullet_lt = position;
    sf::Vector2f bullet_rb = bullet_lt + size;
    
    sf::Vector2f target_lt = target_pos;
    sf::Vector2f target_rb = target_lt + target_size;
    
    return !(bullet_rb.x <= target_lt.x || bullet_lt.x >= target_rb.x ||
             bullet_rb.y <= target_lt.y || bullet_lt.y >= target_rb.y);
}
