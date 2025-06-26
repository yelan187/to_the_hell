#pragma once

#include "Core/Engine.h"
#include "ViewModel/ViewModel.h"
#include <SFML/Graphics.hpp>

namespace View {
class Page {
public:
    Page(Core::Engine &engine) :engine(engine) {
        this->window_size = engine.getWindowSize();
        // std::cout << "Page initialized with window size: " << window_size.x << "x" << window_size.y << std::endl;
    }
    virtual void update(float deltaTime) {}
    virtual void render(sf::RenderWindow& window) {}
    virtual void handleInput(const sf::Event& event) {}

protected:
    Core::Engine& engine;
    sf::Vector2u window_size;

private:
    std::shared_ptr<ViewModel::ViewModel> view_model;
};
}