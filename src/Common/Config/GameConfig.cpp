#include "Common/Config/GameConfig.h"
#include <fstream>
#include <iostream>

namespace Common {
namespace Config {

// 玩家配置
sf::Vector2f GameConfig::PLAYER_SIZE(60.0f, 60.0f);
float GameConfig::PLAYER_WALK_SPEED = 150.0f;               // 玩家行走速度
float GameConfig::PLAYER_JUMP_FORCE = 350.0f;               // 玩家跳跃力度
float GameConfig::PLAYER_GRAVITY = 500.0f;                  // 玩家重力加速度
float GameConfig::PLAYER_MAX_FALL_SPEED = 600.0f;           // 玩家最大下落速度
float GameConfig::PLAYER_FALL_ACCELERATION = 300.0f;        // 玩家手动下落时的额外加速度

float GameConfig::PLAYER_COLLISION_SHRINK_RATIO = 0.3f;     // 玩家碰撞框水平方向缩小比例（0-1）

// 平台配置
sf::Vector2f GameConfig::PLATFORM_SIZE(100.0f, 12.0f);
float GameConfig::PLATFORM_GENERATE_INTERVAL = 0.5f;        // 平台生成间隔时间(秒)
float GameConfig::PLATFORM_GENERATE_INTERVAL_VARIANCE = 0.25f; // 平台生成间隔随机变化范围（±秒）

float GameConfig::PLATFORM_NORMAL_PROBABILITY = 0.5f;       // 普通平台生成概率
float GameConfig::PLATFORM_ROLLING_PROBABILITY = 0.15f;     // 滚动平台生成概率
float GameConfig::PLATFORM_BOUNCY_PROBABILITY = 0.15f;      // 弹跳平台生成概率
float GameConfig::PLATFORM_FRAGILE_PROBABILITY = 0.15f;     // 脆弱平台生成概率
float GameConfig::PLATFORM_SPIKED_PROBABILITY = 0.05f;      // 带刺平台生成概率

float GameConfig::PLATFORM_ROLLING_SPEED = 100.0f;          // 滚动平台速度
float GameConfig::PLATFORM_BOUNCY_FORCE = 500.0f;           // 弹跳平台的弹跳力
float GameConfig::PLATFORM_FRAGILE_BREAK_DELAY = 0.3f;      // 脆弱平台破碎延迟时间(秒)
int GameConfig::PLATFORM_GENERATION_MAX_ATTEMPTS = 5;       // 平台生成最大尝试次数

// 敌人配置
sf::Vector2f GameConfig::ENEMY_SIZE(40.0f, 40.0f);
float GameConfig::ENEMY_SPEED = 50.0f;                      // 敌人移动速度
float GameConfig::ENEMY_SHOOT_INTERVAL = 2.0f;              // 敌人射击间隔时间(秒)
float GameConfig::ENEMY_BULLET_SPEED = 200.0f;              // 敌人子弹速度

float GameConfig::ENEMY_GENERATE_INTERVAL = 3.0f;           // 敌人生成间隔时间(秒)，用于初始设置
float GameConfig::ENEMY_SPAWN_MIN_INTERVAL = 3.0f;          // 敌人生成最小间隔
float GameConfig::ENEMY_SPAWN_MAX_INTERVAL = 6.0f;          // 敌人生成最大间隔
int GameConfig::ENEMY_SCORE_VALUE = 5;                      // 击败敌人得分

// 豆子配置
sf::Vector2f GameConfig::PICKUP_SIZE(20.0f, 20.0f);
float GameConfig::PICKUP_GENERATE_INTERVAL = 1.5f;          // 豆子生成间隔时间(秒)
float GameConfig::PICKUP_NORMAL_PROBABILITY = 0.8f;         // 普通豆子概率
float GameConfig::PICKUP_STAR_PROBABILITY = 0.2f;           // 五角星豆子概率
float GameConfig::PICKUP_SPAWN_MIN_INTERVAL = 0.8f;         // 豆子生成最小间隔
float GameConfig::PICKUP_SPAWN_MAX_INTERVAL = 1.6f;         // 豆子生成最大间隔
int GameConfig::PICKUP_NORMAL_SCORE = 2;                    // 普通豆子得分
int GameConfig::PICKUP_STAR_SCORE = 5;                      // 五角星豆子得分

// 子弹配置
sf::Vector2f GameConfig::BULLET_SIZE(16.0f, 16.0f);
float GameConfig::BULLET_SPEED = 300.0f;                    // 玩家子弹速度

// 技能配置
float GameConfig::SKILL_ARROW_COOLDOWN = 1.0f;              // 箭矢技能冷却时间(秒)
float GameConfig::SKILL_SPRINT_COOLDOWN = 5.0f;             // 冲刺技能冷却时间(秒)
float GameConfig::SKILL_SPRINT_DURATION = 1.0f;             // 冲刺持续时间(秒)
float GameConfig::SKILL_SPRINT_SPEED_MULTIPLIER = 2.0f;     // 冲刺速度倍数
float GameConfig::SKILL_SPRINT_DISTANCE = 100.0f;           // 冲刺距离(像素)

// 游戏流程配置
float GameConfig::INITIAL_SCROLL_SPEED = 100.0f;            // 初始滚动速度

}
}
