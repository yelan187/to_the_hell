#include "Model/Managers/CollisionManager.h"
#include "Model/Managers/EntityManager.h"
#include "Model/Entities/Player.h"
#include "Model/Entities/Bullet.h"
#include "Model/Entities/Pickup.h"
#include "Model/Entities/Arrow.h"
#include "Model/Entities/Enemy.h"

using namespace Model::Managers;
using namespace Model::Entities;

CollisionManager::CollisionManager(EntityManager* entity_manager)
    : entity_manager(entity_manager) {
}

int CollisionManager::checkAllCollisions(Player* player) {
    int score_gained = 0;
    
    // 检查子弹与玩家的碰撞
    if (checkBulletPlayerCollisions(player)) {
        return -1; // 表示游戏结束
    }
    
    // 检查豆子与玩家的碰撞
    score_gained += checkPickupPlayerCollisions(player);
    
    // 检查箭矢与敌人的碰撞
    score_gained += checkArrowEnemyCollisions();
    
    return score_gained;
}

bool CollisionManager::checkBulletPlayerCollisions(Player* player) {
    auto& bullets = entity_manager->getBullets();
    for (auto& bullet_pair : bullets) {
        Bullet* bullet = bullet_pair.second;
        if (bullet->collidesWith(player->getPosition(), player->getSize())) {
            return true; // 玩家被击中
        }
    }
    return false;
}

int CollisionManager::checkPickupPlayerCollisions(Player* player) {
    int score_gained = 0;
    auto& pickups = entity_manager->getPickups();
    
    for (auto it = pickups.begin(); it != pickups.end(); ) {
        Pickup* pickup = it->second;
        if (pickup->collidesWith(player->getPosition(), player->getSize())) {
            score_gained += pickup->getScore();
            delete pickup;
            it = pickups.erase(it);
        } else {
            ++it;
        }
    }
    
    return score_gained;
}

int CollisionManager::checkArrowEnemyCollisions() {
    int score_gained = 0;
    auto& arrows = entity_manager->getArrows();
    auto& enemies = entity_manager->getEnemies();
    
    for (auto arrow_it = arrows.begin(); arrow_it != arrows.end(); ) {
        Arrow* arrow = arrow_it->second;
        bool arrow_hit = false;
        
        for (auto enemy_it = enemies.begin(); enemy_it != enemies.end(); ) {
            Enemy* enemy = enemy_it->second;
            if (arrow->collidesWith(enemy->getPosition(), enemy->getSize())) {
                score_gained += 5; // 击杀敌人得5分
                delete enemy;
                enemy_it = enemies.erase(enemy_it);
                arrow_hit = true;
                break;
            } else {
                ++enemy_it;
            }
        }
        
        if (arrow_hit) {
            delete arrow;
            arrow_it = arrows.erase(arrow_it);
        } else {
            ++arrow_it;
        }
    }
    
    return score_gained;
}
