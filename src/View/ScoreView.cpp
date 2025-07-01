#include "View/ScoreView.h"
#include "Common/Config/Config.h"
#include <string>
#include <iostream>

using View::ScoreView;

namespace View {

void ScoreView::init() {
    option_pointer.setPointCount(3);
    option_pointer.setRadius(menu_options[0].getCharacterSize() / 3);
    option_pointer.setFillColor(sf::Color::Red);
    option_pointer.setOrigin(option_pointer.getRadius(), option_pointer.getRadius());
    option_pointer.rotate(90);
    updateCurrentSelection();
}

void ScoreView::updateCurrentSelection() {
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
                switch (current_selection) {
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

}
void ScoreView::notification_callback(Common::NotificationParam* param, void* view) {
    if (!view) return;
    ScoreView* main_menu_view = static_cast<ScoreView*>(view);
    switch (param->id) {
        case Common::NotificationId::ChangeCurrentSelection:
            main_menu_view->current_selection = dynamic_cast<Common::ChangeCurrentSelectionParam*>(param)->value;
            main_menu_view->updateCurrentSelection();
            break;
    }
}