#include "Common/Config/GameConfig.h"
#include <fstream>
#include <iostream>

namespace Common {
namespace Config {

// 静态成员变量定义
int GameConfig::SCORE_UPDATE_INTERVAL;
int GameConfig::SCORE_INCREMENT;

sf::Vector2f GameConfig::PLAYER_SIZE;
float GameConfig::PLAYER_WALK_SPEED;
float GameConfig::PLAYER_JUMP_FORCE;
float GameConfig::PLAYER_GRAVITY;
float GameConfig::PLAYER_MAX_FALL_SPEED;
float GameConfig::PLAYER_FALL_ACCELERATION;
float GameConfig::PLAYER_COLLISION_SHRINK_RATIO;
int GameConfig::PLAYER_MAX_HP;
int GameConfig::PLAYER_INITIAL_HP;

sf::Vector2f GameConfig::PLATFORM_SIZE;
float GameConfig::PLATFORM_GENERATE_INTERVAL;
float GameConfig::PLATFORM_GENERATE_INTERVAL_VARIANCE;
float GameConfig::PLATFORM_NORMAL_PROBABILITY;
float GameConfig::PLATFORM_ROLLING_PROBABILITY;
float GameConfig::PLATFORM_BOUNCY_PROBABILITY;
float GameConfig::PLATFORM_FRAGILE_PROBABILITY;
float GameConfig::PLATFORM_SPIKED_PROBABILITY;
float GameConfig::PLATFORM_ROLLING_SPEED;
float GameConfig::PLATFORM_BOUNCY_FORCE;
float GameConfig::PLATFORM_FRAGILE_BREAK_DELAY;
int GameConfig::PLATFORM_GENERATION_MAX_ATTEMPTS;

sf::Vector2f GameConfig::ENEMY_SIZE;
float GameConfig::ENEMY_SPEED;
float GameConfig::ENEMY_SHOOT_INTERVAL;
float GameConfig::ENEMY_BULLET_SPEED;
float GameConfig::ENEMY_GENERATE_INTERVAL;
float GameConfig::ENEMY_SPAWN_MIN_INTERVAL;
float GameConfig::ENEMY_SPAWN_MAX_INTERVAL;
int GameConfig::ENEMY_SCORE_VALUE;

sf::Vector2f GameConfig::PICKUP_SIZE;
float GameConfig::PICKUP_GENERATE_INTERVAL;
float GameConfig::PICKUP_NORMAL_PROBABILITY;
float GameConfig::PICKUP_STAR_PROBABILITY;
float GameConfig::PICKUP_SPAWN_MIN_INTERVAL;
float GameConfig::PICKUP_SPAWN_MAX_INTERVAL;
int GameConfig::PICKUP_NORMAL_SCORE;
int GameConfig::PICKUP_STAR_SCORE;

sf::Vector2f GameConfig::BULLET_SIZE;
float GameConfig::BULLET_SPEED;
int GameConfig::BULLET_PLAYER_DAMAGE;
int GameConfig::BULLET_ENEMY_DAMAGE;

float GameConfig::SKILL_ARROW_COOLDOWN;
int GameConfig::SKILL_ARROW_DAMAGE;
float GameConfig::SKILL_SPRINT_COOLDOWN;
float GameConfig::SKILL_SPRINT_DURATION;
float GameConfig::SKILL_SPRINT_SPEED_MULTIPLIER;
float GameConfig::SKILL_SPRINT_DISTANCE;
int GameConfig::SKILL_SPRINT_RESET_KILL_COUNT;
float GameConfig::SKILL_GROUND_PENETRATION_COOLDOWN;

float GameConfig::SCROLL_SPEED;
float GameConfig::BACKGROUND_TRANSITION_SPEED;

// 恢复所有配置为初始值
void GameConfig::resetToInitialValues() {
    
    std::cout << "Resetting GameConfig to initial values..." << std::endl;

    // 分数配置
    SCORE_UPDATE_INTERVAL = 10;                         // 分数更新间隔(秒)
    SCORE_INCREMENT = 1;                                // 每秒得分

    // 玩家配置
    PLAYER_SIZE = sf::Vector2f(60.0f, 60.0f);           // 玩家大小
    PLAYER_WALK_SPEED = 150.0f;                         // 玩家行走速度
    PLAYER_JUMP_FORCE = 350.0f;                         // 玩家跳跃力度
    PLAYER_GRAVITY = 500.0f;                            // 玩家重力加速度
    PLAYER_MAX_FALL_SPEED = 600.0f;                     // 玩家最大下落速度
    PLAYER_FALL_ACCELERATION = 300.0f;                  // 玩家手动下落时的额外加速度
    PLAYER_COLLISION_SHRINK_RATIO = 0.3f;               // 玩家碰撞框水平方向缩小比例（0-1）
    PLAYER_MAX_HP = 3;                                  // 玩家最大生命值
    PLAYER_INITIAL_HP = 3;                              // 玩家初始生命值

    // 平台配置
    PLATFORM_SIZE = sf::Vector2f(100.0f, 12.0f);        // 平台大小
    PLATFORM_GENERATE_INTERVAL = 0.5f;                  // 平台生成间隔时间(秒)
    PLATFORM_GENERATE_INTERVAL_VARIANCE = 0.25f;        // 平台生成间隔随机变化范围（±秒）
    PLATFORM_NORMAL_PROBABILITY = 0.5f;                 // 普通平台生成概率
    PLATFORM_ROLLING_PROBABILITY = 0.15f;               // 滚动平台生成概率
    PLATFORM_BOUNCY_PROBABILITY = 0.15f;                // 弹跳平台生成概率
    PLATFORM_FRAGILE_PROBABILITY = 0.15f;               // 脆弱平台生成概率
    PLATFORM_SPIKED_PROBABILITY = 0.05f;                // 带刺平台生成概率
    PLATFORM_ROLLING_SPEED = 100.0f;                    // 滚动平台速度
    PLATFORM_BOUNCY_FORCE = 400.0f;                     // 弹跳平台的弹跳力
    PLATFORM_FRAGILE_BREAK_DELAY = 0.3f;                // 脆弱平台破碎延迟时间(秒)
    PLATFORM_GENERATION_MAX_ATTEMPTS = 5;               // 平台生成最大尝试次数

    // 敌人配置
    ENEMY_SIZE = sf::Vector2f(40.0f, 40.0f);            // 敌人大小
    ENEMY_SPEED = 50.0f;                                // 敌人移动速度
    ENEMY_SHOOT_INTERVAL = 2.0f;                        // 敌人射击间隔时间(秒)
    ENEMY_BULLET_SPEED = 200.0f;                        // 敌人子弹速度
    ENEMY_GENERATE_INTERVAL = 5.0f;                     // 敌人生成间隔时间(秒)，用于初始设置
    ENEMY_SPAWN_MIN_INTERVAL = 5.0f;                    // 敌人生成最小间隔
    ENEMY_SPAWN_MAX_INTERVAL = 8.0f;                    // 敌人生成最大间隔
    ENEMY_SCORE_VALUE = 5;                              // 击败敌人得分

    // 豆子配置
    PICKUP_SIZE = sf::Vector2f(20.0f, 20.0f);           // 豆子大小
    PICKUP_GENERATE_INTERVAL = 1.5f;                    // 豆子生成间隔时间(秒)
    PICKUP_NORMAL_PROBABILITY = 0.8f;                   // 普通豆子概率
    PICKUP_STAR_PROBABILITY = 0.2f;                     // 五角星豆子概率
    PICKUP_SPAWN_MIN_INTERVAL = 0.8f;                   // 豆子生成最小间隔
    PICKUP_SPAWN_MAX_INTERVAL = 1.6f;                   // 豆子生成最大间隔
    PICKUP_NORMAL_SCORE = 2;                            // 普通豆子得分
    PICKUP_STAR_SCORE = 5;                              // 五角星豆子得分

    // 子弹配置
    BULLET_SIZE = sf::Vector2f(16.0f, 16.0f);           // 子弹大小
    BULLET_SPEED = 500.0f;                              // 玩家子弹速度
    BULLET_PLAYER_DAMAGE = 1;                           // 玩家子弹伤害
    BULLET_ENEMY_DAMAGE = 1;                            // 敌人子弹伤害

    // 技能配置
    SKILL_ARROW_COOLDOWN = 1.0f;                        // 箭矢技能冷却时间(秒)
    SKILL_ARROW_DAMAGE = 10;                            // 箭矢技能伤害
    SKILL_SPRINT_COOLDOWN = 5.0f;                       // 冲刺技能冷却时间(秒)
    SKILL_SPRINT_DURATION = 1.0f;                       // 冲刺持续时间(秒)
    SKILL_SPRINT_SPEED_MULTIPLIER = 2.0f;               // 冲刺速度倍数
    SKILL_SPRINT_DISTANCE = 100.0f;                     // 冲刺距离(像素)
    SKILL_SPRINT_RESET_KILL_COUNT = 2;                  // 冲刺重置击杀数
    SKILL_GROUND_PENETRATION_COOLDOWN = 0.5f;           // 地面穿透技能冷却时间(秒)

    // 游戏流程配置
    SCROLL_SPEED = 100.0f;                              // 滚动速度
    
    // 背景配置
    BACKGROUND_TRANSITION_SPEED = 2.0f;                 // 背景过渡速度（2秒完成过渡）
}

}
}
