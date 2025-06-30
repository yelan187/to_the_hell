#pragma once

#include "Model/Model.h"
#include "Model/Entities/Player.h"
#include "Model/Entities/Platform.h"
#include <chrono>

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
    int getTotalScore() { return total_score; }
    std::chrono::seconds getDuration() { return std::chrono::seconds(static_cast<int>(game_time)); }
    std::map<int, Entities::Platform*> getPlatforms() const { return platforms; }
    Entities::Platform* getPlatformById(int id) const { return platforms.at(id); }

    void update(float delta_time);

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
    sf::Vector2f platform_size = Utils::PLATFORM_SIZE;
    sf::Vector2f player_size = sf::Vector2f(30, 60);
    
private:
    bool init;
    
    int total_score;
    float game_time;
    float scroll_speed;

    std::map<int, Entities::Platform*> platforms;
    int next_platform_id;
    float platform_generate_interval;
    
    Entities::Player* player;

    void resetPlatformGenerateInterval();
    void generatePlatform();
    Entities::PlatformType getPlatformTypeRand();
    void initPlatforms();
    void initPlayer();
    void initGame();
    void fire();
};
}