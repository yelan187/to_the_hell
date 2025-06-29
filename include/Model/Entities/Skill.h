#pragma once

#include <SFML/Graphics.hpp>

namespace Model {
    namespace Entities {
        enum class SkillType {
            ARROW_SHOT,
            SPRINT
        };
        
        class Skill {
        private:
            SkillType type;
            float cooldown_time;
            float current_cooldown;
            bool is_available;
            
        public:
            Skill(SkillType type, float cooldown_time);
            
            void update(float delta_time);
            bool canUse() const;
            void use();
            float getCooldownProgress() const; // 返回0.0-1.0的冷却进度
            
            // Getters
            SkillType getType() const { return type; }
            bool isAvailable() const { return is_available; }
            float getCooldownTime() const { return cooldown_time; }
        };
    }
}
