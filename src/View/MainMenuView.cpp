#include <iostream>
#include "View/MainMenuView.h"

using View::MainMenuView;

void MainMenuView::initMenu() {
    if (!font.loadFromFile("assets/fonts/fusion.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
        return;
    }    
    if (!title_texture.loadFromFile("assets/images/title.png")) {
        std::cerr << "Error loading title image!" << std::endl;
        return;
    }
    
    title_sprite.setTexture(title_texture);
    title_sprite.setScale(0.4f, 0.4f);
    sf::FloatRect bounds = title_sprite.getLocalBounds();
    title_sprite.setOrigin(bounds.width / 2, bounds.height / 2);
    title_sprite.setPosition(window_size.x / 2, window_size.y / 4);

    option_pointer.setPointCount(3);
    option_pointer.setRadius(menu_options[0].getCharacterSize() / 3);
    option_pointer.setFillColor(sf::Color::Red);
    option_pointer.setOrigin(option_pointer.getRadius(), option_pointer.getRadius());
    option_pointer.rotate(90);
}

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
                change_page_param.value.new_page_state = static_cast<View::PAGE_STATE>(current_selection);
                change_page_param.value.deferred = false;
                confirmSelection_command->execute(change_page_param);
                break;
            default:
                break;
        }
    }
}

void MainMenuView::updateCurrentSelection(int selection) {
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

void MainMenuView::updateBackgroundParticles(std::vector<sf::Vector2f>* particles) {
    background_particles.clear();
    for (const auto& pos : *particles) {
        sf::CircleShape particle(2.0f);
        particle.setFillColor(sf::Color(255, 255, 255, 100));
        particle.setPosition(pos);
        background_particles.push_back(particle);
    }
}

static void notification_callback(Common::NotificationParam* param, void* view) {
    if (!view) return;
    MainMenuView* main_menu_view = static_cast<MainMenuView*>(view);
    switch (param->id) {
        case Common::NotificationId::ChangeCurrentSelection:
            int selection = dynamic_cast<Common::ChangeCurrentSelectionParam*>(param)->value;
            main_menu_view->updateCurrentSelection(selection);
            break;
        case Common::NotificationId::ChangeBackgroundParticles:
            std::vector<sf::Vector2f>* particles = dynamic_cast<Common::ChangeBackgroundParticlesParam*>(param)->value;
            main_menu_view->updateBackgroundParticles(particles);
            break;
    }
}

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