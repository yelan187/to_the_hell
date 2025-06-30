#include "Model/Managers/SkillManager.h"
#include "Model/Managers/EntityManager.h"
#include "Model/Entities/Skill.h"
#include "Model/Entities/Player.h"
#include "Model/Entities/Platform.h"
#include "Model/GameConfig.h"
#include <SFML/Graphics.hpp>

using namespace Model::Managers;
using namespace Model::Entities;

SkillManager::SkillManager(EntityManager* entity_manager, Model::GameModel* game_model)
    : entity_manager(entity_manager), game_model(game_model) {
}

SkillManager::~SkillManager() {
    // EntityManager会负责清理技能，这里不需要额外操作
}

void SkillManager::initializeSkills() {
    entity_manager->initSkills();
}

void SkillManager::updateSkills(float delta_time) {
    auto& skills = entity_manager->getSkills();
    for (auto* skill : skills) {
        skill->update(delta_time);
    }
}

bool SkillManager::useSkill(int skill_index, Player* player, sf::Vector2u window_size) {
    auto& skills = entity_manager->getSkills();
    
    if (skill_index >= 0 && skill_index < skills.size()) {
        Skill* skill = skills[skill_index];
        if (skill->canUse()) {
            skill->use();
            
            // 根据技能类型执行对应效果
            switch (skill->getType()) {
                case SkillType::ARROW_SHOT:
                    executeArrowShot(player);
                    return true;
                case SkillType::SPRINT:
                    executeSprint(player, window_size);
                    return true;
            }
        }
    }
    return false;
}

std::vector<Skill*>& SkillManager::getSkills() {
    return entity_manager->getSkills();
}

void SkillManager::executeArrowShot(Player* player) {
    sf::Vector2f player_pos = player->getPosition();
    sf::Vector2f arrow_pos;
    sf::Vector2f arrow_velocity;
    
    // 根据玩家朝向决定箭矢发射方向
    if (player->getFacingDirection() == FacingDirection::RIGHT) {
        arrow_pos = sf::Vector2f(
            player_pos.x + Model::GameConfig::PLAYER_SIZE.x,
            player_pos.y + Model::GameConfig::PLAYER_SIZE.y / 2 - Model::GameConfig::ARROW_SIZE.y / 2
        );
        arrow_velocity = sf::Vector2f(Model::GameConfig::ARROW_SPEED, 0.0f); // 水平向右
    } else {
        arrow_pos = sf::Vector2f(
            player_pos.x - Model::GameConfig::ARROW_SIZE.x,
            player_pos.y + Model::GameConfig::PLAYER_SIZE.y / 2 - Model::GameConfig::ARROW_SIZE.y / 2
        );
        arrow_velocity = sf::Vector2f(-Model::GameConfig::ARROW_SPEED, 0.0f); // 水平向左
    }
    
    entity_manager->addArrow(arrow_pos, arrow_velocity, Model::GameConfig::ARROW_SIZE, game_model);
}

void SkillManager::executeSprint(Player* player, sf::Vector2u window_size) {
    sf::Vector2f player_pos = player->getPosition();
    float sprint_distance = Model::GameConfig::SPRINT_DISTANCE; // 冲刺距离150像素
    float direction = (player->getFacingDirection() == FacingDirection::RIGHT) ? 1.0f : -1.0f;
    
    // 获取玩家当前所在的平台ID（如果有的话）
    int current_platform_id = player->getOnPlatformId();
    
    // 分步检测冲刺路径上的碰撞
    float step_size = 5.0f; // 每步5像素
    float current_distance = 0.0f;
    sf::Vector2f final_pos = player_pos;
    
    auto& platforms = entity_manager->getPlatforms();
    
    while (current_distance < sprint_distance) {
        sf::Vector2f test_pos = player_pos + sf::Vector2f(direction * (current_distance + step_size), 0.0f);
        
        // 检查窗口边界
        if (test_pos.x < 0 || test_pos.x + Model::GameConfig::PLAYER_SIZE.x > window_size.x) {
            break;
        }
        
        // 检查与平台的碰撞（忽略当前站立的平台）
        bool collision = false;
        sf::FloatRect player_rect(test_pos.x, test_pos.y, Model::GameConfig::PLAYER_SIZE.x, Model::GameConfig::PLAYER_SIZE.y);
        
        for (const auto& platform_pair : platforms) {
            Platform* platform = platform_pair.second;
            
            // 忽略当前站立的平台，允许在其上移动
            if (current_platform_id != -1 && platform->id == current_platform_id) {
                continue;
            }
            
            sf::Vector2f platform_pos = platform->getPosition();
            sf::Vector2f platform_size_actual = platform->getSize();
            sf::FloatRect platform_rect(platform_pos.x, platform_pos.y, platform_size_actual.x, platform_size_actual.y);
            
            if (player_rect.intersects(platform_rect)) {
                collision = true;
                break;
            }
        }
        
        if (collision) {
            break;
        }
        
        final_pos = test_pos;
        current_distance += step_size;
    }
    
    // 确保最终位置不会超出窗口边界
    if (final_pos.x < 0) {
        final_pos.x = 0;
    } else if (final_pos.x + Model::GameConfig::PLAYER_SIZE.x > window_size.x) {
        final_pos.x = window_size.x - Model::GameConfig::PLAYER_SIZE.x;
    }
    
    if (final_pos.y < 0) {
        final_pos.y = 0;
    } else if (final_pos.y + Model::GameConfig::PLAYER_SIZE.y > window_size.y) {
        final_pos.y = window_size.y - Model::GameConfig::PLAYER_SIZE.y;
    }
    
    player->setPosition(final_pos);
}
