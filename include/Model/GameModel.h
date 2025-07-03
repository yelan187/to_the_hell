#pragma once

#include "Model/Model.h"
#include "Model/Entities/Player.h"
#include "Model/Entities/Platform.h"
#include "Model/Entities/Enemy.h"
#include "Model/Entities/Bullet.h"
#include "Model/Entities/Pickup.h"
#include "Model/Entities/Skill.h"
#include "Common/Config/Config.h"
#include "Common/SkillID.h"
#include <chrono>
#include <map>

// 前向声明
namespace Model {
    namespace Entities {
        enum class PlatformType;
    }
}

namespace Model {
/**
 * GameModel类 - 游戏核心逻辑模型
 * 管理游戏中的所有实体：玩家、平台、敌人、子弹、豆子、技能
 * 处理碰撞检测、实体生成、游戏状态更新等核心功能
 */
class GameModel : public Model {
public:
    GameModel(sf::Vector2u window_size);
    ~GameModel();
    // interact with viewmodel
    std::string getDebugInfo() {
        std::string debug_info;
        if (player) {
            debug_info += "on_platform: " + std::to_string(player->getOnPlatformId()) + "\n";
        } else {
            debug_info += "Player not initialized\n";
        }
        return debug_info;
    }
    void addScore(int score) { total_score += score; }
    int getTotalScore() { return total_score; }
    std::chrono::seconds getDuration() { return std::chrono::seconds(static_cast<int>(game_time)); }
    // platform
    std::map<int, Entities::Platform*> getPlatforms() const { return platforms; }
    Entities::Platform* getPlatformById(int id) const { 
        auto it = platforms.find(id);
        return (it != platforms.end()) ? it->second : nullptr;
    }

    // enemy
    std::map<int, Entities::Enemy*>& getEnemies() { return enemies; }
    // bullet
    void createBullet(sf::Vector2f position, sf::Vector2f velocity, int damage, bool is_player_bullet = false);
    std::map<int, Entities::Bullet*>& getBullets() { return bullets; }
    // pickups
    std::map<int, Entities::Pickup*>& getPickups() { return pickups; }
    // player
    sf::Vector2f getPlayerPosition() const { return player->getPosition(); }
    Entities::Player* getPlayer() const { return player; }
    void playerJump();
    void playerDown();
    void playerWalkLeft();
    void playerWalkRight();
    void playerStopLeft();
    void playerStopRight();
    // skill_id: 0=箭矢射击, 1=冲刺
    void playerUseSkill(Common::SkillID skill_id, sf::Vector2f direction = sf::Vector2f(1.0f, 0.0f));
    // gameover
    void gameOver() {
        trigger.fire(Common::NotificationId::GameOver);
    }
    // others
    void update(float delta_time);
    sf::Vector2f platform_size = Common::Config::GameConfig::PLATFORM_SIZE;
    sf::Vector2f player_size = Common::Config::GameConfig::PLAYER_SIZE;
    
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

    void initPlatforms();
    void initPlayer();
    void initGame();
    void generatePlatform();
    void generateEnemy();
    void generatePickup();
    int checkBulletPlayerCollisions();
    int checkPlayerBulletEnemyCollisions();
    int checkPickupPlayerCollisions();
    void cleanupOutOfBoundsEntities();
    
    // 平台相关方法
    Entities::PlatformType getPlatformTypeRand();
    void resetPlatformGenerateInterval();
    bool isPlatformPositionValid(sf::Vector2f position, sf::Vector2f size); // 检查平台位置是否有效（无碰撞）
};
}