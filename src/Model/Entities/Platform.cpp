#include "Model/Entities/Platform.h"
#include <cstdlib>

using Model::Entities::Platform;

Platform::Platform(int id, PlatformType type, sf::Vector2f position, sf::Vector2f size, float scroll_speed) {
    this->id = id;
    this->size = size;
    this->type = type;
    this->position = position;
    this->velocity = sf::Vector2f(0, -scroll_speed);
    
    // 初始化所有成员变量
    rolling_speed = 0.0f;
    rolling_direction_right = true;
    is_broken = false;
    break_timer = 0.0f;
    bounce_force = 0.0f;
    
    // 根据平台类型初始化特有属性
    switch (type) {
        case PlatformType::ROLLING:
            rolling_speed = 80.0f + static_cast<float>(rand() % 40); // 80-120的随机速度
            rolling_direction_right = (rand() % 2 == 0); // 随机方向
            break;
            
        case PlatformType::FRAGILE:
            is_broken = false;
            break_timer = 0.0f;
            break;
            
        case PlatformType::BOUNCY:
            bounce_force = 500.0f; // 增强弹跳力，确保明显的弹跳效果
            break;
            
        default:
            break;
    }
}

void Platform::update(float delta_time) {
    // 基础位置更新
    position = position + delta_time * velocity;
    
    // 根据平台类型更新特殊行为
    switch (type) {
        case PlatformType::FRAGILE:
            if (break_timer > 0) {
                break_timer -= delta_time;
                if (break_timer <= 0) {
                    is_broken = true;
                }
            }
            break;
            
        default:
            break;
    }
}

void Platform::onPlayerLanded() {
    switch (type) {
        case PlatformType::FRAGILE:
            if (!is_broken && break_timer <= 0) {
                break_timer = BREAK_DELAY; // 开始倒计时
            }
            break;
            
        default:
            break;
    }
}

sf::Vector2f Platform::getRollingVelocity() const {
    if (type == PlatformType::ROLLING) {
        float horizontal_speed = rolling_direction_right ? rolling_speed : -rolling_speed;
        return sf::Vector2f(horizontal_speed, velocity.y);
    }
    return velocity;
}