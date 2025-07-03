#include "Model/Entities/Event.h"
#include "Common/Config/GameConfig.h"
#include <iostream>

namespace Model {
namespace Entities {

// ====================================
// Event类实现
// ====================================
Event::Event(float trigger_time, const std::string& description, std::function<void()> effect_function)
    : trigger_time(trigger_time), description(description), triggered(false), effect_function(effect_function) {
}

void Event::update(float current_game_time) {
    if (!triggered && current_game_time >= trigger_time) {
        triggerEffect();
        triggered = true;
        std::cout << "[Game Event] Time: " << (int)current_game_time << "s - " << description << std::endl;
    }
}

void Event::triggerEffect() {
    if (effect_function) {
        effect_function();
    }
}


}
}
