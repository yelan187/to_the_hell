#include "Model/Entities/Skill.h"

using Model::Entities::Skill;

Skill::Skill(SkillType type, float cooldown_time)
    : type(type), cooldown_time(cooldown_time), current_cooldown(0.0f), is_available(true) {
}

void Skill::update(float delta_time) {
    if (current_cooldown > 0.0f) {
        current_cooldown -= delta_time;
        if (current_cooldown <= 0.0f) {
            current_cooldown = 0.0f;
            is_available = true;
        }
    }
}

bool Skill::canUse() const {
    return is_available && current_cooldown <= 0.0f;
}

void Skill::use() {
    if (canUse()) {
        current_cooldown = cooldown_time;
        is_available = false;
    }
}

float Skill::getCooldownProgress() const {
    if (cooldown_time <= 0.0f) return 0.0f;
    return current_cooldown / cooldown_time;
}
