#include <ViewModel/MainMenuViewModel.h>
#include <Model/MainMenuModel.h>
#include <iostream>

namespace ViewModel {

MainMenuViewModel::MainMenuViewModel(Core::Engine &engine) 
    : ViewModel(engine),
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
}