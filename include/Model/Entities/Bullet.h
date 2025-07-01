#pragma once
#include <SFML/Graphics.hpp>

namespace Model {
namespace Entities {

class Bullet {
public:
    Bullet(int id, sf::Vector2f position, sf::Vector2f velocity, sf::Vector2f size, bool is_player_bullet = false);
    
    void update(float delta_time);
    void update(float delta_time, float scroll_speed);  // 新增：支持滚动速度补偿
    bool outOfWindow(sf::Vector2u window_size) const;
    bool collidesWith(sf::Vector2f target_pos, sf::Vector2f target_size) const;
    
    sf::Vector2f getPosition() const { return position; }
    sf::Vector2f getSize() const { return size; }
    sf::Vector2f getVelocity() const { return velocity; }
    int getId() const { return id; }
    bool isPlayerBullet() const { return is_player_bullet; }

private:
    int id;
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Vector2f size;
    bool is_player_bullet;
};

}
}
