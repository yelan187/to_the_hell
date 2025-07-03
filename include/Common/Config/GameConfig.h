#pragma once
#include <SFML/Graphics.hpp>

namespace Common {
namespace Config {

// ====================================
// 游戏配置 - 游戏玩法相关可调参数
// ====================================
class GameConfig {
public:

// 分数配置

static int SCORE_UPDATE_INTERVAL;               // 分数更新间隔(秒)
static int SCORE_INCREMENT;

// 玩家配置
static sf::Vector2f PLAYER_SIZE;                        // 玩家大小
static float PLAYER_WALK_SPEED;                         // 玩家行走速度
static float PLAYER_JUMP_FORCE;                         // 玩家跳跃力度
static float PLAYER_GRAVITY;                            // 玩家重力加速度
static float PLAYER_MAX_FALL_SPEED;                     // 玩家最大下落速度
static float PLAYER_FALL_ACCELERATION;                  // 玩家手动下落时的额外加速度
static float PLAYER_COLLISION_SHRINK_RATIO;             // 玩家碰撞框水平方向缩小比例（0-1）
static int PLAYER_MAX_HP;                               // 玩家最大生命值
static int PLAYER_INITIAL_HP;                           // 玩家初始生命值

// 非子弹伤害配置
static int ENEMY_CONTACT_DAMAGE;                        // 敌人接触伤害

// 平台配置
static sf::Vector2f PLATFORM_SIZE;                      // 平台大小
static float PLATFORM_GENERATE_INTERVAL;                // 平台生成间隔时间(秒)
static float PLATFORM_GENERATE_INTERVAL_VARIANCE;       // 平台生成间隔随机变化范围（±秒）

static float PLATFORM_NORMAL_PROBABILITY;               // 普通平台生成概率
static float PLATFORM_ROLLING_PROBABILITY;              // 滚动平台生成概率
static float PLATFORM_BOUNCY_PROBABILITY;               // 弹跳平台生成概率
static float PLATFORM_FRAGILE_PROBABILITY;              // 脆弱平台生成概率
static float PLATFORM_SPIKED_PROBABILITY;               // 带刺平台生成概率

static float PLATFORM_ROLLING_SPEED;                    // 滚动平台速度
static float PLATFORM_BOUNCY_FORCE;                     // 弹跳平台的弹跳力
static float PLATFORM_FRAGILE_BREAK_DELAY;              // 脆弱平台破碎延迟时间(秒)
static int PLATFORM_GENERATION_MAX_ATTEMPTS;            // 平台生成最大尝试次数

// 敌人配置
static sf::Vector2f ENEMY_SIZE;                         // 敌人大小
static float ENEMY_SPEED;                               // 敌人移动速度
static float ENEMY_SHOOT_INTERVAL;                      // 敌人射击间隔时间(秒)
static float ENEMY_BULLET_SPEED;                        // 敌人子弹速度
static sf::Vector2f ENEMY_BULLET_SIZE;                  // 子弹大小
static int ENEMY_DAMAGE;                                // 敌人对玩家造成的伤害
static int ENEMY_MAX_HP;                                // 敌人最大生命值

static float ENEMY_GENERATE_INTERVAL;                   // 敌人生成间隔时间(秒)，用于初始设置
static float ENEMY_SPAWN_MIN_INTERVAL;                  // 敌人生成最小间隔
static float ENEMY_SPAWN_MAX_INTERVAL;                  // 敌人生成最大间隔
static int ENEMY_SCORE_VALUE;                           // 击败敌人得分

// 豆子配置
static sf::Vector2f PICKUP_SIZE;                        // 豆子大小
static float PICKUP_GENERATE_INTERVAL;                  // 豆子生成间隔时间(秒)
static float PICKUP_NORMAL_PROBABILITY;                 // 普通豆子概率
static float PICKUP_STAR_PROBABILITY;                   // 五角星豆子概率
static float PICKUP_SPAWN_MIN_INTERVAL;                 // 豆子生成最小间隔
static float PICKUP_SPAWN_MAX_INTERVAL;                 // 豆子生成最大间隔
static int PICKUP_NORMAL_SCORE;                         // 普通豆子得分
static int PICKUP_STAR_SCORE;                           // 五角星豆子得分

// 技能配置
static float SKILL_ARROW_COOLDOWN;                      // 箭矢技能冷却时间(秒)
static int SKILL_ARROW_DAMAGE;                          // 箭矢技能伤害
static float SKILL_ARROW_SPEED;                         // 箭矢技能速度
static sf::Vector2f SKILL_ARROW_SIZE;                   // 箭矢技能大小
static float SKILL_SPRINT_COOLDOWN;                     // 冲刺技能冷却时间(秒)
static float SKILL_SPRINT_DURATION;                     // 冲刺持续时间(秒)
static float SKILL_SPRINT_SPEED_MULTIPLIER;             // 冲刺速度倍数
static float SKILL_SPRINT_DISTANCE;                     // 冲刺距离(像素)
static int SKILL_SPRINT_RESET_KILL_COUNT;
static float SKILL_GROUND_PENETRATION_COOLDOWN;

// 游戏流程配置
static float SCROLL_SPEED;                              // 滚动速度

// 背景配置
static float BACKGROUND_TRANSITION_SPEED;               // 背景过渡速度（每秒完成度）

// 效果配置
static int MAX_CHOICES;                                 // 最大选择数量

// 配置管理函数
static void resetToInitialValues();                     // 恢复所有配置为初始值

private:
    GameConfig() = default;                              // 静态类，禁止实例化
};

}
}
