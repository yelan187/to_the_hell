#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Common/Config/Config.h"
#include "Common/PlatformType.h"

namespace Model{
namespace Entities {
using Common::PlatformType;
class Platform{
public:
    Platform(int id, PlatformType type, sf::Vector2f position, sf::Vector2f size);

    bool outOfWindow(sf::Vector2u window_size) const {
        return position.y + size.y < 0;
    }
    sf::Vector2f getPosition() const    {return position;}
    sf::Vector2f getSize() const        {return size;}
    sf::Vector2f getVelocity() const;
    void update(float delta_time);
    
    // 平台状态和特殊效果方法
    bool isDeadly() const { return type == PlatformType::SPIKED; }
    bool isBroken() const { return type == PlatformType::FRAGILE && is_broken; }
    float getRollingSpeed() const { return rolling_speed; }
    bool getRollingDirection() const { return rolling_direction_right; } // 获取滚动方向
    float getBounceForce() const { return bounce_force; }
    
    // 触发平台特殊效果
    void onPlayerLanded(); // 当玩家踩到平台时调用
    sf::Vector2f getRollingVelocity() const; // 获取滚动平台的额外速度

    int id;
    PlatformType type;
    sf::Vector2f size;
    sf::Vector2f position;
    
private:
    // 滚动平台相关
    float rolling_speed;
    bool rolling_direction_right; // true为向右，false为向左
    
    // 脆弱平台相关
    bool is_broken;
    float break_timer;

    // 弹跳平台相关
    float bounce_force;
};
}
}