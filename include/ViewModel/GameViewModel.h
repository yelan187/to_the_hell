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
    
    namespace PlayerState {
        enum class State;
    }
    
    namespace PlatformType {
        enum class Type;
    }
}
}

namespace ViewModel {

class GameViewModel : public ViewModel {
public:

    GameViewModel(Core::Engine& engine);
    
    void update() { model->update(); }

    std::string getTotalScore();
    std::string getGameTime();

    View::UI::PlayerState::State getPlayerState();
    sf::Vector2f getPlayerPosition();

    void playerJump();
    void playerDown();
    void playerWalkLeft();
    void playerWalkRight();
    void playerStopLeft();
    void playerStopRight();

    std::vector<int> getPlatformsId();
    View::UI::PlatformType::Type getPlatformTypeById(int id);
    sf::Vector2f getPlatformPositionById(int id);    

private:
    std::shared_ptr<Model::GameModel> model;
    std::chrono::time_point<std::chrono::steady_clock> start_time;
    
};

}