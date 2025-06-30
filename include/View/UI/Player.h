#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <memory>
#include "Common/NotificationBase.h"

namespace View{
namespace UI {

class Player {
public:
    Player(sf::RenderWindow& window): window(window) {}
    void init();
    void render();
    void update(Common::FrameInfo::PlayerInfo player_info);

    sf::Vector2f size;
    std::map<Utils::GameViewModelPlayerState,sf::Texture> texture_map;
    sf::RectangleShape rect;
private:
    void loadTextures();
    void setTexture(Utils::GameViewModelPlayerState state);
    sf::RenderWindow& window;
};

}
}