#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <memory>
#include "Common/ChoicesInfo.h"
#include "Common/Config/GameConfig.h"

namespace View{
namespace UI {

class Choice {
public:
    Choice(sf::RenderWindow& window,int index): window(window),index(index) {}
    void init();
    void render();
    void update(Common::ChoiceInfo info);

    int index;
    sf::RectangleShape rect;
    sf::Text name;
    sf::Text description;
    sf::Sprite icon_sprite;
    
private:
    sf::Font font;
    sf::Texture icon_texture;
    sf::RenderWindow& window;
};

}
}