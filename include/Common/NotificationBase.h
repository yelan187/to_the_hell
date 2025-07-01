/**
 * @file NotificationBase.h
 * @brief 外部通知系统基础定义
 * 
 * 本文件定义了MVVM架构中用于View层和ViewModel层之间通信的通知系统。
 * 主要职责：
 * 1. 定义通知ID枚举，标识不同类型的通知事件
 * 2. 提供通用的通知参数基类和模板类
 * 3. 定义各类游戏数据的外部传输结构（View层可见的简化版本）
 * 
 * 与InternalNotification.h的区别：
 * - NotificationBase.h：用于跨层通信，View层可访问，使用基础数据类型
 * - InternalNotification.h：用于Model->ViewModel内部通信，使用Model层枚举类型
 * 
 * 通知流向：Model -> ViewModel -> View
 * 数据转换：Model枚举 -> ViewModel处理 -> View基础类型
 */

#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <chrono>
#include "Common/ParamBase.h"

namespace Common {

/**
 * @enum NotificationId
 * @brief 通知事件标识符
 * 
 * 定义了MVVM架构中不同层之间的通知类型。
 * 命名规则：动词+名词，描述具体的通知行为
 */
enum class NotificationId {
    // 主菜单相关通知
    ChangeCurrentSelection,      ///< 改变当前选中项（菜单导航）
    ChangeBackgroundParticles,   ///< 更新背景粒子效果
    
    // 游戏相关通知
    ChangeGameFrame,             ///< 更新游戏帧数据（ViewModel -> View）
    _ChangeGameFrame,            ///< Model层到ViewModel层的内部通知
    GameOver                     ///< 游戏结束通知
};

/**
 * @class NotificationParam
 * @brief 通知参数基类
 * 
 * 所有通知参数的基类，包含通知ID用于标识通知类型。
 * 继承自Param基类，确保参数传递的统一性。
 */
class NotificationParam : public Param {
public:
    NotificationId id;          ///< 通知类型标识符
    virtual ~NotificationParam() = default;
};

/// 通知回调函数类型定义
typedef void (* NotificationFunc)(NotificationParam*, void* p);

/**
 * @class NotificationTypeParam
 * @brief 通知参数模板类
 * @tparam T 具体的参数数据类型
 * 
 * 为不同类型的通知数据提供统一的封装模板。
 * 使用模板可以避免类型转换，提高类型安全性。
 */
template <typename T>
class NotificationTypeParam : public NotificationParam {
public:
    T value;                    ///< 具体的通知数据
    virtual ~NotificationTypeParam() = default;
};

// ==================== 通知参数类型定义 ====================
// 以下类型定义与NotificationId一一对应，用于具体通知的数据传递

// 主菜单相关通知参数
/// 改变当前选中项的参数（包含选中项索引）
typedef NotificationTypeParam<int> ChangeCurrentSelectionParam;
/// 更新背景粒子的参数（包含粒子位置数组指针）
typedef NotificationTypeParam<std::vector<sf::Vector2f>*> ChangeBackgroundParticlesParam;

// 游戏相关通知参数
/**
 * @struct frameInfo
 * @brief 游戏帧信息结构体（View层可见版本）
 * 
 * 包含游戏中所有实体的渲染信息，用于View层显示。
 * 所有数据类型都是基础类型或SFML类型，View层可以直接使用。
 * 
 * 与InternalNotification.h中的_frameInfo区别：
 * - frameInfo：使用基础数据类型（如int、sf::Color等），供View层使用
 * - _frameInfo：使用Model层枚举类型，供ViewModel层内部处理
 */
typedef struct frameInfo{
    // 游戏状态文本信息
    std::string total_score_text;    ///< 总分数显示文本
    std::string game_time_text;      ///< 游戏时间显示文本
    std::string debug_info_text;     ///< 调试信息显示文本
    
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
} FrameInfo;

/// 游戏帧更新通知参数类型
typedef NotificationTypeParam<FrameInfo> ChangeGameFrameParam;

/**
 * @struct gameOver
 * @brief 游戏结束信息结构体
 */
typedef struct gameOver {
    int total_score;                 ///< 最终总分数
    std::chrono::seconds game_time;  ///< 游戏总时长
} GameOver;

/// 游戏结束通知参数类型
typedef NotificationTypeParam<GameOver> GameOverNotificationParam;
}
