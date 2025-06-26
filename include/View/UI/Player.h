#pragma once
#include <SFML/Graphics.hpp>
#include <ViewModel/GameViewModel.h>
#include <map>

namespace View{
namespace UI {

// 为了与前向声明匹配
namespace PlayerState {
    enum class State {
        WALKING
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