#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

namespace ViewModel {
    class GameViewModel;
}

namespace View{
namespace UI {

class Bullet {
public:
    void init(std::shared_ptr<ViewModel::GameViewModel> view_model, int id);
    void render(sf::RenderWindow& window);
    void update(float delta_time);

private:
    int id;
    std::shared_ptr<ViewModel::GameViewModel> view_model;
    sf::Vector2f size;
    sf::CircleShape shape;
};

}
}
