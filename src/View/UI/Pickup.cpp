#include "View/UI/Pickup.h"
#include <iostream>

using namespace View::UI;

// Pickup实现
Pickup::Pickup(int id, sf::RenderWindow& window) : id(id), window(window) {
    normal_shape.setFillColor(sf::Color::Yellow);
    star_shape.setFillColor(sf::Color(255, 215, 0)); // 金色五角星
}

void Pickup::update(const Common::FrameInfo::PickupInfo& info) {
    position = info.position;
    size = info.size;
    pickup_type = info.pickup_type;
    
    if (pickup_type == 0) { // 普通圆形豆子
        normal_shape.setRadius(size.x / 2);
        normal_shape.setPosition(position);
    } else { // 五角星豆子（暂用较大圆形表示）
        star_shape.setRadius(size.x / 2);
        star_shape.setPosition(position);
    }
}

void Pickup::render() {
    if (pickup_type == 0) {
        window.draw(normal_shape);
    } else {
        window.draw(star_shape);
    }
}
