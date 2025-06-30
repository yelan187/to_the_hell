#include "Model/Entities/Pickup.h"
#include "Model/GameModel.h"
#include "Model/Entities/Platform.h"

using Model::Entities::Pickup;

Pickup::Pickup(int id, PickupType type, sf::Vector2f position, sf::Vector2f size, GameModel* game_model, int platform_id)
    : id(id), type(type), position(position), size(size), game_model(game_model), platform_id(platform_id) {
    
    scroll_speed = 100.0f;
    velocity = sf::Vector2f(0, -scroll_speed);
    
    // 如果是普通豆子且指定了平台ID，定位到平台上方
    if (platform_id != -1 && type == PickupType::NORMAL_DOT) {
        auto platforms = game_model->getPlatforms();
        if (platforms.find(platform_id) != platforms.end()) {
            sf::Vector2f platform_pos = platforms[platform_id]->getPosition();
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
        // 普通豆子跟随平台移动
        auto platforms = game_model->getPlatforms();
        if (platforms.find(platform_id) != platforms.end()) {
            sf::Vector2f platform_pos = platforms[platform_id]->getPosition();
            position.y = platform_pos.y - size.y;
            velocity = sf::Vector2f(0, -scroll_speed);
        } else {
            // 平台已销毁，脱离平台
            velocity = sf::Vector2f(0, -scroll_speed);
            platform_id = -1;
        }
    } else {
        // 自由移动（五角星豆子或脱离平台的普通豆子）
        velocity = sf::Vector2f(0, -scroll_speed);
        
        // 普通豆子尝试附着到新平台
        if (type == PickupType::NORMAL_DOT) {
            auto platforms = game_model->getPlatforms();
            
            for (const auto& platform_pair : platforms) {
                sf::Vector2f platform_pos = platform_pair.second->getPosition();
                sf::Vector2f platform_size = platform_pair.second->getSize();
                
                bool x_overlap = (position.x + size.x > platform_pos.x && 
                                position.x < platform_pos.x + platform_size.x);
                bool y_contact = (position.y + size.y >= platform_pos.y - 5 && 
                                position.y <= platform_pos.y + 10);
                
                if (x_overlap && y_contact) {
                    platform_id = platform_pair.first;
                    position.y = platform_pos.y - size.y;
                    velocity = sf::Vector2f(0, -scroll_speed);
                    break;
                }
            }
        }
    }
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

bool Pickup::outOfWindow(sf::Vector2u window_size) const {
    return position.x + size.x < 0 ||
           position.x > window_size.x ||
           position.y + size.y < 0 ||
           position.y > window_size.y;
}

bool Pickup::collidesWith(sf::Vector2f target_pos, sf::Vector2f target_size) const {
    return !(position.x + size.x < target_pos.x ||
             position.x > target_pos.x + target_size.x ||
             position.y + size.y < target_pos.y ||
             position.y > target_pos.y + target_size.y);
}
