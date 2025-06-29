#pragma once
#include <SFML/Graphics.hpp>

namespace Model {
    class GameModel;
}

namespace Model {
namespace Entities {

enum class PickupType {
    NORMAL_DOT,    // 普通豆子（圆形）
    STAR_DOT       // 五角星豆子
};

class Pickup {
public:
    Pickup(int id, PickupType type, sf::Vector2f position, sf::Vector2f size, GameModel* game_model, int platform_id = -1);
    
    void update(float delta_time);
    bool outOfWindow(sf::Vector2u window_size) const;
    bool collidesWith(sf::Vector2f target_pos, sf::Vector2f target_size) const;
    
    sf::Vector2f getPosition() const { return position; }
    sf::Vector2f getSize() const { return size; }
    PickupType getType() const { return type; }
    int getId() const { return id; }
    int getScore() const;
    bool isOnPlatform() const { return platform_id != -1; }
    int getPlatformId() const { return platform_id; }

private:
    int id;
    PickupType type;
    sf::Vector2f position;
    sf::Vector2f size;
    sf::Vector2f velocity;
    GameModel* game_model;
    int platform_id; // -1表示不在平台上
    float scroll_speed;
    
    void updateMovement(float delta_time);
};

}
}
