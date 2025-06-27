#pragma once

#include <ViewModel/ViewModel.h>
#include <Model/GameModel.h>
#include <functional>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>

namespace View{
namespace UI{
    class Player;
    class Platform;
    
    enum class PlayerState;
    enum class PlatformType;
}
}

namespace ViewModel {

enum class towards {
    LEFT,
    RIGHT
};
class GameViewModel : public ViewModel {
public:
    GameViewModel(Core::Engine& engine);
    GameViewModel(Core::Engine& engine, sf::Vector2u windowSize);
    
    void update(float delta_time) { model->update(delta_time); }

    std::string getTotalScore();
    std::string getGameTime();

    View::UI::PlayerState getPlayerState();
    sf::Vector2f getPlayerPosition();

    void playerJump();
    void playerDown();
    void playerWalkLeft();
    void playerWalkRight();
    void playerStopLeft();
    void playerStopRight();

    std::vector<int> getPlatformsId();
    View::UI::PlatformType getPlatformTypeById(int id);
    sf::Vector2f getPlatformPositionById(int id);    

private:
    std::shared_ptr<Model::GameModel> model;
    towards player_towards = towards::RIGHT;
    
};

}