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

    MainMenuViewModel(Core::Engine& engine, sf::Vector2u window_size);
    virtual ~MainMenuViewModel() = default;

    void navigateUp();
    void navigateDown();
    void confirmSelection();
    
    std::vector<std::string> getMenuOptions() const;
    int getCurrentSelectionIndex() const;

public:
    std::vector<sf::Vector2f> getBackgroundParticles() const;
    
    void updateAnimationState(float delta_time);
    
private:
    float animation_time_ = 0.0f;
    std::vector<sf::Vector2f> background_particles;

    std::shared_ptr<Model::MainMenuModel> model;
    std::vector<MenuOption> available_options;
    int current_selection_index;
    
    void updateAvailableOptions();
};

}