#include "ViewModel/MainMenuViewModel.h"
#include <iostream>
#include <cmath> 

using ViewModel::MainMenuViewModel;

MainMenuViewModel::MainMenuViewModel(sf::Vector2u window_size) : 
    ViewModel(window_size),
    model(nullptr),
    current_selection_index(0),
    animation_time(0.0f),
    navigateUp_command(this),
    navigateDown_command(this),
    update_command(this) 
{
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

void MainMenuViewModel::updateAnimationState(float delta_time) {
    animation_time += delta_time;

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

void MainMenuViewModel::UpdateCommand::execute(Common::CommandParam& delta_time) {
    auto& update_param = dynamic_cast<Common::UpdateParam&>(delta_time);
    view_model->updateAnimationState(update_param.value);
    Common::ChangeBackgroundParticlesParam* param = new Common::ChangeBackgroundParticlesParam();
    param->id = Common::NotificationId::ChangeBackgroundParticles;
    param->value = &view_model->background_particles;
    view_model->trigger.fire(param);
}
