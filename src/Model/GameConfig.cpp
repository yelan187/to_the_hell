#include "Model/GameConfig.h"

namespace Model {

// 实体尺寸配置
const sf::Vector2f GameConfig::PLATFORM_SIZE = sf::Vector2f(100, 12);
const sf::Vector2f GameConfig::PLAYER_SIZE = sf::Vector2f(30, 60);
const sf::Vector2f GameConfig::ENEMY_SIZE = sf::Vector2f(40, 40);
const sf::Vector2f GameConfig::BULLET_SIZE = sf::Vector2f(8, 8);
const sf::Vector2f GameConfig::PICKUP_SIZE = sf::Vector2f(20, 20);
const sf::Vector2f GameConfig::ARROW_SIZE = sf::Vector2f(30, 8);

// 游戏速度配置
const float GameConfig::DEFAULT_SCROLL_SPEED = 100.0f;
const float GameConfig::ARROW_SPEED = 300.0f;
const float GameConfig::SPRINT_DISTANCE = 150.0f;

// 生成间隔配置
const float GameConfig::PLATFORM_GENERATE_BASE_INTERVAL = 1.0f;
const float GameConfig::ENEMY_GENERATE_BASE_INTERVAL = 3.0f;
const float GameConfig::PICKUP_GENERATE_BASE_INTERVAL = 0.8f;

// 技能冷却时间
const float GameConfig::ARROW_SKILL_COOLDOWN = 3.0f;
const float GameConfig::SPRINT_SKILL_COOLDOWN = 5.0f;

} // namespace Model
