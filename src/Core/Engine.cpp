#include <iostream>

#include "Core/Engine.h"
#include "View/Page.h"
#include "View/MainMenuView.h"
#include "View/GameView.h"

using Core::Engine;
using View::MainMenuView;
using View::GameView;

Engine::Engine(std::string game_title, sf::Vector2u window_size, int fps) {
    this->game_title = game_title;
    window.create(sf::VideoMode(window_size.x, window_size.y), game_title, sf::Style::Default);

    this->fps = fps;

    window.setFramerateLimit(fps);
    changePage(PAGE_STATE::MAIN_MENU);
}

sf::Vector2u Engine::getWindowSize() const {
    return window.getSize();
}

void Engine::changePage(PAGE_STATE new_page_state) {
    std::cout << "change page." << std::endl;
    page_state = new_page_state;
    switch (page_state) {
        case PAGE_STATE::MAIN_MENU:
            page = std::make_shared<MainMenuView>(*this);
            break;
        case PAGE_STATE::GAME:
            page = std::make_shared<GameView>(*this);
            break;
    }
}

void Engine::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::KeyPressed) {
                handleInput(event);
            }
        }
        page->update(1.00f / this->fps);
        page->render(window);
    }
}

void Engine::handleInput(const sf::Event& event) {
    page->handleInput(event);
}

void Engine::startGame() {
    std::cout << "Game started!" << std::endl;
}

void Engine::exitGame() {
    std::cout << "Exiting game!" << std::endl;
    window.close();
}