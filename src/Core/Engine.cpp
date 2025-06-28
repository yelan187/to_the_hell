#include <iostream>

#include "Core/Engine.h"
#include "View/Page.h"
#include "View/MainMenuView.h"
#include "View/GameView.h"
#include "View/ScoreView.h"

using Core::Engine;
using View::MainMenuView;
using View::GameView;
using View::ScoreView;

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
        case PAGE_STATE::SCORE:
            page = std::make_shared<ScoreView>(*this, last_game_result.score, last_game_result.time);
            break;
    }
}

void Engine::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) {
                handleInput(event);
            }
        }
        page->update(1.00f / fps);
        page->render(window);
        // Deferred end game mechanism
        if (pending_end_game) {
            last_game_result = {pending_score, pending_time};
            changePage(PAGE_STATE::SCORE);
            pending_end_game = false;
        }
        // Deferred page switching mechanism
        if (pending_page_change) {
            changePage(pending_page_state);
            pending_page_change = false;
        }
    }
}

void Engine::handleInput(const sf::Event& event) {
    page->handleInput(event);
}

void Engine::startGame() {
    // std::cout << "Game started!" << std::endl;
    requestPageChange(PAGE_STATE::GAME);
}

void Engine::exitGame() {
    // std::cout << "Exiting game!" << std::endl;
    window.close();
}

void Engine::requestPageChange(PAGE_STATE state) {
    pending_page_change = true;
    pending_page_state = state;
}

bool Engine::hasPendingPageChange() const {
    return pending_page_change;
}

Engine::PAGE_STATE Engine::getPendingPageState() const {
    return pending_page_state;
}

void Engine::requestEndGame(int score, std::chrono::seconds time) {
    pending_end_game = true;
    pending_score = score;
    pending_time = time;
}

bool Engine::hasPendingEndGame() const {
    return pending_end_game;
}

std::pair<int, std::chrono::seconds> Engine::getPendingEndGame() const {
    return {pending_score, pending_time};
}