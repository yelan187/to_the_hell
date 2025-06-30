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
    int id;
    sf::RenderWindow& window;
    sf::CircleShape normal_shape;    // 圆形豆子
    sf::CircleShape star_shape;      // 五角星豆子（暂用圆形，可扩展为星形）
    sf::Vector2f position;
    sf::Vector2f size;
    int pickup_type;
};

}
}
