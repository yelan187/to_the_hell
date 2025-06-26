#include <SFML/Graphics.hpp>
#include "ViewModel/GameViewModel.h"
#include <map>
namespace View{
namespace UI {

class Platform {
public:
    enum class Type {
        NORMAL
    };
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