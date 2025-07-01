#pragma once
#include <SFML/Graphics.hpp>
#include "Common/NotificationBase.h"

namespace View {
namespace UI {

class Pickup {
public:
    Pickup(int id, sf::RenderWindow& window);
    void update(const Common::FrameInfo::PickupInfo& info);
    void render();

private:
    // 豆子类型常量定义（避免依赖Model层枚举）
    static const int NORMAL_DOT_TYPE = 0;
    static const int STAR_DOT_TYPE = 1;
    
    int id;
    sf::RenderWindow& window;
    sf::CircleShape normal_shape;    // 圆形豆子
    sf::ConvexShape star_shape;      // 五角星豆子
    sf::Vector2f position;
    sf::Vector2f size;
    int pickup_type;
    
    void createStarShape(sf::Vector2f center, float radius);
};

}
}
