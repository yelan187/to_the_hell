#pragma once

#include <SFML/Graphics.hpp>
#include "Common/SkillID.h"
#include "Common/Config/Config.h"

namespace Model {
    class GameModel;
    namespace Entities {
        class Player;  // 前向声明Player类
    }
}
#include "Common/SkillID.h"
#include "Common/Config/Config.h"

namespace Model {
    class GameModel;
    namespace Entities {
        class Player;  // 前向声明Player类
    }
}

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
    Skill(Player* player);
    
    Common::SkillID getSkillID() const { return id; }
    void resetCD();
    bool canUse() const;           // 检查是否可以使用
    void use();                    // 使用技能（启动冷却）
    virtual void update(float delta_time);  // 更新冷却时间
    virtual void execute() {}
    float getCooldownProgress() const; // 返回0.0-1.0的冷却进度    
    // Getters
    Common::SkillID getid() const { return id; }
    int getDamage() const {return damage;}
    bool isAvailable() const { return is_available; }
    float getCooldownTime() const { return cooldown_time; }
    float getCurrentCooldown() const { return current_cooldown; }

protected:
    Player* player;
    Common::SkillID id;
    float cooldown_time;
    float current_cooldown;
    bool is_available;
    int level;
    int damage;
};

// 箭矢射击技能
class ArrowShot : public Skill {
public:
    ArrowShot(Player* player) : Skill(player) {
        id = Common::SkillID::ARROW_SHOT;
        cooldown_time = Common::Config::GameConfig::SKILL_ARROW_COOLDOWN;
        damage = Common::Config::GameConfig::SKILL_ARROW_DAMAGE;
    }
    void execute() override;
};

// 冲刺技能
class Sprint : public Skill {
public:
    Sprint(Player* player) : Skill(player) {
        id = Common::SkillID::SPRINT;
        cooldown_time = Common::Config::GameConfig::SKILL_SPRINT_COOLDOWN;
    }
    void execute() override;
    void update(float delta_time) override;
};

// 地面穿透技能
class GroundPenetration : public Skill {
public:
    GroundPenetration(Player* player) : Skill(player) {
        id = Common::SkillID::GROUND_PENETRATION;
        cooldown_time = Common::Config::GameConfig::SKILL_GROUND_PENETRATION_COOLDOWN;
    }
    void execute() override;
};

}
}
