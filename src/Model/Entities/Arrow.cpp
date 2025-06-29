#include "Model/Entities/Arrow.h"
#include "Model/GameModel.h"

using Model::Entities::Arrow;

Arrow::Arrow(int id, sf::Vector2f position, sf::Vector2f velocity, sf::Vector2f size, GameModel* game_model)
    : id(id), position(position), velocity(velocity), size(size), game_model(game_model), lifetime(0.0f) {
    // 确保箭矢只有水平速度
    this->velocity.y = 0.0f;
}

void Arrow::update(float delta_time) {
    // 箭矢水平移动
    position.x += velocity.x * delta_time;
    
    // 补偿游戏世界的向上滚动，使箭矢保持水平
    position.y -= game_model->getScrollSpeed() * delta_time;
    
    lifetime += delta_time;
}

bool Arrow::outOfWindow(sf::Vector2u window_size) const {
    return position.x + size.x < 0 || position.x > window_size.x ||
           position.y + size.y < 0 || position.y > window_size.y;
}

bool Arrow::isExpired() const {
    return lifetime >= MAX_LIFETIME;
}

bool Arrow::collidesWith(sf::Vector2f target_pos, sf::Vector2f target_size) const {
    sf::Vector2f arrow_lt = position;
    sf::Vector2f arrow_rb = arrow_lt + size;
    
    sf::Vector2f target_lt = target_pos;
    sf::Vector2f target_rb = target_lt + target_size;
    
    return !(arrow_rb.x <= target_lt.x || arrow_lt.x >= target_rb.x ||
             arrow_rb.y <= target_lt.y || arrow_lt.y >= target_rb.y);
}
