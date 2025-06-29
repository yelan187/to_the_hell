#include "View/UI/Arrow.h"

using View::UI::Arrow;

Arrow::Arrow() : facing_right(true) {
    arrow_shape.setFillColor(sf::Color::White);
    arrow_shape.setOutlineColor(sf::Color(200, 200, 200));
    arrow_shape.setOutlineThickness(1.0f);
}

void Arrow::setPosition(sf::Vector2f new_position) {
    position = new_position;
    updateArrowShape();
}

void Arrow::setSize(sf::Vector2f new_size) {
    size = new_size;
    updateArrowShape();
}

void Arrow::setFacingRight(bool right) {
    facing_right = right;
    updateArrowShape();
}

void Arrow::updateArrowShape() {
    // 创建箭矢形状：箭头 + 箭身 + 尾羽
    arrow_shape.setPointCount(7);
    
    if (facing_right) {
        // 向右的箭矢
        float arrow_head_width = size.x * 0.3f;  // 箭头宽度
        float arrow_body_width = size.x * 0.6f;  // 箭身宽度
        float arrow_tail_width = size.x * 0.1f;  // 尾羽宽度
        float arrow_height = size.y;
        
        // 从箭头尖端开始，顺时针设置点
        arrow_shape.setPoint(0, sf::Vector2f(position.x + size.x, position.y + arrow_height / 2)); // 箭头尖端
        arrow_shape.setPoint(1, sf::Vector2f(position.x + size.x - arrow_head_width, position.y)); // 箭头上边
        arrow_shape.setPoint(2, sf::Vector2f(position.x + size.x - arrow_head_width, position.y + arrow_height * 0.3f)); // 箭身上边
        arrow_shape.setPoint(3, sf::Vector2f(position.x + arrow_tail_width, position.y + arrow_height * 0.3f)); // 尾羽连接处上
        arrow_shape.setPoint(4, sf::Vector2f(position.x, position.y + arrow_height / 2)); // 尾羽尖端
        arrow_shape.setPoint(5, sf::Vector2f(position.x + arrow_tail_width, position.y + arrow_height * 0.7f)); // 尾羽连接处下
        arrow_shape.setPoint(6, sf::Vector2f(position.x + size.x - arrow_head_width, position.y + arrow_height * 0.7f)); // 箭身下边
    } else {
        // 向左的箭矢（镜像）
        float arrow_head_width = size.x * 0.3f;
        float arrow_body_width = size.x * 0.6f;
        float arrow_tail_width = size.x * 0.1f;
        float arrow_height = size.y;
        
        // 从箭头尖端开始，顺时针设置点
        arrow_shape.setPoint(0, sf::Vector2f(position.x, position.y + arrow_height / 2)); // 箭头尖端
        arrow_shape.setPoint(1, sf::Vector2f(position.x + arrow_head_width, position.y + arrow_height * 0.7f)); // 箭身下边
        arrow_shape.setPoint(2, sf::Vector2f(position.x + size.x - arrow_tail_width, position.y + arrow_height * 0.7f)); // 尾羽连接处下
        arrow_shape.setPoint(3, sf::Vector2f(position.x + size.x, position.y + arrow_height / 2)); // 尾羽尖端
        arrow_shape.setPoint(4, sf::Vector2f(position.x + size.x - arrow_tail_width, position.y + arrow_height * 0.3f)); // 尾羽连接处上
        arrow_shape.setPoint(5, sf::Vector2f(position.x + arrow_head_width, position.y + arrow_height * 0.3f)); // 箭身上边
        arrow_shape.setPoint(6, sf::Vector2f(position.x + arrow_head_width, position.y)); // 箭头上边
    }
}

void Arrow::draw(sf::RenderWindow& window) const {
    window.draw(arrow_shape);
}
