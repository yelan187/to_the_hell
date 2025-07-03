#include <iostream>
#include "View/UI/Choice.h"

using View::UI::Choice;

void Choice::init() {
    if (!font.loadFromFile("assets/fonts/fusion.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
        return;
    }
    sf::Vector2u window_size = window.getSize();
    rect.setSize(sf::Vector2f(window_size.x * 0.25f, window_size.y * 0.6f));
    rect.setFillColor(sf::Color::Black);
    rect.setOrigin(rect.getSize().x / 2, rect.getSize().y / 2);
    rect.setPosition((window_size.x * 0.25f / 4)*(index + 1) + (rect.getSize().x / 2)*(index * 2 + 1), window_size.y / 2.0f);

    name.setCharacterSize(30);
    name.setFillColor(sf::Color::White);
    name.setFont(font);

    description.setCharacterSize(18);
    description.setFillColor(sf::Color::White);
    description.setFont(font);
}

void Choice::update(Common::ChoiceInfo info) {
    name.setString(info.name);
    name.setOrigin(name.getLocalBounds().width / 2, name.getLocalBounds().height / 2);
    name.setPosition(rect.getPosition().x, rect.getPosition().y - rect.getSize().y / 2 + 20);

    description.setString(info.description);
    description.setOrigin(description.getLocalBounds().width / 2, description.getLocalBounds().height / 2);
    description.setPosition(rect.getPosition().x, rect.getPosition().y + rect.getSize().y / 2 - 20);
}

void Choice::render(){
    window.draw(rect);
    window.draw(name);
    window.draw(description);
}