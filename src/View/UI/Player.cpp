#include <iostream>
#include "View/UI/Player.h"

using View::UI::Player;

void Player::init() {
    rect.setSize(size);
    rect.setOrigin(size.x / 2, size.y / 2);
}

void Player::update(Common::FrameInfo::PlayerInfo player_info) {
    rect.setTexture(player_info.texture);
    rect.setPosition(player_info.position + player_info.size / 2.0f);
}

void Player::render(){
    window.draw(rect);
}