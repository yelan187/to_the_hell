#include <View/UI/Bullet.h>
#include <ViewModel/GameViewModel.h>

using View::UI::Bullet;

void Bullet::init(std::shared_ptr<ViewModel::GameViewModel> view_model, int id) {
    this->id = id;
    this->view_model = view_model;
    size = view_model->getBulletSize();
    
    shape.setRadius(size.x / 2);
    shape.setFillColor(sf::Color::White);
    shape.setOrigin(size.x / 2, size.y / 2);
}

void Bullet::update(float delta_time) {
    shape.setPosition(view_model->getBulletPositionById(id));
}

void Bullet::render(sf::RenderWindow& window) {
    window.draw(shape);
}
