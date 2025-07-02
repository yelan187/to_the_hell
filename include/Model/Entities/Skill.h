#pragma once

#include <SFML/Graphics.hpp>

namespace Model {
namespace Entities {

// 技能类型枚举
enum class SkillType {
    ARROW_SHOT,  // 箭矢射击
    SPRINT,      // 冲刺
    GROUND_PENETRATION
};

// 技能类 - 管理技能冷却和可用性
class Skill {
public:
    Skill(SkillType type, float cooldown_time);
    
    void update(float delta_time);  // 更新冷却时间
    bool canUse() const;           // 检查是否可以使用
    void use();                    // 使用技能（启动冷却）
    float getCooldownProgress() const; // 返回0.0-1.0的冷却进度
    
    // Getters
    SkillType getType() const { return type; }
    bool isAvailable() const { return is_available; }
    float getCooldownTime() const { return cooldown_time; }
    float getCurrentCooldown() const { return current_cooldown; }

private:
    SkillType type;
    float cooldown_time;
    float current_cooldown;
    bool is_available;
};

}
}
