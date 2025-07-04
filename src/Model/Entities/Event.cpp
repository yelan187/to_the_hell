#include "Model/Entities/Event.h"
#include "Common/Config/GameConfig.h"
#include <iostream>

namespace Model {
namespace Entities {

// ====================================
// Event类实现
// ====================================

// 基于时间的事件构造函数
Event::Event(float trigger_time, const std::string& description, std::function<void()> effect_function)
    : trigger_type(TriggerType::TIME_BASED), trigger_time(trigger_time), trigger_score(0), 
      description(description), triggered(false), effect_function(effect_function) {
}

// 基于分数的事件构造函数
Event::Event(int trigger_score, const std::string& description, std::function<void()> effect_function)
    : trigger_type(TriggerType::SCORE_BASED), trigger_time(0.0f), trigger_score(trigger_score),
      description(description), triggered(false), effect_function(effect_function) {
}

void Event::update(float current_game_time, int current_score) {
    if (!triggered) {
        bool should_trigger = false;
        
        if (trigger_type == TriggerType::TIME_BASED && current_game_time >= trigger_time) {
            should_trigger = true;
            std::cout << "[Game Event] Time: " << (int)current_game_time << "s - " << description << std::endl;
        } else if (trigger_type == TriggerType::SCORE_BASED && current_score >= trigger_score) {
            should_trigger = true;
            std::cout << "[Game Event] Score: " << current_score << " - " << description << std::endl;
        }
        
        if (should_trigger) {
            triggerEffect();
            triggered = true;
        }
    }
}

void Event::triggerEffect() {
    if (effect_function) {
        effect_function();
    }
}


}
}
