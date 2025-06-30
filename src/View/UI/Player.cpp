#include <iostream>
#include "View/UI/Player.h"

using View::UI::Player;

void Player::init() {
    loadTextures();
    size = sf::Vector2f(60,60);
    rect.setSize(size);
    rect.setOrigin(size.x / 2, size.y / 2);
}

void Player::update(Common::FrameInfo::PlayerInfo player_info) {
    setTexture(player_info.state);
    rect.setPosition((player_info.position + player_info.size) / 2.0f);
}

void Player::render(){
    window.draw(rect);
}

void Player::loadTextures() {
    // std::cout << "Loading player textures..." << std::endl;
    // Load textures for different player states
    try {
        sf::Texture idle_l_texture;
        idle_l_texture.loadFromFile("assets/images/player/player_idle_l.png");
        texture_map[Utils::GameViewModelPlayerState::IDLE_L] = idle_l_texture;
    } catch (const std::exception& e) {
        std::cout << "Error loading player textures: " << e.what() << std::endl;
    }
    try {
        sf::Texture idle_r_texture;
        idle_r_texture.loadFromFile("assets/images/player/player_idle_r.png");
        texture_map[Utils::GameViewModelPlayerState::IDLE_R] = idle_r_texture;
    } catch (const std::exception& e) {
        std::cout << "Error loading player textures: " << e.what() << std::endl;
    }
    try {
        sf::Texture walk_l_texture;
        walk_l_texture.loadFromFile("assets/images/player/player_walk_l.png");
        texture_map[Utils::GameViewModelPlayerState::WALKING_L] = walk_l_texture;
    } catch (const std::exception& e) {
        std::cout << "Error loading player textures: " << e.what() << std::endl;
    }
    try {
        sf::Texture walk_r_texture;
        walk_r_texture.loadFromFile("assets/images/player/player_walk_r.png");
        texture_map[Utils::GameViewModelPlayerState::WALKING_R] = walk_r_texture;
    } catch (const std::exception& e) {
        std::cout << "Error loading player textures: " << e.what() << std::endl;
    }
    try {
        sf::Texture jump_l_texture;
        jump_l_texture.loadFromFile("assets/images/player/player_jump_l.png");
        texture_map[Utils::GameViewModelPlayerState::JUMPING_L] = jump_l_texture;
    } catch (const std::exception& e) {
        std::cout << "Error loading player textures: " << e.what() << std::endl;
    }
    try {
        sf::Texture jump_r_texture;
        jump_r_texture.loadFromFile("assets/images/player/player_jump_r.png");
        texture_map[Utils::GameViewModelPlayerState::JUMPING_R] = jump_r_texture;
    } catch (const std::exception& e) {
        std::cout << "Error loading player textures: " << e.what() << std::endl;
    }
}

void Player::setTexture(Utils::GameViewModelPlayerState state) {
    switch (state) {
        case Utils::GameViewModelPlayerState::IDLE_L:
            rect.setTexture(&texture_map[Utils::GameViewModelPlayerState::IDLE_L]);
            break;
        case Utils::GameViewModelPlayerState::IDLE_R:
            rect.setTexture(&texture_map[Utils::GameViewModelPlayerState::IDLE_R]);
            break;
        case Utils::GameViewModelPlayerState::WALKING_L:
            rect.setTexture(&texture_map[Utils::GameViewModelPlayerState::WALKING_L]);
            break;
        case Utils::GameViewModelPlayerState::WALKING_R:
            rect.setTexture(&texture_map[Utils::GameViewModelPlayerState::WALKING_R]);
            break;
        case Utils::GameViewModelPlayerState::JUMPING_L:
            rect.setTexture(&texture_map[Utils::GameViewModelPlayerState::JUMPING_L]);
            break;
        case Utils::GameViewModelPlayerState::JUMPING_R:
            rect.setTexture(&texture_map[Utils::GameViewModelPlayerState::JUMPING_R]);
            break;
        default:
            break;
    }
}