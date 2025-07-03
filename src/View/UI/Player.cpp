#include <iostream>
#include "View/UI/Player.h"

using View::UI::Player;

void Player::init() {
    rect.setSize(size);
    rect.setOrigin(size.x / 2, size.y / 2);
}

void Player::update(Common::FrameInfo::PlayerInfo player_info) {
    // 只有当texture不为空时才设置纹理
    if (player_info.texture != nullptr) {
        rect.setTexture(player_info.texture);
    }
    rect.setPosition(player_info.position + player_info.size / 2.0f);
    // 更新size以防配置发生变化
    size = player_info.size;
    rect.setSize(size);
    rect.setOrigin(size.x / 2, size.y / 2);
}

void Player::render(){
    window.draw(rect);
}