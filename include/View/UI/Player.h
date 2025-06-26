#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include "ViewModel/GameViewModel.h"

namespace View{
namespace UI {

namespace PlayerState {
    enum class State {
        IDLE,
        WALKING,
        JUMPING
    };
}

class Player {
public:
    using State = PlayerState::State;

    void init(std::shared_ptr<ViewModel::GameViewModel> view_model);
    void render(sf::RenderWindow& window);
    void update(float delta_time);

    std::shared_ptr<ViewModel::GameViewModel> view_model;
    sf::Vector2f size;
    std::map<State,sf::Texture> texture_map;
    sf::RectangleShape rect;
};

}
}