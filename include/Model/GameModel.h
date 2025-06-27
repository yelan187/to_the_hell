#pragma once
#include "Model/Model.h"
#include "Model/Entities/Player.h"
#include "Model/Entities/Platform.h"
#include <chrono>

namespace Model {
class GameModel : public Model {
public:
    GameModel(Core::Engine &engine);
    GameModel(Core::Engine &engine, sf::Vector2f window_size);
    Entities::PlayerState getPlayerState() {};
    int getTotalScore() const { return total_score; }
    std::chrono::seconds getDuration() {};
    sf::Vector2f getPlayerPosition() const { return player->getPosition(); }
    void update(float delta_time) override;

    void playerJump() {};
    void playerDown() {};
    void playerWalkLeft() {};
    void playerWalkRight() {};
    void playerStopLeft() {};
    void playerStopRight() {};
    Entities::Platform* getPlatformById(int id) const { return platforms.at(id); }
    std::map<int, Entities::Platform*> getPlatforms() const { return platforms; }
    
private:
    bool init;
    
    int total_score;
    std::chrono::seconds game_time;
    float scroll_speed;

    std::map<int, Entities::Platform*> platforms;
    int next_platform_id;
    float platform_generate_interval;
    
    Entities::Player* player;

    void resetPlatformGenerateInterval();
    void generatePlatform();
    Entities::PlatformType GameModel::getPlatformTypeRand();
    void initPlatforms();
    void initPlayer();
    void initGame();

};
}