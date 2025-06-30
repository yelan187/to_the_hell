#pragma once
#include <SFML/Graphics.hpp>
#include "Common/NotificationBase.h"

namespace View {
namespace UI {

class Bullet {
public:
    Bullet(int id, sf::RenderWindow& window);
    void update(const Common::FrameInfo::BulletInfo& info);
    void render();

private:
    int id;
    sf::RenderWindow& window;
    sf::CircleShape circle_shape;     // 用于敌人子弹
    sf::ConvexShape arrow_shape;      // 用于玩家箭矢
    sf::Vector2f position;
    sf::Vector2f size;
    bool is_player_bullet;
    
    void createArrowShape();
};

}
}
