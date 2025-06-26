#include <iostream>
#include "View/MainMenuView.h"

using View::MainMenuView;

MainMenuView::MainMenuView(Core::Engine &engine) : View::Page(engine) {
    if (!font.loadFromFile("assets/fonts/JetBrainsMono.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
        return;
    }
    view_model = std::make_shared<ViewModel::MainMenuViewModel>(engine);
    current_selection = view_model->getCurrentSelectionIndex();
    for (auto s: view_model->getMenuOptions()){
        sf::Text text;
        text.setString(s);
        text.setCharacterSize(50);
        text.setFillColor(sf::Color::White);
        text.setFont(font);
        text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
        text.setPosition(
            window_size.x / 2,
            window_size.y / 2 + (menu_options.size() * text.getCharacterSize() * 1.5f) - text.getCharacterSize() * 1.5f
        );
        std::cout << text.getString().toAnsiString() << std::endl;
        menu_options.push_back(text);
    }

    option_pointer.setPointCount(3);
    option_pointer.setRadius(menu_options[0].getCharacterSize() / 3);
    option_pointer.setFillColor(sf::Color::Red);
    option_pointer.setOrigin(option_pointer.getRadius(), option_pointer.getRadius());
    option_pointer.rotate(90);
}

void MainMenuView::update(float deltaTime) {
    current_selection = view_model->getCurrentSelectionIndex();

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

void MainMenuView::render(sf::RenderWindow& window) {

    window.clear(sf::Color::Black);
    for (int i = 0; i < menu_options.size(); ++i) {
        window.draw(menu_options[i]);
    }
    window.draw(option_pointer);
    window.display();
}

void MainMenuView::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
            case sf::Keyboard::W:
                view_model->navigateUp();
                break;
            case sf::Keyboard::S:
                view_model->navigateDown();
                break;
            case sf::Keyboard::Enter:
                view_model->confirmSelection();
                break;
            default:
                break;
        }
    }
}