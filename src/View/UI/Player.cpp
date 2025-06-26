#include <View/UI/Player.h>
#include <ViewModel/GameViewModel.h>

using View::UI::Player;

void Player::init(std::shared_ptr<ViewModel::GameViewModel> view_model) {
    size = sf::Vector2f(50, 50);

    switch (view_model->getPlayerState()) {
        case View::UI::PlayerState::WALKING:
            break;
    }

    rect.setPosition(view_model->getPlayerPosition());
    rect.setSize(size);
}

void Player::update(float delta_time) {
    switch (view_model->getPlayerState()) {
        case View::UI::PlayerState::WALKING:
            break;
    }
    rect.setPosition(view_model->getPlayerPosition());
}

void Player::render(sf::RenderWindow& window){
    window.draw(rect);
}