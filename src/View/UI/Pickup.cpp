#include "View/UI/Pickup.h"
#include <iostream>
#include <cmath>

using namespace View::UI;

// Pickup实现
Pickup::Pickup(int id, sf::RenderWindow& window) : id(id), window(window) {
    normal_shape.setFillColor(sf::Color::Yellow);
    star_shape.setFillColor(sf::Color(255, 215, 0)); // 金色五角星
    star_shape.setPointCount(10); // 五角星需要10个点（5个外点 + 5个内点）
}

void Pickup::createStarShape(sf::Vector2f center, float radius) {
    const float PI = 3.14159265f;
    const int points = 5;
    const float innerRadius = radius * 0.4f; // 内半径为外半径的40%
    
    star_shape.setPointCount(points * 2);
    
    for (int i = 0; i < points * 2; ++i) {
        float angle = (i * PI / points) - PI / 2; // 从顶部开始
        float currentRadius = (i % 2 == 0) ? radius : innerRadius;
        
        float x = center.x + std::cos(angle) * currentRadius;
        float y = center.y + std::sin(angle) * currentRadius;
        
        star_shape.setPoint(i, sf::Vector2f(x, y));
    }
}

void Pickup::update(const Common::FrameInfo::PickupInfo& info) {
    position = info.position;
    size = info.size;
    pickup_type = info.pickup_type;
    
    if (pickup_type == 0) { // 普通圆形豆子
        normal_shape.setRadius(size.x / 2);
        normal_shape.setPosition(position);
    } else { // 五角星豆子
        sf::Vector2f center = sf::Vector2f(position.x + size.x / 2, position.y + size.y / 2);
        createStarShape(center, size.x / 2);
    }
}

void Pickup::render() {
    if (pickup_type == 0) {
        window.draw(normal_shape);
    } else {
        window.draw(star_shape);
    }
}
