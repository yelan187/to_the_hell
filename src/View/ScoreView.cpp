#include "View/ScoreView.h"
#include "Common/Config/Config.h"
#include <string>
#include <iostream>

using View::ScoreView;

void ScoreView::init() {
    option_pointer.setPointCount(3);
    option_pointer.setRadius(menu_options[0].getCharacterSize() / 3);
    option_pointer.setFillColor(sf::Color::Red);
    option_pointer.setOrigin(option_pointer.getRadius(), option_pointer.getRadius());
    option_pointer.rotate(90);
    updateCurrentSelection();
}

//init 
void ScoreView::initMenuOptions() {
    menu_options.clear();
    for (const auto& option : *menu_options_text) {
        sf::Text text;
        text.setString(option);
        text.setCharacterSize(40);
        text.setFillColor(sf::Color::White);
        text.setFont(font);
        auto textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2, textRect.top + textRect.height / 2);
        text.setPosition(
            window_size.x / 2,
            window_size.y / 2 + 50.0f + menu_options.size() * 50.0f
        );
        menu_options.push_back(text);
    }
}
void ScoreView::initTotalScoreText() {
    score_text.setFont(font);
    score_text.setString(*total_score);
    score_text.setCharacterSize(50);
    score_text.setFillColor(sf::Color::White);
    sf::FloatRect textRect = score_text.getLocalBounds();
    score_text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    score_text.setPosition(window_size.x / 2.0f, window_size.y / 2.0f - 150.f);
}
void ScoreView::initTimeText() {
    time_text.setFont(font);
    time_text.setString(*game_time);
    time_text.setCharacterSize(50);
    time_text.setFillColor(sf::Color::White);
    sf::FloatRect textRect = time_text.getLocalBounds();
    time_text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    time_text.setPosition(window_size.x / 2.0f, window_size.y / 2.0f - 50.f);
}

// update
void ScoreView::updateCurrentSelection() {
    for (int i = 0; i < menu_options.size(); ++i) {
        if (i == *current_selection) {
            menu_options[i].setFillColor(sf::Color::Yellow);
        } else {
            menu_options[i].setFillColor(sf::Color::White);
        }
    }
    
    sf::Text& selected_text = menu_options[*current_selection];
    sf::FloatRect text_rect = selected_text.getGlobalBounds();
    float pointer_x = window_size.x / 2 - selected_text.getCharacterSize() * 3;
    float pointer_y = text_rect.top + text_rect.height/2;
    option_pointer.setPosition(pointer_x, pointer_y);
}
void ScoreView::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        Common::ChangePageParam change_page_param;
        switch (event.key.code) {
            case sf::Keyboard::Up:
            case sf::Keyboard::W:
                navigateUp_command->execute();
                break;
            case sf::Keyboard::Down:
            case sf::Keyboard::S:
                navigateDown_command->execute();
                break;
            case sf::Keyboard::Enter:
            case sf::Keyboard::J:
                switch (*current_selection) {
                    case 0:
                        change_page_param.value.new_page_state = View::PAGE_STATE::GAME;
                        break;
                    case 1:
                        change_page_param.value.new_page_state = View::PAGE_STATE::MAIN_MENU;
                        break;
                    case 2:
                        change_page_param.value.new_page_state = View::PAGE_STATE::EXIT;
                        break;
                }
                change_page_param.value.init = true;
                confirmSelection_command->execute(change_page_param);
                break;
            default:
                break;
        }
    }
}

void ScoreView::notification_callback(Common::NotificationId id, void* view) {
    if (!view) return;
    ScoreView* score_view = static_cast<ScoreView*>(view);
    switch (id) {
        case Common::NotificationId::ChangeCurrentSelection:
            score_view->updateCurrentSelection();
            break;
    }
}

// render
void ScoreView::render() {
    window.clear(sf::Color::Black);
    window.draw(score_text);
    window.draw(time_text);
    for (const auto& option : menu_options) {
        window.draw(option);
    }
    window.draw(option_pointer);
    window.display();
}