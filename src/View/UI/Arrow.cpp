#include "View/UI/Arrow.h"

using View::UI::Arrow;

Arrow::Arrow() : facing_right(true) {
    // 初始化箭矢UI的默认外观：白色填充，浅灰色边框
    arrow_shape.setFillColor(sf::Color::White);
    arrow_shape.setOutlineColor(sf::Color(200, 200, 200));
    arrow_shape.setOutlineThickness(1.0f);
}

void Arrow::setPosition(sf::Vector2f new_position) {
    // 设置箭矢位置并更新形状
    position = new_position;
    updateArrowShape();
}

void Arrow::setSize(sf::Vector2f new_size) {
    // 设置箭矢尺寸并更新形状
    size = new_size;
    updateArrowShape();
}

void Arrow::setFacingRight(bool right) {
    // 设置箭矢朝向（左或右）并更新形状
    facing_right = right;
    updateArrowShape();
}

void Arrow::updateArrowShape() {
    // 创建箭矢的几何形状：由箭头、箭身和尾羽三部分组成
    // 使用7个顶点来构造箭矢的轮廓
    arrow_shape.setPointCount(7);
    
    if (facing_right) {
        // 构造向右飞行的箭矢形状
        float arrow_head_width = size.x * 0.3f;  // 箭头部分占总长度30%
        float arrow_body_width = size.x * 0.6f;  // 箭身部分占总长度60%
        float arrow_tail_width = size.x * 0.1f;  // 尾羽部分占总长度10%
        float arrow_height = size.y;
        
        // 按顺时针方向设置箭矢的7个关键顶点
        arrow_shape.setPoint(0, sf::Vector2f(position.x + size.x, position.y + arrow_height / 2)); // 箭头尖端
        arrow_shape.setPoint(1, sf::Vector2f(position.x + size.x - arrow_head_width, position.y)); // 箭头上边
        arrow_shape.setPoint(2, sf::Vector2f(position.x + size.x - arrow_head_width, position.y + arrow_height * 0.3f)); // 箭身上边
        arrow_shape.setPoint(3, sf::Vector2f(position.x + arrow_tail_width, position.y + arrow_height * 0.3f)); // 尾羽连接处上
        arrow_shape.setPoint(4, sf::Vector2f(position.x, position.y + arrow_height / 2)); // 尾羽尖端
        arrow_shape.setPoint(5, sf::Vector2f(position.x + arrow_tail_width, position.y + arrow_height * 0.7f)); // 尾羽连接处下
        arrow_shape.setPoint(6, sf::Vector2f(position.x + size.x - arrow_head_width, position.y + arrow_height * 0.7f)); // 箭身下边
    } else {
        // 构造向左飞行的箭矢形状（水平镜像）
        float arrow_head_width = size.x * 0.3f;
        float arrow_body_width = size.x * 0.6f;
        float arrow_tail_width = size.x * 0.1f;
        float arrow_height = size.y;
        
        // 向左箭矢的顶点顺序：从左侧箭头尖端开始
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
    // 将箭矢图形渲染到窗口
    window.draw(arrow_shape);
}
