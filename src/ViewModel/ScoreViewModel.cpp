// #include "ViewModel/ScoreViewModel.h"
// #include "Model/ScoreModel.h"
// #include <iostream>

// namespace ViewModel {

// ScoreViewModel::ScoreViewModel(Core::Engine& engine, int score, std::chrono::seconds time) : engine(engine), current_selection(0) {
//     model = std::make_shared<Model::ScoreModel>(engine, score, time);
//     menu_options = {"Restart", "Main Menu", "Exit"};
// }

// void ScoreViewModel::navigateUp() {
//     current_selection = (current_selection - 1 + menu_options.size()) % menu_options.size();
// }

// void ScoreViewModel::navigateDown() {
//     current_selection = (current_selection + 1) % menu_options.size();
// }

// void ScoreViewModel::confirmSelection() {
//     switch (current_selection) {
//         case 0:
//             model->restartGame();
//             break;
//         case 1:
//             model->backToMainMenu();
//             break;
//         case 2:
//             model->exitGame();
//             break;
//     }
// }

// int ScoreViewModel::getScore() const {
//     return model->getScore();
// }

// std::chrono::seconds ScoreViewModel::getTime() const {
//     return model->getTime();
// }

// int ScoreViewModel::getCurrentSelectionIndex() const {
//     return current_selection;
// }

// const std::vector<std::string>& ScoreViewModel::getMenuOptions() const {
//     return menu_options;
// }

// }
