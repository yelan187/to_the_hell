#include <iostream>
#include <View/UI/Player.h>
#include <ViewModel/GameViewModel.h>

using View::UI::Player;

void Player::init(std::shared_ptr<ViewModel::GameViewModel> view_model) {
    size = sf::Vector2f(50, 50);
    loadTextures();
    switch (view_model->getPlayerState()) {
        case View::UI::PlayerState::IDLE_L:
            rect.setTexture(&texture_map[PlayerState::IDLE_R]);
            break;
        case View::UI::PlayerState::IDLE_R:
            rect.setTexture(&texture_map[PlayerState::IDLE_R]);
            break;
    }
    this->view_model = view_model;
    rect.setPosition(view_model->getPlayerPosition());
    rect.setSize(size);
}

void Player::update(float delta_time) {
    switch (view_model->getPlayerState()) {
        case View::UI::PlayerState::IDLE_L:
            break;
        case View::UI::PlayerState::IDLE_R:
            rect.setTexture(&texture_map[PlayerState::IDLE_R]);
            break;
    }
    // std::cout << "Player position: " << view_model->getPlayerPosition().x << ", " << view_model->getPlayerPosition().y << std::endl;
    rect.setPosition(100,100);
}

void Player::render(sf::RenderWindow& window){
    window.draw(rect);
}

void Player::loadTextures() {
    std::cout << "Loading player textures..." << std::endl;
    // Load textures for different player states
    try {
        sf::Texture idle_r_texture;
        idle_r_texture.loadFromFile("assets/images/player/player_idle_r.png");
        texture_map[PlayerState::IDLE_R] = idle_r_texture;
        sf::Texture walk_r_texture;
        walk_r_texture.loadFromFile("assets/images/player/player_walk_r.png");
        texture_map[PlayerState::WALKING_R] = walk_r_texture;
    } catch (const std::exception& e) {
        std::cout << "Error loading player textures: " << e.what() << std::endl;
    }
}