// #pragma once

// #include "Core/Engine.h"
// #include "Model/ScoreModel.h"
// #include <memory>
// #include <chrono>
// #include <vector>
// #include <string>

// namespace ViewModel {

// class ScoreViewModel {
// public:
//     ScoreViewModel(Core::Engine& engine, int score, std::chrono::seconds time);

//     void navigateUp();
//     void navigateDown();
//     void confirmSelection();

//     int getScore() const;
//     std::chrono::seconds getTime() const;
//     int getCurrentSelectionIndex() const;
//     const std::vector<std::string>& getMenuOptions() const;

// private:
//     Core::Engine& engine;
//     std::shared_ptr<Model::ScoreModel> model;
//     int current_selection;
//     std::vector<std::string> menu_options;
// };

// }
