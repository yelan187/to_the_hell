#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Common/ParamBase.h"
#include "Utils/Config.h"
namespace Common {

enum class NotificationId {
    // mainmenu
    ChangeCurrentSelection,
    ChangeBackgroundParticles,
    // game
    ChangePlatformsId,
    ChangeGameFrame
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
typedef NotificationTypeParam<std::vector<int>> ChangePlatformsIdParam;
typedef struct {
    std::string total_score_text;
    std::string game_time_text;
    std::string debug_info_text;
    
    struct PlayerInfo {
        sf::Vector2f position;
        sf::Vector2f size;
        Utils::GameViewModelPlayerState state;
    } player_info;

    typedef struct {
        sf::Vector2f position;
        Utils::PlatformType type;
    } PlatformInfo;
    std::map<int, PlatformInfo> platforms_info;
} FrameInfo;
typedef NotificationTypeParam<FrameInfo> ChangeGameFrameParam;
}
