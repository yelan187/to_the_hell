#pragma once

#include <ViewModel/ViewModel.h>
#include <Model/MainMenuModel.h>
#include <functional>
#include <vector>
#include <string>

namespace ViewModel {

class MainMenuViewModel : public ViewModel {
public:
    enum class MenuOption {
        START_GAME,
        EXIT
    };

    MainMenuViewModel(Core::Engine& engine);
    virtual ~MainMenuViewModel() = default;

    void navigateUp();
    void navigateDown();
    void confirmSelection();
    
    std::vector<std::string> getMenuOptions() const;
    int getCurrentSelectionIndex() const;

private:
    std::shared_ptr<Model::MainMenuModel> model;
    std::vector<MenuOption> available_options;
    int current_selection_index;
    
    void updateAvailableOptions();
};

}