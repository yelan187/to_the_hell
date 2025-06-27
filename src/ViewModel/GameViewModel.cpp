#include <iostream>
#include "ViewModel/GameViewModel.h"
#include "Model/GameModel.h"
#include "View/GameView.h"

using ViewModel::GameViewModel;

GameViewModel::GameViewModel(Core::Engine& engine) : ViewModel(engine) {
    model = std::make_shared<Model::GameModel>(engine);
}

GameViewModel::GameViewModel(Core::Engine& engine, sf::Vector2u window_size) : ViewModel(engine) {
    model = std::make_shared<Model::GameModel>(engine, window_size);
}

std::string GameViewModel::getGameTime() {
    std::chrono::seconds duration = model->getDuration();
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

View::UI::PlayerState GameViewModel::getPlayerState() {
    Model::Entities::PlayerState player_state = model->getPlayerState();
    switch (player_state) {
        case Model::Entities::PlayerState::IDLE:
            if (player_towards == towards::LEFT) {
                return View::UI::PlayerState::IDLE_L;
            } else {
                return View::UI::PlayerState::IDLE_R;
            }
        case Model::Entities::PlayerState::WALKING:
            if (player_towards == towards::LEFT) {
                return View::UI::PlayerState::WALKING_L;
            } else {
                return View::UI::PlayerState::WALKING_R;
            }
        case Model::Entities::PlayerState::JUMPING:
            if (player_towards == towards::LEFT) {
                return View::UI::PlayerState::JUMPING_L;
            } else {
                return View::UI::PlayerState::JUMPING_R;
            }
    }
}

sf::Vector2f GameViewModel::getPlayerPosition() {
    return model->getPlayerPosition();
}

void GameViewModel::playerJump() {
    model->playerJump();
}

void GameViewModel::playerDown() {
    model->playerDown();
}

void GameViewModel::playerWalkLeft() {
    model->playerWalkLeft();
    player_towards = towards::LEFT;
}

void GameViewModel::playerWalkRight() {
    model->playerWalkRight();
    player_towards = towards::RIGHT;
}

void GameViewModel::playerStopLeft() {
    model->playerStopLeft();
}

void GameViewModel::playerStopRight() {
    model->playerStopRight();
}

std::vector<int> GameViewModel::getPlatformsId() {
    std::map<int, Model::Entities::Platform*> platforms = model->getPlatforms();
    std::vector<int> platform_ids;
    for (const auto& pair : platforms) {
        platform_ids.push_back(pair.first);
    }
    return platform_ids;
}

View::UI::PlatformType GameViewModel::getPlatformTypeById(int id) {
    Model::Entities::Platform* platform = model->getPlatformById(id);
    if (platform) {
        return static_cast<View::UI::PlatformType>(platform->type);
    }
    return View::UI::PlatformType::NORMAL;
}

sf::Vector2f GameViewModel::getPlatformPositionById(int id) {
    Model::Entities::Platform* platform = model->getPlatformById(id);
    if (platform) {
        return platform->position;
    }
    return sf::Vector2f(0, 0);
}