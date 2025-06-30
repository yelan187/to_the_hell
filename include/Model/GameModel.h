#pragma once
#include "Model/Model.h"
#include "Model/Entities/Player.h"
#include "Model/Managers/EntityManager.h"
#include "Model/Managers/GeneratorManager.h"
#include "Model/Managers/CollisionManager.h"
#include "Model/Managers/SkillManager.h"
#include "Model/GameConfig.h"
#include <chrono>
#include <memory>

namespace Model {
class GameModel : public Model {
public:
    GameModel(Core::Engine &engine, sf::Vector2u window_size);
    ~GameModel();
    Entities::PlayerState getPlayerState() { return player->getState(); }
    int getTotalScore() const { return total_score; }
    bool isPlayerDead() const { return player->isDead(); } // 新增：检查玩家是否死亡

    std::string getDebugInfo() {
        std::string debug_info;
        if (player) {
            debug_info += "on_platform: " + std::to_string(player->getOnPlatformId()) + "\n";
        } else {
            debug_info += "Player not initialized\n";
        }
        return debug_info;
    }
    std::chrono::seconds getDuration() { return std::chrono::seconds(static_cast<int>(game_time)); }
    sf::Vector2f getPlayerPosition() const { return player->getPosition(); }
    Entities::FacingDirection getPlayerFacingDirection() const { return player->getFacingDirection(); }

    void update(float delta_time) override;

    void playerJump() {
        player->jump(scroll_speed);
    }
    void playerDown() {
        player->fall();
    }
    void playerWalkLeft() {
        player->walkLeft();
    }
    void playerWalkRight() {
        player->walkRight();
    }
    void playerStopLeft() {
        player->stopLeft();
    }
    void playerStopRight() {
        player->stopRight();
    }
    Entities::Platform* getPlatformById(int id) const { return entity_manager->getPlatformById(id); }
    std::map<int, Entities::Platform*> getPlatforms() const { return entity_manager->getPlatforms(); }
    
    // 敌人和子弹相关方法
    std::map<int, Entities::Enemy*> getEnemies() const { return entity_manager->getEnemies(); }
    std::map<int, Entities::Bullet*> getBullets() const { return entity_manager->getBullets(); }
    std::map<int, Entities::Pickup*> getPickups() const { return entity_manager->getPickups(); }
    void createBullet(sf::Vector2f position, sf::Vector2f velocity) {
        entity_manager->addBullet(position, velocity, GameConfig::BULLET_SIZE);
    }
    
    // 技能和箭矢相关方法
    std::map<int, Entities::Arrow*> getArrows() const { return entity_manager->getArrows(); }
    std::vector<Entities::Skill*> getSkills() const { return skill_manager->getSkills(); }
    void useSkill(int skill_index) { skill_manager->useSkill(skill_index, player, window_size); }
    float getScrollSpeed() const { return scroll_speed; }
    
private:
    bool init;
    
    int total_score;
    float game_time;
    float scroll_speed;
    
    // 管理器
    std::unique_ptr<Managers::EntityManager> entity_manager;
    std::unique_ptr<Managers::GeneratorManager> generator_manager;
    std::unique_ptr<Managers::CollisionManager> collision_manager;
    std::unique_ptr<Managers::SkillManager> skill_manager;
    
    Entities::Player* player;

    void initPlayer();
    void initGame();
};
}