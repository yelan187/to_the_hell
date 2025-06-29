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
    NORMAL,     // 普通平台
    SPIKED,     // 带刺平台（踩上即死）
    ROLLING,    // 滚动平台（左右滚动）
    FRAGILE,    // 脆弱平台（踩到后几秒破碎）
    BOUNCY      // 弹跳平台（踩到后给人物施加向上速度）
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
    sf::CircleShape ellipse; // 用于弹跳平台的椭圆形状
};

}
}