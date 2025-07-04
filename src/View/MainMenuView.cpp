#include <iostream>
#include "View/MainMenuView.h"

using View::MainMenuView;

void MainMenuView::init() {
    title_sprite.setTexture(title_texture);
    title_sprite.setScale(0.4f, 0.4f);
    sf::FloatRect bounds = title_sprite.getLocalBounds();
    title_sprite.setOrigin(bounds.width / 2, bounds.height / 2);
    title_sprite.setPosition(window_size.x / 2, window_size.y / 4);

    option_pointer.setPointCount(3);
    option_pointer.setRadius(menu_options[*current_selection].getCharacterSize() / 3);
    option_pointer.setFillColor(sf::Color::Red);
    option_pointer.setOrigin(option_pointer.getRadius(), option_pointer.getRadius());
    option_pointer.rotate(90);
    updateCurrentSelection();
}

// init
void MainMenuView::initMenuOptions() {
    menu_options.clear();
    for (const auto& option : *menu_options_text) {
        sf::Text text;
        text.setString(option);
        text.setCharacterSize(50);
        text.setFillColor(sf::Color::White);
        text.setFont(font);
        auto textRect = text.getLocalBounds();
        text.setOrigin(textRect.width / 2, textRect.height / 2);
        text.setPosition(
            window_size.x / 2,
            window_size.y / 2 + (menu_options.size() * text.getCharacterSize() * 1.5f)
        );
        menu_options.push_back(text);
    }
}

// update
void MainMenuView::handleInput(const sf::Event& event) {
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
                // 先播放音效
                if (viewmodel_confirm_command) {
                    viewmodel_confirm_command->execute();
                }
                // 然后处理页面切换
                switch (*current_selection) {
                    case 0:
                        change_page_param.value.new_page_state = View::PAGE_STATE::GAME;
                        break;
                    case 1:
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

void MainMenuView::updateCurrentSelection() {
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

void MainMenuView::updateBackgroundParticles() {
    background_particles.clear();
    for (const auto& pos : *background_particles_pos) {
        sf::CircleShape particle(2.0f);
        particle.setFillColor(sf::Color(255, 255, 255, 100));
        particle.setPosition(pos);
        background_particles.push_back(particle);
    }
}

void MainMenuView::notification_callback(Common::NotificationId id, void* view) {
    if (!view) return;
    MainMenuView* main_menu_view = static_cast<MainMenuView*>(view);
    switch (id) {
        case Common::NotificationId::ChangeCurrentSelection:
            main_menu_view->updateCurrentSelection();
            break;
        case Common::NotificationId::ChangeBackgroundParticles:
            main_menu_view->updateBackgroundParticles();
            break;
    }
}

// render
void MainMenuView::render() {
    window.clear(sf::Color::Black);

    for (const auto& particle : background_particles) {
        window.draw(particle);
    }
    window.draw(title_sprite);

    for (int i = 0; i < menu_options.size(); ++i) {
        window.draw(menu_options[i]);
    }
    window.draw(option_pointer);
    window.display();
}