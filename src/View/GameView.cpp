#include <iostream>
#include "View/GameView.h"

using View::GameView;

GameView::GameView(Core::Engine &engine) : View::Page(engine), debug(true) {
    if (!font.loadFromFile("assets/fonts/fusion.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
        return;
    }
    view_model = std::make_shared<ViewModel::GameViewModel>(engine,window_size);
    
    total_score_text.setString(view_model->getTotalScore());
    total_score_text.setCharacterSize(24);
    total_score_text.setFillColor(sf::Color::White);
    total_score_text.setFont(font);
    total_score_text.setPosition(15,15);

    game_time_text.setString(view_model->getGameTime());
    game_time_text.setCharacterSize(24);
    game_time_text.setFillColor(sf::Color::White);
    game_time_text.setFont(font);
    game_time_text.setPosition(15,40);

    if (debug) {
        std::cout << "Debug mode is ON" << std::endl;
        debug_info_text.setString(view_model->getDebugInfo());
        debug_info_text.setCharacterSize(24);
        debug_info_text.setFillColor(sf::Color::White);
        debug_info_text.setFont(font);
        debug_info_text.setPosition(15,65);
    }

    player.init(view_model);
    for (int id : view_model->getPlatformsId()){
        View::UI::Platform p;
        p.init(view_model,id);
        platforms.push_back(p);
    }
}

void GameView::update(float deltaTime) {
    view_model->update(deltaTime);

    total_score_text.setString(view_model->getTotalScore());

    game_time_text.setString(view_model->getGameTime());

    if (debug) {
        debug_info_text.setString(view_model->getDebugInfo());
    }

    player.update(deltaTime);
    platforms.clear();
    for (int id : view_model->getPlatformsId()){
        View::UI::Platform p;
        p.init(view_model,id);
        platforms.push_back(p);
    }
}

void GameView::render(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);
    
    window.draw(game_time_text);
    window.draw(total_score_text);

    if (debug) {
        window.draw(debug_info_text);
    }

    player.render(window);

    for (auto &p: platforms)
        p.render(window);

    window.display();
}

void GameView::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
            case sf::Keyboard::W:
                view_model->playerJump();
                break;
            case sf::Keyboard::S:
                view_model->playerDown();
                break;
            case sf::Keyboard::A:
                view_model->playerWalkLeft();
                break;
            case sf::Keyboard::D:
                view_model->playerWalkRight();
                break;
            default:
                break;
        }
    } else if (event.type == sf::Event::KeyReleased) {
        switch (event.key.code) {
            case sf::Keyboard::A:
                view_model->playerStopLeft();
                break;
            case sf::Keyboard::D:
                view_model->playerStopRight();
                break;
            case sf::Keyboard::W:
                view_model->playerStopJump();
                break;
            case sf::Keyboard::S:
                view_model->playerStopDown();
                break;
            default:
                break;
        }
    }
}