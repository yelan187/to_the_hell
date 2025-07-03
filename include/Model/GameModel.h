#pragma once

#include "Model/Model.h"
#include "Model/Entities/Player.h"
#include "Model/Entities/Platform.h"
#include "Model/Entities/Enemy.h"
#include "Model/Entities/Bullet.h"
#include "Model/Entities/Pickup.h"
#include "Model/Entities/Skill.h"
#include "Model/Entities/Event.h"
#include "Common/Config/Config.h"
#include "Common/SkillID.h"
#include <SFML/Audio.hpp>
#include <chrono>
#include <map>


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
    std::string getPlatformInfo() {
        std::string platform_info;
        if (player) {
            platform_info += "on_platform: " + std::to_string(player->getOnPlatformId()) + "\n";
        } else {
            platform_info += "Player not initialized\n";
        }
        return platform_info;
    }

    void addScore(int increment) { total_score += increment; }
    int getTotalScore() { return total_score; }
    std::chrono::seconds getDuration() { return std::chrono::seconds(static_cast<int>(game_time)); }
    
    
    // platform
    std::map<int, Entities::Platform*> getPlatforms() const { return platforms; }
    Entities::Platform* getPlatformById(int id) const { 
        auto it = platforms.find(id);
        return (it != platforms.end()) ? it->second : nullptr;
    }

    // enemy
    std::map<int, Entities::Enemy*> getEnemies() const { return enemies; }
    Entities::Enemy* getEnemyById(int id) const {
        auto it = enemies.find(id);
        return (it != enemies.end()) ? it->second : nullptr;
    }
    
    // bullet
    void createBullet(sf::Vector2f position, sf::Vector2f velocity, int damage, bool is_player_bullet = false);
    std::map<int, Entities::Bullet*> getBullets() const { return bullets; }
    Entities::Bullet* getBulletById(int id) const {
        auto it = bullets.find(id);
        return (it != bullets.end()) ? it->second : nullptr;
    }
    void removeBullet(int id);
    void removePickup(int id);
    
    // pickups
    std::map<int, Entities::Pickup*> getPickups() const { return pickups; }
    Entities::Pickup* getPickupById(int id) const {
        auto it = pickups.find(id);
        return (it != pickups.end()) ? it->second : nullptr;
    }
    void handlePickup(int pickup_id);  // 处理拾取物逻辑
    // player
    sf::Vector2f getPlayerPosition() const { return player->getPosition(); }
    Entities::Player* getPlayer() const { return player; }
    void playerJump();
    void playerDown();
    void playerWalkLeft();
    void playerWalkRight();
    void playerStopLeft();
    void playerStopRight();
    // skills
    void playerUseSkill(Common::SkillID skill_id);
    
    // background
    std::string getCurrentBackground() const { return current_background; }
    void setBackground(const std::string& background_file);
    bool isBackgroundChanged() const { return background_changed; }
    void markBackgroundAsLoaded() { background_changed = false; }
    
    // gameover
    void gameOver() {
        stopBackgroundMusic();  // 停止背景音乐
        trigger.fire(Common::NotificationId::GameOver);
    }
    // audio
    void startBackgroundMusic();
    void stopBackgroundMusic();
    bool isBackgroundMusicPlaying() const;
    // others
    void update(float delta_time);
    
private:
    bool init;
    int next_platform_id = 0;
    int next_enemy_id = 0;
    int next_bullet_id = 0;
    int next_pickup_id = 0;
    
    int total_score;
    float game_time;
    float platform_generate_interval;
    float enemy_generate_interval;
    float pickup_generate_interval;

    Entities::Player* player;
    std::map<int, Entities::Platform*> platforms;
    std::map<int, Entities::Enemy*> enemies;
    std::map<int, Entities::Bullet*> bullets;
    std::map<int, Entities::Pickup*> pickups;
    
    // 事件系统
    std::vector<Entities::Event*> events;
    int next_event_id = 0;
    
    // 音频系统
    sf::Music background_music;
    
    // 背景系统
    std::string current_background;
    bool background_changed;

    void initPlatforms();
    void initPlayer();
    void initGame();
    void initSkills();  // 初始化技能
    void initEvents();  // 初始化事件系统
    void generatePlatform();
    void generateEnemy();
    void generatePickup();
    void checkPlayerBulletsHitEnemies(); // 检测玩家子弹击中敌人
    void cleanupOutOfBoundsEntities();
    
    // 平台相关方法
    Entities::PlatformType getPlatformTypeRand();
    void resetPlatformGenerateInterval();
    bool isPlatformPositionValid(sf::Vector2f position, sf::Vector2f size); // 检查平台位置是否有效（无碰撞）
};
}