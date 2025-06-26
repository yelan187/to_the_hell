#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "View/Page.h"
#include "ViewModel/MainMenuViewModel.h"

namespace View {

class MainMenuView : public Page {
public:

    MainMenuView(Core::Engine& engine);

    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    void handleInput(const sf::Event& event) override;

private:
    std::shared_ptr<ViewModel::MainMenuViewModel> view_model;
    
    std::vector<sf::Text> menu_options;
    int current_selection;
    sf::Font font;

    sf::CircleShape option_pointer;
};

}