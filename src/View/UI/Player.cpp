#include <iostream>
#include "View/UI/Player.h"

using View::UI::Player;

void Player::init() {
    rect.setSize(size);
    rect.setOrigin(size.x / 2, size.y / 2);

    health_bar.setSize(sf::Vector2f(size.x * 1.2, 8.0f));
    health_bar.setFillColor(sf::Color::Green);

    health_bar_background.setSize(health_bar.getSize());
    health_bar_background.setFillColor(sf::Color::Red);
}

void Player::update(Common::FrameInfo::PlayerInfo player_info) {
    rect.setTexture(player_info.texture);
    rect.setPosition(player_info.position + player_info.size / 2.0f);

    health_bar_background.setPosition(rect.getPosition().x - health_bar_background.getSize().x / 2, rect.getPosition().y - size.y / 2 - 15);

    health_bar.setPosition(health_bar_background.getPosition().x, health_bar_background.getPosition().y);

    health_bar.setSize(
        sf::Vector2f(
            health_bar_background.getSize().x * (static_cast<float>(player_info.hp) / static_cast<float>(player_info.max_hp)),
            health_bar_background.getSize().y
        )
    );
}

void Player::render(){
    window.draw(rect);
    window.draw(health_bar_background);
    window.draw(health_bar);
}