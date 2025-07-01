#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <chrono>
#include "Common/ParamBase.h"
#include "Utils/Config.h"

namespace Model {
    namespace Entities {
        enum class PlayerState;
        enum class PlatformType;
        enum class EnemyType;
        enum class PickupType;
    }
}

namespace Common {

enum class NotificationId {
    // 主菜单相关通知
    ChangeCurrentSelection,
    ChangeBackgroundParticles,
    // 游戏相关通知
    ChangeGameFrame,
    _ChangeGameFrame,   // Model层到ViewModel层的内部通知
    GameOver
};

class NotificationParam : public Param {
public:
    NotificationId id;
    virtual ~NotificationParam() = default;
};

typedef void (* NotificationFunc)(NotificationParam*, void* p);

template <typename T>
class NotificationTypeParam : public NotificationParam {
public:
    T value;
    virtual ~NotificationTypeParam() = default;
};

// Add notification param (match with NotificationId)
// mainmenu
typedef NotificationTypeParam<int> ChangeCurrentSelectionParam;
typedef NotificationTypeParam<std::vector<sf::Vector2f>*> ChangeBackgroundParticlesParam;
// game
typedef struct frameInfo{
    std::string total_score_text;
    std::string game_time_text;
    std::string debug_info_text;
    
    struct PlayerInfo {
        sf::Vector2f position;
        sf::Vector2f size;
        sf::Texture* texture;
    } player_info;

    typedef struct {
        sf::Vector2f position;
        sf::Color color;
        sf::Vector2f size;
        int platform_type;      // 平台类型（对应PlatformType枚举）
        bool rolling_direction; // 滚动方向（true=右，false=左）仅滚动平台使用
    } PlatformInfo;
    std::map<int, PlatformInfo> platforms_info;
    std::vector<int> platforms_id;
    
    // 敌人信息
    typedef struct {
        sf::Vector2f position;
        sf::Vector2f size;
        sf::Color color;
        sf::Vector2f facing_direction;  // 面向方向
    } EnemyInfo;
    std::map<int, EnemyInfo> enemies_info;
    std::vector<int> enemies_id;
    
    // 子弹信息
    typedef struct {
        sf::Vector2f position;
        sf::Vector2f size;
        sf::Color color;
        bool is_player_bullet;  // 是否为玩家箭矢
        sf::Vector2f velocity;  // 子弹速度方向
    } BulletInfo;
    std::map<int, BulletInfo> bullets_info;
    std::vector<int> bullets_id;
    
    // 豆子信息
    typedef struct {
        sf::Vector2f position;
        sf::Vector2f size;
        sf::Color color;
        int pickup_type; // 0=圆形豆子, 1=五角星豆子
    } PickupInfo;
    std::map<int, PickupInfo> pickups_info;
    std::vector<int> pickups_id;
    
    // 技能信息
    typedef struct {
        int skill_type;           // 技能类型 0=箭矢射击, 1=冲刺
        float cooldown_progress;  // 冷却进度 0.0=可用, 1.0=完全冷却
        bool is_available;        // 是否可用
    } SkillInfo;
    std::vector<SkillInfo> skills_info; // 按技能ID顺序
} FrameInfo;
typedef NotificationTypeParam<FrameInfo> ChangeGameFrameParam;

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
typedef NotificationTypeParam<_FrameInfo> _ChangeGameFrameParam;

typedef struct gameOver {
    int total_score;
    std::chrono::seconds game_time;
} GameOver;
typedef NotificationTypeParam<GameOver> GameOverNotificationParam;
}
