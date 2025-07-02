#pragma once

#include "Common/PageState.h"
#include <chrono>
#include <SFML/Graphics.hpp>

namespace Common {

class CommandParam{
public:
    virtual ~CommandParam() = default;
};

class CommandBase {
public:
    virtual void execute() {}
    virtual void execute(CommandParam& params) {}
};

template <typename T>
class CommandTypeParam : public CommandParam {
public:
	T value;
};

// Add command param
typedef CommandTypeParam<float> UpdateParam;

typedef struct {
    Common::PAGE_STATE new_page_state;
    bool init;
} ChangePageParamValue;
typedef CommandTypeParam<ChangePageParamValue> ChangePageParam;

typedef struct {
    std::string total_score;
    std::string game_time;
} GameOverParamValue;
typedef CommandTypeParam<GameOverParamValue> GameOverCommandParam;

// 玩家技能命令参数
typedef struct {
    int skill_id;
    sf::Vector2f direction;
} PlayerSkillParamValue;
typedef CommandTypeParam<PlayerSkillParamValue> PlayerSkillParam;

}