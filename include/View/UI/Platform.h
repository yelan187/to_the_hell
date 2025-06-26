#pragma once
#include <SFML/Graphics.hpp>
#include "ViewModel/GameViewModel.h"
#include <map>
namespace View{
namespace UI {

// 为了与前向声明匹配
namespace PlatformType {
    enum class Type {
        NORMAL
    };
}

class Platform {
public:
    using Type = PlatformType::Type;
    void init(std::shared_ptr<ViewModel::GameViewModel> view_model,int id);
    void render(sf::RenderWindow& window);
    void update(float delta_time);

    int id;
    std::shared_ptr<ViewModel::GameViewModel> view_model;
    sf::Vector2f size;
    std::map<Type,sf::Texture> texture_map;
    sf::RectangleShape rect;
};

}
}