#include <View/UI/Bullet.h>
#include <ViewModel/GameViewModel.h>

using View::UI::Bullet;

void Bullet::init(std::shared_ptr<ViewModel::GameViewModel> view_model, int id) {
    // 初始化子弹UI组件，绑定到对应的游戏模型数据
    this->id = id;
    this->view_model = view_model;
    size = view_model->getBulletSize(); // 从游戏模型获取子弹尺寸
    
    // 设置子弹的视觉表现为白色圆形
    shape.setRadius(size.x / 2);           // 半径为宽度的一半
    shape.setFillColor(sf::Color::White);  // 白色填充
    shape.setOrigin(size.x / 2, size.y / 2); // 设置原点为中心，便于定位
}

void Bullet::update(float delta_time) {
    // 每帧更新子弹位置，从游戏模型获取最新位置数据
    shape.setPosition(view_model->getBulletPositionById(id));
}

void Bullet::render(sf::RenderWindow& window) {
    // 渲染子弹到屏幕上
    window.draw(shape);
}
