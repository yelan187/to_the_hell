#include <iostream>
#include "ViewModel/GameViewModel.h"
#include "Model/GameModel.h"

using ViewModel::GameViewModel;

//     View::UI::PlayerState::State getPlayerState();
//     sf::Vector2f getPlayerPosition();

//     void playerJump();
//     void playerDown();
//     void playerWalkLeft();
//     void playerWalkRight();
//     void playerStopLeft();
//     void playerStopRight();

//     std::vector<int> getPlatformsId();
//     View::UI::PlatformType::Type getPlatformTypeById(int id);
//     sf::Vector2f getPlatformPositionById(int id);    

GameViewModel::GameViewModel(Core::Engine& engine) : ViewModel(engine) {
    model = std::make_shared<Model::GameModel>(engine);
    start_time = std::chrono::steady_clock::now();
}

std::string GameViewModel::getGameTime() {
    auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - start_time);
    int seconds = duration.count() % 60;
    int minutes = (duration.count() / 60) % 60;
    int hours = duration.count() / 3600;

    std::stringstream ss;
    ss << "Time: ";
    if (hours > 0) {
        ss << std::setw(2) << std::setfill('0') << hours << ":";
    }
    ss << std::setw(2) << std::setfill('0') << minutes << ":"
       << std::setw(2) << std::setfill('0') << seconds;
    
    return ss.str();
}

std::string GameViewModel::getTotalScore() {
    return "Score: " + std::to_string(model->getTotalScore());
}

View::UI::PlayerState::State GameViewModel::getPlayerState() {
    
}
