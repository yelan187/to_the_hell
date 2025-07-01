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
float GameConfig::PLAYER_SPEED = 200.0f;
float GameConfig::PLAYER_JUMP_FORCE = 400.0f;
float GameConfig::PLAYER_GRAVITY = 800.0f;
float GameConfig::PLAYER_MAX_FALL_SPEED = 600.0f;
float GameConfig::PLAYER_INITIAL_Y_RATIO = 0.2f;  // 1/5 = 0.2

// 平台配置
sf::Vector2f GameConfig::PLATFORM_SIZE(100.0f, 12.0f);
float GameConfig::PLATFORM_GENERATE_INTERVAL = 0.5f;
float GameConfig::PLATFORM_NORMAL_PROBABILITY = 0.5f;      // 50%
float GameConfig::PLATFORM_ROLLING_PROBABILITY = 0.15f;    // 15%
float GameConfig::PLATFORM_BOUNCY_PROBABILITY = 0.15f;     // 15%
float GameConfig::PLATFORM_FRAGILE_PROBABILITY = 0.15f;    // 15%
float GameConfig::PLATFORM_SPIKED_PROBABILITY = 0.05f;     // 5%
float GameConfig::PLATFORM_ROLLING_SPEED = 100.0f;
int GameConfig::PLATFORM_GENERATION_MAX_ATTEMPTS = 5;       // 最多尝试找到合适位置次数

// 敌人配置
sf::Vector2f GameConfig::ENEMY_SIZE(40.0f, 40.0f);
float GameConfig::ENEMY_SPEED = 80.0f;
float GameConfig::ENEMY_GENERATE_INTERVAL = 3.0f;
float GameConfig::ENEMY_GENERATE_PROBABILITY = 0.6f;
float GameConfig::ENEMY_SPAWN_MIN_INTERVAL = 3.0f;
float GameConfig::ENEMY_SPAWN_MAX_INTERVAL = 6.0f;
int GameConfig::ENEMY_SCORE_VALUE = 50;

// 豆子配置
sf::Vector2f GameConfig::PICKUP_SIZE(20.0f, 20.0f);
float GameConfig::PICKUP_GENERATE_INTERVAL = 1.5f;
float GameConfig::PICKUP_NORMAL_PROBABILITY = 0.8f;
float GameConfig::PICKUP_STAR_PROBABILITY = 0.2f;
float GameConfig::PICKUP_SPAWN_MIN_INTERVAL = 0.8f;
float GameConfig::PICKUP_SPAWN_MAX_INTERVAL = 1.6f;
int GameConfig::PICKUP_NORMAL_SCORE = 10;
int GameConfig::PICKUP_STAR_SCORE = 50;

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

// 默认值初始化标志
// bool GameConfig::defaults_initialized = false; // 删除不需要的变量

// ====================================
// 运行时修改方法
// ====================================
void GameConfig::setPlayerSpeed(float speed) {
    if (speed > 0) {
        PLAYER_SPEED = speed;
    }
}

void GameConfig::setEnemyGenerateRate(float interval) {
    if (interval > 0) {
        ENEMY_GENERATE_INTERVAL = interval;
    }
}

void GameConfig::setPlatformGenerateRate(float interval) {
    if (interval > 0) {
        PLATFORM_GENERATE_INTERVAL = interval;
    }
}

void GameConfig::setPickupGenerateRate(float interval) {
    if (interval > 0) {
        PICKUP_GENERATE_INTERVAL = interval;
    }
}

void GameConfig::setDifficultyScale(float factor) {
    if (factor >= 1.0f) {
        DIFFICULTY_SCALE_FACTOR = factor;
    }
}

void GameConfig::resetToDefaults() {
    // 重置为初始默认值
    PLAYER_SIZE = sf::Vector2f(60.0f, 60.0f);
    PLAYER_SPEED = 200.0f;
    PLAYER_JUMP_FORCE = 400.0f;
    PLAYER_GRAVITY = 800.0f;
    PLAYER_MAX_FALL_SPEED = 600.0f;
    PLAYER_INITIAL_Y_RATIO = 0.2f;
    
    PLATFORM_SIZE = sf::Vector2f(100.0f, 12.0f);
    PLATFORM_GENERATE_INTERVAL = 2.0f;
    PLATFORM_NORMAL_PROBABILITY = 0.5f;      // 50%
    PLATFORM_ROLLING_PROBABILITY = 0.15f;    // 15%
    PLATFORM_BOUNCY_PROBABILITY = 0.15f;     // 15%
    PLATFORM_FRAGILE_PROBABILITY = 0.15f;    // 15%
    PLATFORM_SPIKED_PROBABILITY = 0.05f;     // 5%
    PLATFORM_ROLLING_SPEED = 100.0f;
    PLATFORM_GENERATION_MAX_ATTEMPTS = 5;
    
    ENEMY_SIZE = sf::Vector2f(40.0f, 40.0f);
    ENEMY_SPEED = 80.0f;
    ENEMY_GENERATE_INTERVAL = 3.0f;
    ENEMY_GENERATE_PROBABILITY = 0.6f;
    ENEMY_SPAWN_MIN_INTERVAL = 3.0f;
    ENEMY_SPAWN_MAX_INTERVAL = 6.0f;
    ENEMY_SCORE_VALUE = 50;
    
    PICKUP_SIZE = sf::Vector2f(20.0f, 20.0f);
    PICKUP_GENERATE_INTERVAL = 1.5f;
    PICKUP_NORMAL_PROBABILITY = 0.8f;
    PICKUP_STAR_PROBABILITY = 0.2f;
    PICKUP_SPAWN_MIN_INTERVAL = 0.8f;
    PICKUP_SPAWN_MAX_INTERVAL = 1.6f;
    PICKUP_NORMAL_SCORE = 10;
    PICKUP_STAR_SCORE = 50;
    
    BULLET_SIZE = sf::Vector2f(8.0f, 8.0f);
    BULLET_SPEED = 300.0f;
    BULLET_LIFETIME = 3.0f;
    
    SKILL_ARROW_COOLDOWN = 1.0f;
    SKILL_SPRINT_COOLDOWN = 5.0f;
    SKILL_SPRINT_DURATION = 1.0f;
    SKILL_SPRINT_SPEED_MULTIPLIER = 2.0f;
    SKILL_SPRINT_DISTANCE = 100.0f;
    
    PHYSICS_GRAVITY = 800.0f;
    PHYSICS_FRICTION = 0.8f;
    PHYSICS_AIR_RESISTANCE = 0.95f;
    
    INITIAL_PLATFORM_COUNT = 3;
    DIFFICULTY_SCALE_FACTOR = 1.1f;
    DIFFICULTY_SCALE_INTERVAL = 30.0f;
    MAX_DIFFICULTY_MULTIPLIER = 3.0f;
    
    INITIAL_SCROLL_SPEED = 100.0f;
    GENERATE_INTERVAL_VARIANCE = 0.25f;
    BULLET_SIZE_SCALE = 0.6f;
}

}
}
