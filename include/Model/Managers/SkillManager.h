#pragma once
#include <SFML/Graphics.hpp>

namespace Model {
class GameModel; // 前向声明

namespace Entities {
    class Skill;
    class Player;
    enum class SkillType;
    enum class FacingDirection;
}

namespace Managers {

class EntityManager; // 前向声明

class SkillManager {
public:
    SkillManager(EntityManager* entity_manager, GameModel* game_model);
    ~SkillManager();
    
    // 初始化技能
    void initializeSkills();
    
    // 更新技能冷却
    void updateSkills(float delta_time);
    
    // 使用技能
    bool useSkill(int skill_index, Entities::Player* player, sf::Vector2u window_size);
    
    // 获取技能列表
    std::vector<Entities::Skill*>& getSkills();
    
private:
    EntityManager* entity_manager;
    GameModel* game_model;
    
    // 技能效果实现
    void executeArrowShot(Entities::Player* player);
    void executeSprint(Entities::Player* player, sf::Vector2u window_size);
};

} // namespace Managers
} // namespace Model
