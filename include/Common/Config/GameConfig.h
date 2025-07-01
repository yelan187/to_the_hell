#pragma once
#include <SFML/Graphics.hpp>

namespace Common {
namespace Config {

// ====================================
// 游戏配置 - 游戏玩法相关可调参数
// ====================================
class GameConfig {
public:
    // ====================================
    // 玩家配置
    // ====================================
    static sf::Vector2f PLAYER_SIZE;
    static float PLAYER_WALK_SPEED;
    static float PLAYER_JUMP_FORCE;
    static float PLAYER_GRAVITY;
    static float PLAYER_MAX_FALL_SPEED;
    static float PLAYER_FALL_ACCELERATION;       // 玩家手动下落时的额外加速度
    static float PLAYER_INITIAL_Y_RATIO;         // 玩家初始Y位置比例（相对窗口高度）
    static float PLAYER_COLLISION_SHRINK_RATIO;  // 玩家碰撞框水平方向缩小比例（0-1）
    
    // ====================================
    // 平台配置
    // ====================================
    static sf::Vector2f PLATFORM_SIZE;
    static float PLATFORM_GENERATE_INTERVAL;     // 平台生成间隔时间(秒)

    static float PLATFORM_NORMAL_PROBABILITY;    // 普通平台生成概率
    static float PLATFORM_ROLLING_PROBABILITY;   // 滚动平台生成概率
    static float PLATFORM_BOUNCY_PROBABILITY;    // 弹跳平台生成概率
    static float PLATFORM_FRAGILE_PROBABILITY;   // 脆弱平台生成概率
    static float PLATFORM_SPIKED_PROBABILITY;    // 带刺平台生成概率

    static float PLATFORM_ROLLING_SPEED;         // 滚动平台速度
    static float PLATFORM_BOUNCY_FORCE;         // 弹跳平台的弹跳力
    static float PLATFORM_FRAGILE_BREAK_DELAY;  // 脆弱平台破碎延迟时间(秒)
    static int PLATFORM_GENERATION_MAX_ATTEMPTS; // 平台生成最大尝试次数
    
    // ====================================
    // 敌人配置
    // ====================================
    static sf::Vector2f ENEMY_SIZE;
    static float ENEMY_SPEED;
    static float ENEMY_SHOOT_INTERVAL;           // 敌人射击间隔时间(秒)
    static float ENEMY_BULLET_SPEED;             // 敌人子弹速度

    static float ENEMY_GENERATE_INTERVAL;        // 敌人生成间隔时间(秒)，用于初始设置
    static float ENEMY_SPAWN_MIN_INTERVAL;       // 敌人生成最小间隔
    static float ENEMY_SPAWN_MAX_INTERVAL;       // 敌人生成最大间隔
    static int ENEMY_SCORE_VALUE;                // 击败敌人得分
    
    // ====================================
    // 豆子配置
    // ====================================
    static sf::Vector2f PICKUP_SIZE;
    static float PICKUP_GENERATE_INTERVAL;       // 豆子生成间隔时间(秒)
    static float PICKUP_NORMAL_PROBABILITY;      // 普通豆子概率
    static float PICKUP_STAR_PROBABILITY;        // 五角星豆子概率
    static float PICKUP_SPAWN_MIN_INTERVAL;      // 豆子生成最小间隔
    static float PICKUP_SPAWN_MAX_INTERVAL;      // 豆子生成最大间隔
    static int PICKUP_NORMAL_SCORE;              // 普通豆子得分
    static int PICKUP_STAR_SCORE;                // 五角星豆子得分
    
    // ====================================
    // 子弹配置
    // ====================================
    static sf::Vector2f BULLET_SIZE;
    static float BULLET_SPEED;
    static float BULLET_LIFETIME;                // 子弹存活时间(秒)
    
    // ====================================
    // 技能配置
    // ====================================
    static float SKILL_ARROW_COOLDOWN;           // 箭矢技能冷却时间(秒)
    static float SKILL_SPRINT_COOLDOWN;          // 冲刺技能冷却时间(秒)
    static float SKILL_SPRINT_DURATION;          // 冲刺持续时间(秒)
    static float SKILL_SPRINT_SPEED_MULTIPLIER;  // 冲刺速度倍数
    static float SKILL_SPRINT_DISTANCE;          // 冲刺距离(像素)
    
    // ====================================
    // 物理配置
    // ====================================
    static float PHYSICS_GRAVITY;                // 重力加速度
    static float PHYSICS_FRICTION;               // 摩擦系数
    static float PHYSICS_AIR_RESISTANCE;         // 空气阻力
    
    // ====================================
    // 游戏难度配置
    // ====================================
    static int INITIAL_PLATFORM_COUNT;           // 初始平台数量
    static float DIFFICULTY_SCALE_FACTOR;        // 难度递增因子
    static float DIFFICULTY_SCALE_INTERVAL;      // 难度递增间隔(秒)
    static float MAX_DIFFICULTY_MULTIPLIER;      // 最大难度倍数
    
    // ====================================
    // 游戏流程配置
    // ====================================
    static float INITIAL_SCROLL_SPEED;           // 初始滚动速度
    static float GENERATE_INTERVAL_VARIANCE;     // 生成间隔随机变化范围（±秒）
    static float BULLET_SIZE_SCALE;              // 子弹大小缩放比例
    
    // ====================================
    // 运行时修改方法
    // ====================================

    
private:
    GameConfig() = default; // 静态类，禁止实例化
};

}
}
