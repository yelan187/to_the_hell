#include "Model/Entities/Pickup.h"
#include "Model/GameModel.h"

using Model::Entities::Pickup;

Pickup::Pickup(int id, PickupType type, sf::Vector2f position, sf::Vector2f size, GameModel* game_model, int platform_id)
    : id(id), type(type), position(position), size(size), game_model(game_model), platform_id(platform_id) {
    
    scroll_speed = 100.0f; // 与平台同步的滚动速度
    velocity = sf::Vector2f(0, -scroll_speed); // 默认向上移动
    
    // 如果普通豆子指定了平台ID，则直接附着到该平台
    if (platform_id != -1 && type == PickupType::NORMAL_DOT) {
        auto platforms = game_model->getPlatforms();
        if (platforms.find(platform_id) != platforms.end()) {
            sf::Vector2f platform_pos = platforms[platform_id]->getPosition();
            // 确保豆子正好在平台上方
            this->position.y = platform_pos.y - size.y;
        }
    }
}

void Pickup::update(float delta_time) {
    updateMovement(delta_time);
    position += velocity * delta_time;
}

void Pickup::updateMovement(float delta_time) {
    if (isOnPlatform()) {
        // 在平台上的豆子跟随平台移动
        auto platforms = game_model->getPlatforms();
        if (platforms.find(platform_id) != platforms.end()) {
            sf::Vector2f platform_pos = platforms[platform_id]->getPosition();
            position.y = platform_pos.y - size.y;
            velocity = sf::Vector2f(0, -scroll_speed);
        } else {
            // 平台不存在了，豆子继续向上移动
            velocity = sf::Vector2f(0, -scroll_speed);
            platform_id = -1;
        }
    } else {
        // 不在平台上的豆子向上移动，同时检查是否能附着到平台
        velocity = sf::Vector2f(0, -scroll_speed);
        
        // 只有普通豆子才会附着到平台
        if (type == PickupType::NORMAL_DOT) {
            auto platforms = game_model->getPlatforms();
            
            for (const auto& platform_pair : platforms) {
                sf::Vector2f platform_pos = platform_pair.second->getPosition();
                sf::Vector2f platform_size = platform_pair.second->getSize();
                
                // 检查豆子是否接触到平台顶部
                if (position.x + size.x > platform_pos.x && 
                    position.x < platform_pos.x + platform_size.x &&
                    position.y + size.y >= platform_pos.y - 5 && 
                    position.y <= platform_pos.y + 10) {
                    
                    // 附着到平台
                    platform_id = platform_pair.first;
                    position.y = platform_pos.y - size.y;
                    velocity = sf::Vector2f(0, -scroll_speed);
                    break;
                }
            }
        }
    }
}

bool Pickup::outOfWindow(sf::Vector2u window_size) const {
    // 豆子从底部生成向上移动，只检查左右和顶部边界
    return position.x + size.x < 0 || position.x > window_size.x ||
           position.y + size.y < 0;
}

bool Pickup::collidesWith(sf::Vector2f target_pos, sf::Vector2f target_size) const {
    sf::Vector2f pickup_lt = position;
    sf::Vector2f pickup_rb = pickup_lt + size;
    
    sf::Vector2f target_lt = target_pos;
    sf::Vector2f target_rb = target_lt + target_size;
    
    return !(pickup_rb.x <= target_lt.x || pickup_lt.x >= target_rb.x ||
             pickup_rb.y <= target_lt.y || pickup_lt.y >= target_rb.y);
}

int Pickup::getScore() const {
    switch (type) {
        case PickupType::NORMAL_DOT:
            return 2;
        case PickupType::STAR_DOT:
            return 5;
        default:
            return 0;
    }
}
