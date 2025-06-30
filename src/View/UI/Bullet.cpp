#include "View/UI/Bullet.h"
#include <iostream>

using namespace View::UI;

// Bullet实现
Bullet::Bullet(int id, sf::RenderWindow& window) : id(id), window(window), is_player_bullet(false) {
    circle_shape.setFillColor(sf::Color::White);
}

void Bullet::createArrowShape() {
    // 创建箭矢形状（更长的箭形）
    arrow_shape.setPointCount(7);
    
    float width = size.x;
    float height = size.y;
    float arrow_length = width * 2.0f;  // 箭矢比原始size更长
    float arrow_width = height;
    
    // 箭头的7个点（箭头指向右）
    arrow_shape.setPoint(0, sf::Vector2f(position.x + arrow_length, position.y + arrow_width / 2)); // 箭头尖端
    arrow_shape.setPoint(1, sf::Vector2f(position.x + arrow_length * 0.7f, position.y)); // 上侧
    arrow_shape.setPoint(2, sf::Vector2f(position.x + arrow_length * 0.7f, position.y + arrow_width * 0.3f)); // 上杆
    arrow_shape.setPoint(3, sf::Vector2f(position.x, position.y + arrow_width * 0.3f)); // 左上杆
    arrow_shape.setPoint(4, sf::Vector2f(position.x, position.y + arrow_width * 0.7f)); // 左下杆
    arrow_shape.setPoint(5, sf::Vector2f(position.x + arrow_length * 0.7f, position.y + arrow_width * 0.7f)); // 下杆
    arrow_shape.setPoint(6, sf::Vector2f(position.x + arrow_length * 0.7f, position.y + arrow_width)); // 下侧
    
    arrow_shape.setFillColor(sf::Color::Yellow);
}

void Bullet::update(const Common::FrameInfo::BulletInfo& info) {
    position = info.position;
    size = info.size;
    is_player_bullet = info.is_player_bullet;
    
    if (is_player_bullet) {
        // 玩家箭矢：创建箭形
        createArrowShape();
    } else {
        // 敌人子弹：使用圆形
        circle_shape.setRadius(size.x / 2);
        circle_shape.setPosition(position);
        circle_shape.setFillColor(info.color);
    }
}

void Bullet::render() {
    if (is_player_bullet) {
        window.draw(arrow_shape);
    } else {
        window.draw(circle_shape);
    }
}
