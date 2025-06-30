#pragma once

#include "Common/ParamBase.h"
#include <chrono>
#include <SFML/Graphics.hpp>  // 添加SFML头文件

namespace View {
    enum class PAGE_STATE;
}

namespace Common {

class CommandParam : public Param {
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
    View::PAGE_STATE new_page_state;
    bool init;
} ChangePageParamValue;
typedef CommandTypeParam<ChangePageParamValue> ChangePageParam;

typedef struct {
    int total_score;
    std::chrono::seconds game_time;
} GameOverParamValue;
typedef CommandTypeParam<GameOverParamValue> GameOverCommandParam;

// 玩家技能命令参数
typedef struct {
    int skill_id;      // 技能ID：0=箭矢射击, 1=冲刺
    sf::Vector2f direction;  // 方向参数（用于箭矢射击方向）
} PlayerSkillParamValue;
typedef CommandTypeParam<PlayerSkillParamValue> PlayerSkillParam;

}