#include <ViewModel/MainMenuViewModel.h>
#include <Model/MainMenuModel.h>
#include <iostream>
#include <cmath> 

namespace ViewModel {

MainMenuViewModel::MainMenuViewModel(Core::Engine &engine, sf::Vector2u window_size) 
    : ViewModel(engine, window_size),
      model(std::make_shared<Model::MainMenuModel>(engine)),
      current_selection_index(0) {
    
    updateAvailableOptions();
}

void MainMenuViewModel::updateAvailableOptions() {
    available_options.clear();
    available_options.push_back(MenuOption::START_GAME);
    available_options.push_back(MenuOption::EXIT);
}

std::vector<std::string> MainMenuViewModel::getMenuOptions() const {
    std::vector<std::string> options;
    for (const auto& option : available_options) {
        switch(option) {
            case MenuOption::START_GAME:
                options.push_back("Start");
                break;
            case MenuOption::EXIT:
                options.push_back("Exit");
                break;
        }
    }
    return options;
}

int MainMenuViewModel::getCurrentSelectionIndex() const {
    return current_selection_index;
}

void MainMenuViewModel::navigateUp() {
    if (available_options.empty()) return;
    
    current_selection_index --;
    if (current_selection_index < 0) {
        current_selection_index = available_options.size() - 1;
    }
}

void MainMenuViewModel::navigateDown() {
    if (available_options.empty()) return;
    
    current_selection_index++;
    if (current_selection_index >= available_options.size()) {
        current_selection_index = 0;
    }

}

void MainMenuViewModel::confirmSelection() {
    if (available_options.empty() || 
        current_selection_index < 0 || 
        current_selection_index >= available_options.size()) {
        return;
    }
    
    MenuOption selected = available_options[current_selection_index];

    switch (selected) {
        case MenuOption::START_GAME:
            std::cout << "开始游戏" << std::endl;
            model->startGame();
            break;
        case MenuOption::EXIT:
            std::cout << "退出游戏" << std::endl;
            model->exitGame();
            break;
    }
}

sf::Color MainMenuViewModel::getTitleColor() const {
    int r = 255;
    int g = 215 + 40 * std::sin(animation_time_ * 1.5f);
    int b = 0;
    return sf::Color(r, g, b);
}

void MainMenuViewModel::updateAnimationState(float delta_time) {
    animation_time_ += delta_time;

    if (background_particles.empty()) {
        for (int i = 0; i < 20; i++) {
            float x = static_cast<float>(rand() % window_size.x);
            float y = static_cast<float>(rand() % window_size.y);
            background_particles.push_back({x, y});
        }
    }

    for (auto& particle : background_particles) {
        particle.y += 50.0f * delta_time;
        if (particle.y > window_size.y) {
            particle.y = 0;
            particle.x = static_cast<float>(rand() % window_size.x);
        }
    }
}

std::vector<sf::Vector2f> MainMenuViewModel::getBackgroundParticles() const {
    return background_particles;
}

}