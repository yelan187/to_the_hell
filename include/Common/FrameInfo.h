#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <chrono>

namespace Common {
// accessible for view/viewmodel layers to bind properties
typedef struct frameInfo{
    /**
     * @struct PlayerInfo
     * @brief 玩家渲染信息
     */
    struct PlayerInfo {
        sf::Vector2f position;       ///< 玩家位置
        sf::Vector2f size;           ///< 玩家大小
        sf::Texture* texture;        ///< 玩家纹理指针
    } player_info;

    /**
     * @struct PlatformInfo
     * @brief 平台渲染信息（View层版本）
     */
    typedef struct {
        sf::Vector2f position;       ///< 平台位置
        sf::Color color;             ///< 平台颜色
        sf::Vector2f size;           ///< 平台大小
        int platform_type;           ///< 平台类型（对应PlatformType枚举的int值）
        bool rolling_direction;      ///< 滚动方向（true=右，false=左）仅滚动平台使用
    } PlatformInfo;
    std::map<int, PlatformInfo> platforms_info;  ///< 平台信息映射表（ID -> 信息）
    std::vector<int> platforms_id;               ///< 平台ID列表（用于遍历顺序）
    
    /**
     * @struct EnemyInfo
     * @brief 敌人渲染信息（View层版本）
     */
    typedef struct {
        sf::Vector2f position;       ///< 敌人位置
        sf::Vector2f size;           ///< 敌人大小
        sf::Color color;             ///< 敌人颜色
        sf::Vector2f facing_direction; ///< 面向方向向量
    } EnemyInfo;
    std::map<int, EnemyInfo> enemies_info;       ///< 敌人信息映射表（ID -> 信息）
    std::vector<int> enemies_id;                 ///< 敌人ID列表（用于遍历顺序）
    
    /**
     * @struct BulletInfo
     * @brief 子弹渲染信息（View层版本）
     */
    typedef struct {
        sf::Vector2f position;       ///< 子弹位置
        sf::Vector2f size;           ///< 子弹大小
        sf::Color color;             ///< 子弹颜色
        bool is_player_bullet;       ///< 是否为玩家箭矢（用于区分渲染样式）
        sf::Vector2f velocity;       ///< 子弹速度方向（用于渲染方向）
    } BulletInfo;
    std::map<int, BulletInfo> bullets_info;      ///< 子弹信息映射表（ID -> 信息）
    std::vector<int> bullets_id;                 ///< 子弹ID列表（用于遍历顺序）
    
    /**
     * @struct PickupInfo
     * @brief 拾取物渲染信息（View层版本）
     */
    typedef struct {
        sf::Vector2f position;       ///< 拾取物位置
        sf::Vector2f size;           ///< 拾取物大小
        sf::Color color;             ///< 拾取物颜色
        int pickup_type;             ///< 拾取物类型（0=圆形豆子, 1=五角星豆子）
    } PickupInfo;
    std::map<int, PickupInfo> pickups_info;      ///< 拾取物信息映射表（ID -> 信息）
    std::vector<int> pickups_id;                 ///< 拾取物ID列表（用于遍历顺序）
    
    /**
     * @struct SkillInfo
     * @brief 技能状态信息（View层版本）
     */
    typedef struct {
        int skill_type;              ///< 技能类型（0=箭矢射击, 1=冲刺）
        float cooldown_progress;     ///< 冷却进度（0.0=可用, 1.0=完全冷却）
        bool is_available;           ///< 是否可用（用于UI显示状态）
    } SkillInfo;
    std::vector<SkillInfo> skills_info;          ///< 技能信息列表（按技能ID顺序）
    
    /**
     * @struct BackgroundInfo
     * @brief 背景信息（View层版本）
     */
    typedef struct {
        std::string background_file; ///< 背景图片文件路径
        bool changed;                ///< 背景是否已改变（用于View层重新加载）
    } BackgroundInfo;
    BackgroundInfo background_info;              ///< 背景信息
} FrameInfo;

}