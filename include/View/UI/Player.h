#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <memory>
#include "Common/NotificationBase.h"
#include "Common/FrameInfo.h"

namespace View{
namespace UI {

class Player {
public:
    Player(sf::RenderWindow& window, sf::Vector2f size): window(window), size(size) {}
    void init();
    void render();
    void update(Common::FrameInfo::PlayerInfo player_info);

    sf::Vector2f size;
    sf::RectangleShape rect;
    
private:
    sf::RenderWindow& window;
};

}
}