#pragma once
#include "Model/Model.h"
#include "Model/Entities/Player.h"
#include "Model/Entities/Platform.h"
#include "Model/Entities/Enemy.h"
#include "Model/Entities/Bullet.h"
#include <chrono>

namespace Model {
class GameModel : public Model {
public:
    GameModel(Core::Engine &engine, sf::Vector2u window_size);
    ~GameModel();
    Entities::PlayerState getPlayerState() { return player->getState(); }
    int getTotalScore() const { return total_score; }

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
    Entities::Platform* getPlatformById(int id) const { return platforms.at(id); }
    std::map<int, Entities::Platform*> getPlatforms() const { return platforms; }
    
    // 敌人和子弹相关方法
    std::map<int, Entities::Enemy*> getEnemies() const { return enemies; }
    std::map<int, Entities::Bullet*> getBullets() const { return bullets; }
    void createBullet(sf::Vector2f position, sf::Vector2f velocity);

    sf::Vector2f platform_size = sf::Vector2f(100, 12);
    sf::Vector2f player_size = sf::Vector2f(30, 60);
    sf::Vector2f enemy_size = sf::Vector2f(40, 40);
    sf::Vector2f bullet_size = sf::Vector2f(8, 8);
    
private:
    bool init;
    
    int total_score;
    float game_time;
    float scroll_speed;

    std::map<int, Entities::Platform*> platforms;
    int next_platform_id;
    float platform_generate_interval;
    
    std::map<int, Entities::Enemy*> enemies;
    int next_enemy_id;
    float enemy_generate_interval;
    
    std::map<int, Entities::Bullet*> bullets;
    int next_bullet_id;
    
    Entities::Player* player;

    void resetPlatformGenerateInterval();
    void generatePlatform();
    void generateEnemy();
    void resetEnemyGenerateInterval();
    Entities::PlatformType getPlatformTypeRand();
    void initPlatforms();
    void initPlayer();
    void initGame();
    void checkCollisions();
};
}