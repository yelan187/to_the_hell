#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <memory> 

namespace ViewModel {
    class GameViewModel;
}

namespace View{
namespace UI {

enum class PlatformType {
    NORMAL
};

class Platform {
public:
    void init(std::shared_ptr<ViewModel::GameViewModel> view_model,int id);
    void render(sf::RenderWindow& window);

    int id;
    std::shared_ptr<ViewModel::GameViewModel> view_model;
    sf::Vector2f size;
    std::map<PlatformType,sf::Texture> texture_map;
    sf::RectangleShape rect;
};

}
}