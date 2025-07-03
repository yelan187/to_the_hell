#include "Model/Entities/Skill.h"
#include "Model/Entities/Player.h"
#include "Model/GameModel.h"
#include "Model/Entities/Player.h"
#include "Model/GameModel.h"

using Model::Entities::Skill;
using Model::Entities::ArrowShot;
using Model::Entities::Sprint;
using Model::Entities::GroundPenetration;

// === Skill 基类实现 ===
Skill::Skill(Player* player)
    : player(player), id(Common::SkillID::ARROW_SHOT), cooldown_time(1.0f), 
      current_cooldown(0.0f), is_available(true), level(1), damage(0) {
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

void Skill::resetCD() {
    current_cooldown = 0.0f;
    is_available = true;
}

void Skill::use() {
    if (canUse()) {
        current_cooldown = cooldown_time;
        is_available = false;
        execute(); // 执行技能效果
    }
}

float Skill::getCooldownProgress() const {
    if (cooldown_time <= 0.0f) return 0.0f;
    return current_cooldown / cooldown_time;
}

// === ArrowShot 实现 ===
void ArrowShot::execute() {
    // 创建玩家子弹
    sf::Vector2f bullet_position = player->getPosition() + sf::Vector2f(player->getSize().x / 2, player->getSize().y / 2);
    sf::Vector2f bullet_velocity = player->getFacingDirection() * Common::Config::GameConfig::SKILL_ARROW_SPEED;
    player->game_model->createBullet(bullet_position, bullet_velocity, damage, true);
}

// === Sprint 实现 ===
void Sprint::execute() {
    // 冲刺逻辑 - 瞬间位移
    sf::Vector2f sprint_displacement = player->getFacingDirection() * Common::Config::GameConfig::SKILL_SPRINT_DISTANCE;
    player->updatePosition(0.0f, sprint_displacement); // 使用additional_replacement参数进行瞬间位移
    player->resetKillCount(); // 冲刺后重置击杀计数
}

void Sprint::update(float delta_time) {
    Skill::update(delta_time); // 调用基类更新
    // 冲刺技能重置逻辑
    if (player->getKillCount() == Common::Config::GameConfig::SKILL_SPRINT_RESET_KILL_COUNT) {
        auto& skills = player->getSkills();
        auto sprint_it = skills.find(Common::SkillID::SPRINT);
        if (sprint_it != skills.end()) {
            sprint_it->second->resetCD();
        }
    }
}

// === GroundPenetration 实现 ===
void GroundPenetration::execute() {
    // 地面穿透逻辑
    player->groundPenetration();
}
