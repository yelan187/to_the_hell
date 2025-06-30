#include "Model/Managers/EntityManager.h"
#include "Model/Entities/Platform.h"
#include "Model/Entities/Enemy.h"
#include "Model/Entities/Bullet.h"
#include "Model/Entities/Pickup.h"
#include "Model/Entities/Arrow.h"
#include "Model/Entities/Skill.h"
#include "Utils/GameConfig.h"
#include <iostream>

using namespace Model::Managers;
using namespace Model::Entities;

EntityManager::EntityManager(sf::Vector2u window_size) 
    : window_size(window_size), next_platform_id(0), next_enemy_id(0), 
      next_bullet_id(0), next_pickup_id(0), next_arrow_id(0) {
}

EntityManager::~EntityManager() {
    clearAll();
}

void EntityManager::updateAll(float delta_time) {
    // 更新平台
    for (auto it = platforms.begin(); it != platforms.end(); ) {
        Platform* platform = it->second;
        platform->update(delta_time);
        if (platform->outOfWindow(window_size) || platform->isBroken()) {
            delete platform;
            it = platforms.erase(it);
        } else {
            ++it;
        }
    }
    
    // 更新敌人
    for (auto it = enemies.begin(); it != enemies.end(); ) {
        Enemy* enemy = it->second;
        enemy->update(delta_time);
        
        // 敌人射击
        if (enemy->canShoot()) {
            enemy->shoot();
        }
        
        if (enemy->outOfWindow(window_size)) {
            delete enemy;
            it = enemies.erase(it);
        } else {
            ++it;
        }
    }
    
    // 更新子弹
    for (auto it = bullets.begin(); it != bullets.end(); ) {
        Bullet* bullet = it->second;
        bullet->update(delta_time);
        if (bullet->outOfWindow(window_size)) {
            delete bullet;
            it = bullets.erase(it);
        } else {
            ++it;
        }
    }
    
    // 更新豆子
    for (auto it = pickups.begin(); it != pickups.end(); ) {
        Pickup* pickup = it->second;
        pickup->update(delta_time);
        if (pickup->outOfWindow(window_size)) {
            delete pickup;
            it = pickups.erase(it);
        } else {
            ++it;
        }
    }
    
    // 更新箭矢
    for (auto it = arrows.begin(); it != arrows.end(); ) {
        Arrow* arrow = it->second;
        arrow->update(delta_time);
        if (arrow->outOfWindow(window_size) || arrow->isExpired()) {
            delete arrow;
            it = arrows.erase(it);
        } else {
            ++it;
        }
    }
    
    // 技能更新由 SkillManager 处理，不在这里更新
}

void EntityManager::addPlatform(PlatformType type, sf::Vector2f position, sf::Vector2f size, float scroll_speed) {
    platforms[next_platform_id] = new Platform(next_platform_id, type, position, size, scroll_speed);
    next_platform_id++;
}

Platform* EntityManager::getPlatformById(int id) const {
    auto it = platforms.find(id);
    return (it != platforms.end()) ? it->second : nullptr;
}

void EntityManager::addEnemy(EnemyType type, sf::Vector2f position, sf::Vector2f size, Model::GameModel* game_model) {
    enemies[next_enemy_id] = new Enemy(next_enemy_id, type, position, size, game_model);
    next_enemy_id++;
}

void EntityManager::addBullet(sf::Vector2f position, sf::Vector2f velocity, sf::Vector2f size) {
    bullets[next_bullet_id] = new Bullet(next_bullet_id, position, velocity, size);
    next_bullet_id++;
}

void EntityManager::addPickup(PickupType type, sf::Vector2f position, sf::Vector2f size, Model::GameModel* game_model, int target_platform_id) {
    pickups[next_pickup_id] = new Pickup(next_pickup_id, type, position, size, game_model, target_platform_id);
    next_pickup_id++;
}

void EntityManager::addArrow(sf::Vector2f position, sf::Vector2f velocity, sf::Vector2f size, Model::GameModel* game_model) {
    arrows[next_arrow_id] = new Arrow(next_arrow_id, position, velocity, size, game_model);
    next_arrow_id++;
}

void EntityManager::initSkills() {
    auto& config = Utils::GameConfig::getInstance();
    // 初始化箭矢技能，冷却时间3秒
    skills.push_back(new Skill(SkillType::ARROW_SHOT, config.ARROW_SKILL_COOLDOWN));
    // 初始化瞬移技能，冷却时间5秒
    skills.push_back(new Skill(SkillType::SPRINT, config.SPRINT_SKILL_COOLDOWN));
}

void EntityManager::clearAll() {
    // 清理平台
    for (auto& pair : platforms) {
        if (pair.second) {
            delete pair.second;
        }
    }
    platforms.clear();
    
    // 清理敌人
    for (auto& pair : enemies) {
        if (pair.second) {
            delete pair.second;
        }
    }
    enemies.clear();
    
    // 清理子弹
    for (auto& pair : bullets) {
        if (pair.second) {
            delete pair.second;
        }
    }
    bullets.clear();
    
    // 清理豆子
    for (auto& pair : pickups) {
        if (pair.second) {
            delete pair.second;
        }
    }
    pickups.clear();
    
    // 清理箭矢
    for (auto& pair : arrows) {
        if (pair.second) {
            delete pair.second;
        }
    }
    arrows.clear();
    
    // 清理技能
    for (auto* skill : skills) {
        if (skill) {
            delete skill;
        }
    }
    skills.clear();
}
