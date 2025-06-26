#pragma once

#include "Core/Engine.h"
#include "ViewModel/ViewModel.h"
#include <SFML/Graphics.hpp>

namespace View {
class Page {
public:
    Page(Core::Engine &engine): engine(engine) {}
    virtual void update(float deltaTime) {}
    virtual void render(sf::RenderWindow& window) {}
    virtual void handleInput(const sf::Event& event) {}

protected:
    Core::Engine& engine;

private:
    std::shared_ptr<ViewModel::ViewModel> view_model;
};
}