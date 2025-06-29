#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

namespace Model{
namespace Entities {

enum class PlatformType {
    NORMAL,     // 普通平台
    SPIKED,     // 带刺平台（踩上即死）
    ROLLING,    // 滚动平台（左右滚动，在平台上时人物会以一定速度水平位移）
    FRAGILE,    // 脆弱平台（踩到后几秒破碎）
    BOUNCY      // 弹跳平台（踩到后给人物施加向上速度）
};
class Platform{
public:
    Platform(int id, PlatformType type, sf::Vector2f position, sf::Vector2f size, float scroll_speed);

    bool outOfWindow(sf::Vector2u window_size) const {
        return position.y + size.y < 0;
    }
    sf::Vector2f getPosition() const    {return position;}
    sf::Vector2f getSize() const        {return size;}
    sf::Vector2f getVelocity() const    {return velocity;}
    void update(float delta_time);
    
    // 新增方法：获取平台状态和特殊效果
    bool isDeadly() const { return type == PlatformType::SPIKED; }
    bool isBroken() const { return type == PlatformType::FRAGILE && is_broken; }
    float getRollingSpeed() const { return rolling_speed; }
    bool getRollingDirection() const { return rolling_direction_right; } // 获取滚动方向
    float getBounceForce() const { return bounce_force; }
    
    // 新增方法：触发平台特殊效果
    void onPlayerLanded(); // 当玩家踩到平台时调用
    sf::Vector2f getRollingVelocity() const; // 获取滚动平台的额外速度

    int id;
    PlatformType type;
    sf::Vector2f size;
    sf::Vector2f position;
    sf::Vector2f velocity;
    
private:
    // 滚动平台相关
    float rolling_speed;
    bool rolling_direction_right; // true为向右，false为向左
    
    // 脆弱平台相关
    bool is_broken;
    float break_timer;
    static constexpr float BREAK_DELAY = 0.3f; // 0.3秒后破碎

    // 弹跳平台相关
    float bounce_force;
};
}
}