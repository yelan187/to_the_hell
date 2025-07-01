#include "Common/Config/GameConfig.h"
#include <fstream>
#include <iostream>

namespace Common {
namespace Config {

// ====================================
// 游戏配置实现
// ====================================

// 玩家配置
sf::Vector2f GameConfig::PLAYER_SIZE(60.0f, 60.0f);
float GameConfig::PLAYER_WALK_SPEED = 150.0f;
float GameConfig::PLAYER_JUMP_FORCE = 350.0f;
float GameConfig::PLAYER_GRAVITY = 500.0f;
float GameConfig::PLAYER_MAX_FALL_SPEED = 600.0f;   // 暂未使用，可以用于避免速度过快导致与平台等的碰撞检测出bug
float GameConfig::PLAYER_FALL_ACCELERATION = 300.0f; // 玩家手动下落时的额外加速度

float GameConfig::PLAYER_COLLISION_SHRINK_RATIO = 0.3f;  // 仅水平方向缩小

// 平台配置
sf::Vector2f GameConfig::PLATFORM_SIZE(100.0f, 12.0f);
float GameConfig::PLATFORM_GENERATE_INTERVAL = 0.5f;

float GameConfig::PLATFORM_NORMAL_PROBABILITY = 0.5f;      // 50%
float GameConfig::PLATFORM_ROLLING_PROBABILITY = 0.15f;    // 15%
float GameConfig::PLATFORM_BOUNCY_PROBABILITY = 0.15f;     // 15%
float GameConfig::PLATFORM_FRAGILE_PROBABILITY = 0.15f;    // 15%
float GameConfig::PLATFORM_SPIKED_PROBABILITY = 0.05f;     // 5%

float GameConfig::PLATFORM_ROLLING_SPEED = 100.0f;
float GameConfig::PLATFORM_BOUNCY_FORCE = 500.0f;       // 弹跳平台的弹跳力
float GameConfig::PLATFORM_FRAGILE_BREAK_DELAY = 0.3f;  // 脆弱平台破碎延迟时间
int GameConfig::PLATFORM_GENERATION_MAX_ATTEMPTS = 5;       // 最多尝试找到合适位置的次数

// 敌人配置
sf::Vector2f GameConfig::ENEMY_SIZE(40.0f, 40.0f);
float GameConfig::ENEMY_SPEED = 50.0f;
float GameConfig::ENEMY_SHOOT_INTERVAL = 2.0f;           // 敌人射击间隔时间(秒)
float GameConfig::ENEMY_BULLET_SPEED = 200.0f;           // 敌人子弹速度

float GameConfig::ENEMY_GENERATE_INTERVAL = 3.0f;       // 敌人生成间隔时间(秒)，用于初始设置
float GameConfig::ENEMY_SPAWN_MIN_INTERVAL = 3.0f;
float GameConfig::ENEMY_SPAWN_MAX_INTERVAL = 6.0f;
int GameConfig::ENEMY_SCORE_VALUE = 5;

// 豆子配置
sf::Vector2f GameConfig::PICKUP_SIZE(20.0f, 20.0f);
float GameConfig::PICKUP_GENERATE_INTERVAL = 1.5f;
float GameConfig::PICKUP_NORMAL_PROBABILITY = 0.8f;
float GameConfig::PICKUP_STAR_PROBABILITY = 0.2f;
float GameConfig::PICKUP_SPAWN_MIN_INTERVAL = 0.8f;
float GameConfig::PICKUP_SPAWN_MAX_INTERVAL = 1.6f;
int GameConfig::PICKUP_NORMAL_SCORE = 2;
int GameConfig::PICKUP_STAR_SCORE = 5;

// 子弹配置
sf::Vector2f GameConfig::BULLET_SIZE(16.0f, 16.0f);
float GameConfig::BULLET_SPEED = 300.0f;
float GameConfig::BULLET_LIFETIME = 3.0f;

// 技能配置
float GameConfig::SKILL_ARROW_COOLDOWN = 1.0f;
float GameConfig::SKILL_SPRINT_COOLDOWN = 5.0f;
float GameConfig::SKILL_SPRINT_DURATION = 1.0f;
float GameConfig::SKILL_SPRINT_SPEED_MULTIPLIER = 2.0f;
float GameConfig::SKILL_SPRINT_DISTANCE = 100.0f;

// 物理配置
float GameConfig::PHYSICS_GRAVITY = 800.0f;
float GameConfig::PHYSICS_FRICTION = 0.8f;
float GameConfig::PHYSICS_AIR_RESISTANCE = 0.95f;

// 游戏难度配置
int GameConfig::INITIAL_PLATFORM_COUNT = 3;
float GameConfig::DIFFICULTY_SCALE_FACTOR = 1.1f;
float GameConfig::DIFFICULTY_SCALE_INTERVAL = 30.0f;
float GameConfig::MAX_DIFFICULTY_MULTIPLIER = 3.0f;

// 游戏流程配置
float GameConfig::INITIAL_SCROLL_SPEED = 100.0f;
float GameConfig::GENERATE_INTERVAL_VARIANCE = 0.25f;
float GameConfig::BULLET_SIZE_SCALE = 0.6f;


// ====================================
// 运行时修改方法
// ====================================


}
}
