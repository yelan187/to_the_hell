// #pragma once

// #include <SFML/Graphics.hpp>
// #include <memory>
// #include <vector>
// #include "View/Page.h"
// #include "ViewModel/ScoreViewModel.h"

// namespace View {

// class ScoreView : public Page {
// public:
//     ScoreView(Core::Engine& engine, int score, std::chrono::seconds time);

//     void update(float deltaTime) override;
//     void render(sf::RenderWindow& window) override;
//     void handleInput(const sf::Event& event) override;

// private:
//     std::shared_ptr<ViewModel::ScoreViewModel> view_model;
    
//     sf::Font font;
//     sf::Text score_text;
//     sf::Text time_text;
//     std::vector<sf::Text> menu_options;

//     sf::CircleShape option_pointer;
// };

// }
