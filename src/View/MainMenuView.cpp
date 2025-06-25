#include <iostream>
#include "View/MainMenuView.h"

using View::MainMenuView;

MainMenuView::MainMenuView(Core::Engine &engine) : View::Page(engine) {
    if (!font.loadFromFile("assets/fonts/arial.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
        return;
    }
    view_model = std::make_shared<ViewModel::MainMenuViewModel>(engine);
    current_selection = view_model->getCurrentSelectionIndex();
    for (auto s: view_model->getMenuOptions()){
        sf::Text text;
        text.setString(s);
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::White);
        text.setFont(font);
        std::cout << text.getString().toAnsiString() << std::endl;
        menu_options.push_back(text);
    }
}

void MainMenuView::update(float deltaTime) {
    current_selection = view_model->getCurrentSelectionIndex();
}

void MainMenuView::render(sf::RenderWindow& window) {

    window.clear(sf::Color::Black);
    // std::cout << menu_options.size() << " menu options." << std::endl;
    for (int i = 0; i < menu_options.size(); ++i) {
        if (i == current_selection) {
            menu_options[i].setFillColor(sf::Color::Yellow); // Highlight
        } else {
            menu_options[i].setFillColor(sf::Color::White);
        }
        menu_options[i].setPosition(100, 100 + i * 30); // Align vertically
        // std::cout << "Rendering option: " << menu_options[i].getString().toAnsiString() << std::endl;
        window.draw(menu_options[i]);
    }
    
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