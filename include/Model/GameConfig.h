#pragma once
#include <SFML/Graphics.hpp>

namespace Model {

struct GameConfig {
    // 实体尺寸配置
    static const sf::Vector2f PLATFORM_SIZE;
    static const sf::Vector2f PLAYER_SIZE;
    static const sf::Vector2f ENEMY_SIZE;
    static const sf::Vector2f BULLET_SIZE;
    static const sf::Vector2f PICKUP_SIZE;
    static const sf::Vector2f ARROW_SIZE;
    
    // 游戏速度配置
    static const float DEFAULT_SCROLL_SPEED;
    static const float ARROW_SPEED;
    static const float SPRINT_DISTANCE;
    
    // 生成间隔配置
    static const float PLATFORM_GENERATE_BASE_INTERVAL;
    static const float ENEMY_GENERATE_BASE_INTERVAL;
    static const float PICKUP_GENERATE_BASE_INTERVAL;
    
    // 技能冷却时间
    static const float ARROW_SKILL_COOLDOWN;
    static const float SPRINT_SKILL_COOLDOWN;
};

} // namespace Model
