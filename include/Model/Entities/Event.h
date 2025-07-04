#pragma once
#include <string>
#include <functional>

namespace Model {
    class GameModel;
}

namespace Model {
namespace Entities {

// ====================================
// 事件类型枚举
// ====================================
enum class EventType {
    GAME_CHANGE              // 游戏变化事件
};

// ====================================
// 触发类型枚举
// ====================================
enum class TriggerType {
    TIME_BASED,              // 基于时间触发
    SCORE_BASED              // 基于分数触发
};

// ====================================
// 游戏事件实体类
// ====================================
class Event {
public:
    // 基于时间的事件构造函数
    Event(float trigger_time, const std::string& description, std::function<void()> effect_function);
    
    // 基于分数的事件构造函数
    Event(int trigger_score, const std::string& description, std::function<void()> effect_function);
    
    ~Event() = default;
    
    // 更新事件状态
    void update(float current_game_time, int current_score);
    
    // 获取事件信息
    float getTriggerTime() const { return trigger_time; }
    int getTriggerScore() const { return trigger_score; }
    TriggerType getTriggerType() const { return trigger_type; }
    const std::string& getDescription() const { return description; }
    bool isTriggered() const { return triggered; }

private:
    TriggerType trigger_type;     // 触发类型
    float trigger_time;           // 触发时间（游戏时间，秒）
    int trigger_score;            // 触发分数
    std::string description;      // 事件描述
    bool triggered;              // 是否已触发
    
    // 事件效果函数
    std::function<void()> effect_function;
    
    // 触发事件效果
    void triggerEffect();
    
    // 禁用默认构造函数
    Event() = default;
};

}
}
