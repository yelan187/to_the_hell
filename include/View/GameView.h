#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "View/Page.h"
#include "View/UI/Player.h"
#include "View/UI/Platform.h"
#include "ViewModel/GameViewModel.h"

namespace View {

class GameView : public Page {
public:

    GameView(Core::Engine& engine);

    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    void handleInput(const sf::Event& event) override;

private:
    std::shared_ptr<ViewModel::GameViewModel> view_model;

    sf::Font font;
    sf::Text game_time_text;
    sf::Text total_score_text;

    bool debug;
    sf::Text debug_info_text;

    View::UI::Player player;
    
    std::vector<View::UI::Platform> platforms;
};

}