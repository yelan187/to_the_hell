#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

namespace ViewModel {
    class GameViewModel;
}

namespace View{
namespace UI {

enum class PickupType {
    NORMAL_DOT,    // 普通豆子（圆形）
    STAR_DOT       // 五角星豆子
};

class Pickup {
public:
    void init(std::shared_ptr<ViewModel::GameViewModel> view_model, int id);
    void render(sf::RenderWindow& window);
    void update(float delta_time);

private:
    int id;
    std::shared_ptr<ViewModel::GameViewModel> view_model;
    sf::Vector2f size;
    sf::CircleShape circle_shape;   // 用于圆形豆子
    sf::CircleShape star_shape;     // 用于五角星豆子
    PickupType current_type;
    
    void createStarShape();
};

}
}
