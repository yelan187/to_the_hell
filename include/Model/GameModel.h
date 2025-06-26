#pragma once
#include "Model/Model.h"
#include "Model/Entities/Player.h"
#include "Model/Entities/Platform.h"
#include <chrono>

namespace Model {
class GameModel : public Model {
public:
    GameModel(Core::Engine &engine) : Model(engine) {};
    
    Entities::PlayerState getPlayerState() {};
    int getTotalScore() const { return total_score; }
    std::chrono::seconds getDuration() {};
    sf::Vector2f getPlayerPosition() const { return player_position; }
    void update() override {};
    void playerJump() {};
    void playerDown() {};
    void playerWalkLeft() {};
    void playerWalkRight() {};
    void playerStopLeft() {};
    void playerStopRight() {};
    Entities::Platform* getPlatformById(int id) const { return platforms.at(id); }
    std::map<int, Entities::Platform*> getPlatforms() const { return platforms; }
    
private:
    int total_score;
    sf::Vector2f player_position;
    std::map<int, Entities::Platform*> platforms;
};
}