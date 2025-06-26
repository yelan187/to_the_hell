#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include "ViewModel/GameViewModel.h"

namespace View{
namespace UI {

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

    int id;
    std::shared_ptr<ViewModel::GameViewModel> view_model;
    sf::Vector2f size;
    std::map<Type,sf::Texture> texture_map;
    sf::RectangleShape rect;
};

}
}