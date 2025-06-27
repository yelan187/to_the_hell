#pragma once
#include "Model/Model.h"
#include "Model/Entities/Player.h"
#include "Model/Entities/Platform.h"
#include <chrono>

namespace Model {
class GameModel : public Model {
public:
    GameModel(Core::Engine &engine);
    GameModel(Core::Engine &engine, sf::Vector2u window_size);
    Entities::PlayerState getPlayerState() { return Entities::PlayerState::IDLE; };
    int getTotalScore() const { return total_score; }
    std::chrono::seconds getDuration() { return game_time; };
    sf::Vector2f getPlayerPosition() const { return player->getPosition(); }

    void update(float delta_time) override;

    void adjustPlayerPosition();

    void playerJump() {};
    void playerDown() {};
    void playerWalkLeft() {
        player->walkLeft();
    };
    void playerWalkRight() {
        player->walkRight();
    };
    void playerStopLeft() {};
    void playerStopRight() {};
    Entities::Platform* getPlatformById(int id) const { return platforms.at(id); }
    std::map<int, Entities::Platform*> getPlatforms() const { return platforms; }
    
private:
    bool init;
    
    int total_score;
    std::chrono::seconds game_time;
    float scroll_speed;
    sf::Vector2f gravity;

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

};
}