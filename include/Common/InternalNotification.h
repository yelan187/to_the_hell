/**
 * @file InternalNotification.h
 * @brief 内部通知系统定义
 * 
 * 本文件定义了MVVM架构中Model层到ViewModel层的内部通信结构。
 * 主要职责：
 * 1. 定义Model层内部使用的通知数据结构
 * 2. 使用Model层的具体枚举类型，保持类型安全
 * 3. 为ViewModel层提供详细的实体状态信息
 * 
 * 与NotificationBase.h的区别：
 * - InternalNotification.h：Model -> ViewModel 内部通信，使用Model层枚举
 * - NotificationBase.h：ViewModel -> View 外部通信，使用基础数据类型
 * 
 * 数据流向：Model(_frameInfo) -> ViewModel(处理转换) -> View(frameInfo)
 * ViewModel层负责将Model层的枚举类型转换为View层可用的基础类型
 */

#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include "Common/ParamBase.h"
#include "Common/NotificationBase.h"

// 前向声明Model层枚举（仅用于ViewModel层内部通信）
// 这样设计避免了头文件循环依赖，同时保持了类型安全
namespace Model {
    namespace Entities {
        enum class PlayerState;     ///< 玩家状态枚举（如站立、跳跃、行走等）
        enum class PlatformType;    ///< 平台类型枚举（如普通、滚动、脆弱等）
        enum class EnemyType;       ///< 敌人类型枚举（如Ghost等）
        enum class PickupType;      ///< 拾取物类型枚举（如Normal、Star等）
    }
}

namespace Common {

/**
 * @struct _frameInfo
 * @brief 游戏帧信息结构体（Model层内部版本）
 * 
 * 专用于Model层到ViewModel层的内部通信，使用Model层的具体枚举类型。
 * ViewModel层接收此结构体后，会将其转换为NotificationBase.h中的frameInfo，
 * 将枚举类型转换为基础数据类型供View层使用。
 * 
 * 命名约定：以下划线开头表示内部使用
 */
typedef struct _frameInfo{    
    /**
     * @struct PlayerInfo
     * @brief 玩家信息（Model层内部版本）
     * 
     * 与NotificationBase.h中的PlayerInfo区别：
     * - 使用PlayerState枚举而非纹理指针
     * - ViewModel层根据state选择合适的纹理
     */
    struct PlayerInfo {
        sf::Vector2f position;                    ///< 玩家位置
        sf::Vector2f size;                        ///< 玩家大小
        Model::Entities::PlayerState state;      ///< 玩家状态（枚举类型）
    } player_info;

    /**
     * @struct PlatformInfo
     * @brief 平台信息（Model层内部版本）
     * 
     * 与NotificationBase.h中的PlatformInfo区别：
     * - 使用PlatformType枚举而非int类型
     * - 不包含颜色和大小，由ViewModel层根据类型决定
     */
    typedef struct {
        sf::Vector2f position;                    ///< 平台位置
        Model::Entities::PlatformType type;      ///< 平台类型（枚举类型）
        bool rolling_direction;                  ///< 滚动方向（true=右，false=左）
    } PlatformInfo;
    std::map<int, PlatformInfo> platforms_info;  ///< 平台信息映射表（ID -> 信息）
    std::vector<int> platforms_id;               ///< 平台ID列表（用于遍历顺序）
    
    /**
     * @struct EnemyInfo
     * @brief 敌人信息（Model层内部版本）
     * 
     * 与NotificationBase.h中的EnemyInfo区别：
     * - 使用EnemyType枚举而非颜色
     * - ViewModel层根据type决定渲染属性
     */
    typedef struct {
        sf::Vector2f position;                   ///< 敌人位置
        sf::Vector2f size;                       ///< 敌人大小
        Model::Entities::EnemyType type;         ///< 敌人类型（枚举类型）
        sf::Vector2f facing_direction;           ///< 面向方向向量
    } EnemyInfo;
    std::map<int, EnemyInfo> enemies_info;       ///< 敌人信息映射表（ID -> 信息）
    std::vector<int> enemies_id;                 ///< 敌人ID列表（用于遍历顺序）
    
    /**
     * @struct BulletInfo
     * @brief 子弹信息（Model层内部版本）
     * 
     * 与NotificationBase.h中的BulletInfo基本相同，
     * 因为子弹的属性相对简单，不需要复杂的枚举类型
     */
    typedef struct {
        sf::Vector2f position;               ///< 子弹位置
        sf::Vector2f size;                   ///< 子弹大小
        bool is_player_bullet;               ///< 是否为玩家箭矢
        sf::Vector2f velocity;               ///< 子弹速度方向
    } BulletInfo;
    std::map<int, BulletInfo> bullets_info;      ///< 子弹信息映射表（ID -> 信息）
    std::vector<int> bullets_id;                 ///< 子弹ID列表（用于遍历顺序）
    
    /**
     * @struct PickupInfo
     * @brief 拾取物信息（Model层内部版本）
     * 
     * 与NotificationBase.h中的PickupInfo区别：
     * - 使用PickupType枚举而非int类型
     * - ViewModel层根据type决定颜色和渲染样式
     */
    typedef struct {
        sf::Vector2f position;               ///< 拾取物位置
        sf::Vector2f size;                   ///< 拾取物大小
        Model::Entities::PickupType type;    ///< 拾取物类型（枚举类型）
    } PickupInfo;
    std::map<int, PickupInfo> pickups_info;      ///< 拾取物信息映射表（ID -> 信息）
    std::vector<int> pickups_id;                 ///< 拾取物ID列表（用于遍历顺序）
    
    /**
     * @struct SkillInfo
     * @brief 技能信息（Model层内部版本）
     * 
     * 与NotificationBase.h中的SkillInfo基本相同，
     * 因为技能状态信息主要使用基础数据类型
     */
    typedef struct {
        int skill_type;                      ///< 技能类型（0=箭矢射击, 1=冲刺）
        float cooldown_progress;             ///< 冷却进度（0.0=可用, 1.0=完全冷却）
        bool is_available;                   ///< 是否可用
    } SkillInfo;
    std::vector<SkillInfo> skills_info;          ///< 技能信息列表（按技能ID顺序）
} _FrameInfo;

/**
 * @typedef _ChangeGameFrameParam
 * @brief Model层到ViewModel层的帧更新通知参数类型
 * 
 * 使用与NotificationBase.h相同的模板类，确保通知系统的兼容性。
 * ViewModel层接收此类型的通知后，会将_FrameInfo转换为FrameInfo。
 */
typedef Common::NotificationTypeParam<_FrameInfo> _ChangeGameFrameParam;

}
