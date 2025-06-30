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
    void render();

private:
    int id;
    sf::RenderWindow& window;
    sf::RectangleShape rect;
};

}
}