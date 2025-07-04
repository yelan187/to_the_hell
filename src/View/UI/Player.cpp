#include <iostream>
#include "View/UI/Player.h"

using View::UI::Player;

void Player::init() {
    rect.setSize(size);
    rect.setOrigin(size.x / 2, size.y / 2);
}

void Player::update(Common::FrameInfo::PlayerInfo player_info) {
    if (player_info.texture != nullptr) {
        rect.setTexture(player_info.texture);
        rect.setFillColor(sf::Color::White);
    } else {
        rect.setTexture(nullptr);
        rect.setFillColor(sf::Color::Red);
    }
    rect.setPosition(player_info.position + player_info.size / 2.0f);
    size = player_info.size;
    rect.setSize(size);
    rect.setOrigin(size.x / 2, size.y / 2);
}

void Player::render(){
    if (rect.getTexture() != nullptr) {
        rect.setFillColor(sf::Color::White);
        window.draw(rect);
    } else {
        rect.setTexture(nullptr);
        rect.setFillColor(sf::Color::Red);
        window.draw(rect);
    }
}