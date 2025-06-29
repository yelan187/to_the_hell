#include <View/UI/Pickup.h>
#include <ViewModel/GameViewModel.h>
#include <cmath>

using View::UI::Pickup;

void Pickup::init(std::shared_ptr<ViewModel::GameViewModel> view_model, int id) {
    this->id = id;
    this->view_model = view_model;
    size = view_model->getPickupSize();
    current_type = view_model->getPickupTypeById(id);
    
    // 设置圆形豆子 - 稍微大一点
    circle_shape.setRadius(size.x / 2);
    circle_shape.setFillColor(sf::Color::Yellow);
    circle_shape.setOrigin(size.x / 2, size.y / 2);
    
    // 设置五角星豆子 - 更明显的星形
    star_shape.setRadius(size.x / 2.2f); // 稍微小一点让星形更明显
    star_shape.setPointCount(5);
    star_shape.setFillColor(sf::Color(255, 215, 0)); // 金黄色，更亮一些
    star_shape.setOrigin(size.x / 2, size.y / 2);
}

void Pickup::update(float delta_time) {
    sf::Vector2f position = view_model->getPickupPositionById(id);
    position.x += size.x / 2; // 调整到中心点
    position.y += size.y / 2;
    
    circle_shape.setPosition(position);
    star_shape.setPosition(position);
}

void Pickup::render(sf::RenderWindow& window) {
    switch (current_type) {
        case PickupType::NORMAL_DOT:
            window.draw(circle_shape);
            break;
        case PickupType::STAR_DOT:
            window.draw(star_shape);
            break;
    }
}
