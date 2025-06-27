#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <memory>

namespace ViewModel {
    class GameViewModel;
}

namespace View{
namespace UI {

enum class PlayerState {
    IDLE_L,
    IDLE_R,
    WALKING_L,
    WALKING_R,
    JUMPING_L,
    JUMPING_R
};
class Player {
public:
    void init(std::shared_ptr<ViewModel::GameViewModel> view_model);
    void render(sf::RenderWindow& window);
    void update(float delta_time);

    std::shared_ptr<ViewModel::GameViewModel> view_model;
    sf::Vector2f size;
    std::map<PlayerState,sf::Texture> texture_map;
    sf::RectangleShape rect;
private:
    void loadTextures();
    void setTexture(PlayerState state);
};

}
}