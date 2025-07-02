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
    menu_options = {"Start", "Exit"};
}

// update
void MainMenuViewModel::navigateUp() {
    current_selection_index = (current_selection_index - 1 + menu_options.size()) % menu_options.size();
}

void MainMenuViewModel::navigateDown() {
    current_selection_index = (current_selection_index + 1) % menu_options.size();
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