#pragma once

#include <ViewModel/ViewModel.h>
#include <Model/GameModel.h>
#include <functional>
#include <vector>
#include <string>

namespace View{
namespace UI{
    class Player;
    class Platform;
}
}

namespace ViewModel {

class GameViewModel : public ViewModel {
public:

    GameViewModel(Core::Engine& engine);
    
    void update() { model->update(); }

    std::string getTotalScore();
    std::string getGameTime();

    View::UI::Player::State getPlayerState();
    sf::Vector2f getPlayerPosition();

    void playerJump();
    void playerDown();
    void playerWalkLeft();
    void playerWalkRight();
    void playerStopLeft();
    void playerStopRight();

    std::vector<int> getPlatformsId();
    View::UI::Platform::Type getPlatformTypeById(int id);
    sf::Vector2f getPlatformPositionById(int id);    
    bool platformExists(int id);

private:
    std::shared_ptr<Model::MainMenuModel> model;
    
};

}