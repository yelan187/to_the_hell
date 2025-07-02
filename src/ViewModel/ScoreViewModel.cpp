#include "ViewModel/ScoreViewModel.h"
#include <iostream>

using ViewModel::ScoreViewModel;

ScoreViewModel::ScoreViewModel(sf::Vector2u window_size) : 
    ViewModel(window_size),
    model(nullptr),
    current_selection_index(0),
    navigateUp_command(this),
    navigateDown_command(this),
    update_command(this) 
{
    menu_options = {"Restart", "Main Menu", "Exit"};
}


void ScoreViewModel::navigateUp() {
    current_selection_index = (current_selection_index - 1 + menu_options.size()) % menu_options.size();
}

void ScoreViewModel::navigateDown() {
    current_selection_index = (current_selection_index + 1) % menu_options.size();
}
