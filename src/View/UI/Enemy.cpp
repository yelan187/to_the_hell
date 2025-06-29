#include <iostream>
#include <vector>
#include <View/UI/Enemy.h>
#include <ViewModel/GameViewModel.h>

using View::UI::Enemy;

// 静态成员定义
std::map<View::UI::EnemyType, sf::Texture> Enemy::texture_map;
bool Enemy::textures_loaded = false;

void Enemy::init(std::shared_ptr<ViewModel::GameViewModel> view_model, int id) {
    this->id = id;
    this->view_model = view_model;
    size = view_model->getEnemySize();
    rect.setSize(size);
    
    // 初始化位置和方向
    last_position = view_model->getEnemyPositionById(id);
    facing_left = false; // 默认面向右
    
    // 只加载一次纹理
    if (!textures_loaded) {
        loadTextures();
        textures_loaded = true;
    }
    
    setTexture(view_model->getEnemyTypeById(id));
}

void Enemy::update(float delta_time) {
    sf::Vector2f current_position = view_model->getEnemyPositionById(id);
    
    // 检测移动方向
    float dx = current_position.x - last_position.x;
    if (dx < -0.1f) { // 向左移动
        facing_left = true;
    } else if (dx > 0.1f) { // 向右移动
        facing_left = false;
    }
    // dx接近0时保持当前方向
    
    rect.setPosition(current_position);
    last_position = current_position;
    
    // 根据方向设置纹理变换
    updateTextureTransform();
}

void Enemy::render(sf::RenderWindow& window) {
    window.draw(rect);
}

void Enemy::loadTextures() {
    if (!texture_map[EnemyType::GHOST].loadFromFile("assets/images/enemy/ghost.png")) {
        std::cout << "Error loading ghost.png" << std::endl;
    }
}

void Enemy::setTexture(EnemyType type) {
    switch (type) {
        case EnemyType::GHOST:
            if (texture_map[EnemyType::GHOST].getSize().x > 0) {
                rect.setTexture(&texture_map[EnemyType::GHOST]);
                // 初始化纹理矩形
                updateTextureTransform();
            } else {
                rect.setFillColor(sf::Color::Red); // 只在纹理失败时设置红色
            }
            break;
        default:
            rect.setFillColor(sf::Color::Red); // 未知类型使用红色
            break;
    }
}

void Enemy::updateTextureTransform() {
    if (facing_left) {
        // 镜像纹理 - 水平翻转
        sf::Vector2u textureSize = texture_map[EnemyType::GHOST].getSize();
        rect.setTextureRect(sf::IntRect(textureSize.x, 0, -static_cast<int>(textureSize.x), textureSize.y));
    } else {
        // 正常显示
        sf::Vector2u textureSize = texture_map[EnemyType::GHOST].getSize();
        rect.setTextureRect(sf::IntRect(0, 0, textureSize.x, textureSize.y));
    }
}
