#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include "Common/ParamBase.h"
#include "Common/NotificationBase.h"

// 前向声明Model层枚举（仅用于ViewModel层内部通信）
namespace Model {
    namespace Entities {
        enum class PlayerState;
        enum class PlatformType;
        enum class EnemyType;
        enum class PickupType;
    }
}

namespace Common {

// Model层到ViewModel层的内部通信结构（不暴露给View层）
typedef struct _frameInfo{    
    struct PlayerInfo {
        sf::Vector2f position;
        sf::Vector2f size;
        Model::Entities::PlayerState state;
    } player_info;

    typedef struct {
        sf::Vector2f position;
        Model::Entities::PlatformType type;
        bool rolling_direction; // 滚动方向（true=右，false=左）
    } PlatformInfo;
    std::map<int, PlatformInfo> platforms_info;
    std::vector<int> platforms_id;
    
    // 敌人信息 (Model层内部使用)
    typedef struct {
        sf::Vector2f position;
        sf::Vector2f size;
        Model::Entities::EnemyType type;
        sf::Vector2f facing_direction;  // 面向方向
    } EnemyInfo;
    std::map<int, EnemyInfo> enemies_info;
    std::vector<int> enemies_id;
    
    // 子弹信息 (Model层内部使用)
    typedef struct {
        sf::Vector2f position;
        sf::Vector2f size;
        bool is_player_bullet;  // 是否为玩家箭矢
        sf::Vector2f velocity;  // 子弹速度方向
    } BulletInfo;
    std::map<int, BulletInfo> bullets_info;
    std::vector<int> bullets_id;
    
    // 豆子信息 (Model层内部使用)
    typedef struct {
        sf::Vector2f position;
        sf::Vector2f size;
        Model::Entities::PickupType type;
    } PickupInfo;
    std::map<int, PickupInfo> pickups_info;
    std::vector<int> pickups_id;
    
    // 技能信息 (Model层内部使用)
    typedef struct {
        int skill_type;           // 技能类型 0=箭矢射击, 1=冲刺
        float cooldown_progress;  // 冷却进度 0.0=可用, 1.0=完全冷却
        bool is_available;        // 是否可用
    } SkillInfo;
    std::vector<SkillInfo> skills_info; // 按技能ID顺序
} _FrameInfo;

// 使用与NotificationBase.h相同的模板类，确保兼容性
typedef Common::NotificationTypeParam<_FrameInfo> _ChangeGameFrameParam;

}
