#include "Model/Entities/Skill.h"
#include "Model/Entities/Player.h"
#include "Model/GameModel.h"

using Model::Entities::Skill;
using Model::Entities::Platform;

Skill::Skill(Player* player)
: player(player),current_cooldown(0),is_available(true),level(1)
{}

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
        execute();
    }
}

float Skill::getCooldownProgress() const {
    if (cooldown_time <= 0.0f) return 0.0f;
    return current_cooldown / cooldown_time;
}

// ArrowShot

void Model::Entities::ArrowShot::execute() {
    sf::Vector2f player_size = player->getSize();
    sf::Vector2f player_facing = player->getFacingDirection();
    sf::Vector2f player_pos = player->getPosition();
    sf::Vector2f arrow_pos = sf::Vector2f(
        player_pos.x + player_size.x / 2, 
        player_pos.y + player_size.y / 2 - Common::Config::GameConfig::BULLET_SIZE.y / 2
    );
    sf::Vector2f arrow_velocity = sf::Vector2f(player_facing.x * Common::Config::GameConfig::BULLET_SPEED, 0.0f);
    player->game_model->createBullet(arrow_pos, arrow_velocity, damage, true);

};

// Sprint
void Model::Entities::Sprint::execute() {
    sf::Vector2f player_facing = player->getFacingDirection();
    sf::Vector2f sprint_replacement = sf::Vector2f(
        player_facing.x * Common::Config::GameConfig::SKILL_SPRINT_DISTANCE, 
        0.0f
    );
    player->updatePosition(0.0f, sprint_replacement);
    player->resetKillCount();
}
void Model::Entities::Sprint::update(float delta_time) {
    Skill::update(delta_time);
    if (player->getKillCount() == Common::Config::GameConfig::SKILL_SPRINT_RESET_KILL_COUNT) {
        resetCD();
    }
}

// GroundPenetration
void Model::Entities::GroundPenetration::execute() {
    auto game_model = player->game_model;
    Platform* current_platform = game_model->getPlatformById(player->getOnPlatformId());
    sf::Vector2f p = player->getPosition() + sf::Vector2f(0, player->getSize().y + current_platform->getSize().y + 2);
    for (auto& platform_pair : game_model->getPlatforms()) {
        Platform* platform = platform_pair.second;
        bool res = player->collisionDetection(platform, p);
        if (res) {
            return;
        }
    }
    player->setPosition(p);
    player->setVelocity(
        sf::Vector2f(player->getVelocity().x, current_platform->getVelocity().y)
    );
    player->setOnPlatform(false);
    player->setOnPlatformId(-1);
};