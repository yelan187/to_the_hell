#pragma once

#include "Model/Model.h"
#include "Model/Entities/Player.h"
#include "Model/Entities/Platform.h"
#include "Model/Entities/Enemy.h"
#include "Model/Entities/Bullet.h"
#include "Model/Entities/Pickup.h"
#include "Model/Entities/Skill.h"
#include <chrono>
#include <map>

// 前向声明
namespace Model {
    namespace Entities {
        enum class PlatformType;
    }
}

namespace Model {
class GameModel : public Model {
public:
    GameModel(sf::Vector2u window_size);
    ~GameModel();
    std::string getDebugInfo() {
        std::string debug_info;
        if (player) {
            debug_info += "on_platform: " + std::to_string(player->getOnPlatformId()) + "\n";
        } else {
            debug_info += "Player not initialized\n";
        }
        return debug_info;
    }
    void gameOver();
    int getTotalScore() { return total_score; }
    std::chrono::seconds getDuration() { return std::chrono::seconds(static_cast<int>(game_time)); }
    std::map<int, Entities::Platform*> getPlatforms() const { return platforms; }
    Entities::Platform* getPlatformById(int id) const { return platforms.at(id); }
    std::map<int, Entities::Enemy*> getEnemies() const { return enemies; }
    void createBullet(sf::Vector2f position, sf::Vector2f velocity, bool is_player_bullet = false);
    std::map<int, Entities::Bullet*> getBullets() const { return bullets; }
    std::map<int, Entities::Pickup*> getPickups() const { return pickups; }
    sf::Vector2f getPlayerPosition() const { return player->getPosition(); }

    void update(float delta_time);
    
    // 技能系统
    std::vector<Entities::Skill*> getSkills() const { return skills; }

    // 玩家控制方法声明
    void playerJump();
    void playerDown();
    void playerWalkLeft();
    void playerWalkRight();
    void playerStopLeft();
    void playerStopRight();
    
    // 技能系统
    void playerUseSkill(int skill_id, sf::Vector2f direction = sf::Vector2f(1.0f, 0.0f));
    
    sf::Vector2f platform_size = Utils::PLATFORM_SIZE;
    sf::Vector2f player_size = sf::Vector2f(30, 60);
    
private:
    bool init;
    int next_platform_id = 0;
    int next_enemy_id = 0;
    int next_bullet_id = 0;
    int next_pickup_id = 0;
    
    int total_score;
    float game_time;
    float scroll_speed;
    float platform_generate_interval;
    float enemy_generate_interval;
    float pickup_generate_interval;

    Entities::Player* player;
    std::map<int, Entities::Platform*> platforms;
    std::map<int, Entities::Enemy*> enemies;
    std::map<int, Entities::Bullet*> bullets;
    std::map<int, Entities::Pickup*> pickups;
    
    // 技能系统
    std::vector<Entities::Skill*> skills;

    void initPlatforms();
    void initPlayer();
    void initGame();
    void initSkills();  // 新增：初始化技能
    void generatePlatform();
    void generateEnemy();
    void generatePickup();
    bool checkBulletPlayerCollisions();
    void checkPlayerBulletEnemyCollisions();
    int checkPickupPlayerCollisions();
    void cleanupOutOfBoundsEntities();
    void fire();
    
    // 新增：缺失的方法声明
    Entities::PlatformType getPlatformTypeRand();
    void resetPlatformGenerateInterval();
};
}