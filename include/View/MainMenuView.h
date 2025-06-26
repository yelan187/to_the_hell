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
    void updateBackgroundParticles();

private:
    std::shared_ptr<ViewModel::MainMenuViewModel> view_model;
    
    sf::Text title_text;

    std::vector<sf::Text> menu_options;
    int current_selection;
    sf::Font font;

    sf::CircleShape option_pointer;

    std::vector<sf::CircleShape> background_particles;
};

}