#include "View/ScoreView.h"
#include "Core/Engine.h"
#include "Utils/Config.h"
#include "ViewModel/ScoreViewModel.h"
#include <string>
#include <iostream>

namespace View {

ScoreView::ScoreView(Core::Engine& engine, int score, std::chrono::seconds time) : Page(engine) {
    
    view_model = std::make_shared<ViewModel::ScoreViewModel>(engine, score, time);

    if (!font.loadFromFile("assets/fonts/fusion.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
        return;
    }

    score_text.setFont(font);
    score_text.setString("Score: " + std::to_string(view_model->getScore())); // Placeholder
    score_text.setCharacterSize(50);
    score_text.setFillColor(sf::Color::White);
    sf::FloatRect textRect = score_text.getLocalBounds();
    score_text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
    score_text.setPosition(engine.getWindowSize().x / 2.0f, engine.getWindowSize().y / 2.0f - 150.f);

    time_text.setFont(font);
    time_text.setString("Time: " + std::to_string(view_model->getTime().count()) + "s"); // Placeholder
    time_text.setCharacterSize(50);
    time_text.setFillColor(sf::Color::White);
    textRect = time_text.getLocalBounds();
    time_text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
    time_text.setPosition(engine.getWindowSize().x / 2.0f, engine.getWindowSize().y / 2.0f - 50.f);

    for (const auto& s : view_model->getMenuOptions()) {
        sf::Text text;
        text.setString(s);
        text.setFont(font);
        text.setCharacterSize(40);
        text.setFillColor(sf::Color::White);
        textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
        text.setPosition(engine.getWindowSize().x / 2.0f, engine.getWindowSize().y / 2.f + 50.f + menu_options.size() * 50.f);
        menu_options.push_back(text);
    }

    option_pointer.setPointCount(3);
    option_pointer.setRadius(menu_options[0].getCharacterSize() / 3);
    option_pointer.setFillColor(sf::Color::Red);
    option_pointer.setOrigin(option_pointer.getRadius(), option_pointer.getRadius());
    option_pointer.rotate(90);
}

void ScoreView::update(float deltaTime) {
    int current_selection = view_model->getCurrentSelectionIndex();

    for (int i = 0; i < menu_options.size(); ++i) {
        if (i == current_selection) {
            menu_options[i].setFillColor(sf::Color::Yellow);
        } else {
            menu_options[i].setFillColor(sf::Color::White);
        }
    }

    sf::Text& selected_text = menu_options[current_selection];
    sf::FloatRect text_rect = selected_text.getGlobalBounds();
    float pointer_x = window_size.x / 2 - selected_text.getCharacterSize() * 3;
    float pointer_y = text_rect.top + text_rect.height/2;
    option_pointer.setPosition(pointer_x, pointer_y);
}

void ScoreView::render(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);
    window.draw(score_text);
    window.draw(time_text);
    for (const auto& option : menu_options) {
        window.draw(option);
    }
    window.draw(option_pointer);
    window.display();
}

void ScoreView::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
            case sf::Keyboard::Up:
            case sf::Keyboard::W:
                view_model->navigateUp();
                break;
            case sf::Keyboard::Down:
            case sf::Keyboard::S:
                view_model->navigateDown();
                break;
            case sf::Keyboard::Enter:
            case sf::Keyboard::J:
                view_model->confirmSelection();
                break;
            default:
                break;
        }
    }
}

}
