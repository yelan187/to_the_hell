#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <memory> 
#include "Utils/Config.h"
#include "Common/NotificationBase.h"

namespace View{
namespace UI {


class Platform {
public:
    Platform(int id, sf::RenderWindow& window) : id(id), window(window) {}
    void update(Common::FrameInfo::PlatformInfo platform_info);
    void init();
    void render();

private:
    int id;
    sf::RenderWindow& window;
    sf::Vector2f size;
    std::map<Utils::PlatformType,sf::Texture> texture_map;
    sf::RectangleShape rect;
};

}
}