#include <SFML/Graphics.hpp>
#include <ViewModel/GameViewModel.h>
#include <map>

namespace View{
namespace UI {

class Player {
public:
    enum class State {
        WALKING
    };

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