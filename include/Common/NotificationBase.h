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
    }
}

namespace Common {

enum class NotificationId {
    // mainmenu
    ChangeCurrentSelection,
    ChangeBackgroundParticles,
    // game
    ChangeGameFrame,
    _ChangeGameFrame,   // model -> viewmodel
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
    } PlatformInfo;
    std::map<int, PlatformInfo> platforms_info;
    std::vector<int> platforms_id;
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
    } PlatformInfo;
    std::map<int, PlatformInfo> platforms_info;
    std::vector<int> platforms_id;
} _FrameInfo;
typedef NotificationTypeParam<_FrameInfo> _ChangeGameFrameParam;

typedef struct gameOver {
    int total_score;
    std::chrono::seconds game_time;
} GameOver;
typedef NotificationTypeParam<GameOver> GameOverNotificationParam;
}
