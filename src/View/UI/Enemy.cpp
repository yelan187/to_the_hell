#include "View/UI/Enemy.h"
#include <iostream>

using namespace View::UI;

// Enemy静态成员定义
sf::Texture Enemy::ghost_texture;
bool Enemy::texture_loaded = false;

// Enemy实现
Enemy::Enemy(int id, sf::RenderWindow& window) : id(id), window(window) {
    // 加载纹理（只加载一次）
    if (!texture_loaded) {
        loadTextures();
        texture_loaded = true;
    }
    
    // 设置精灵纹理
    if (ghost_texture.getSize().x > 0) {
        sprite.setTexture(ghost_texture);
    } else {
        shape.setFillColor(sf::Color::Red);
    }
    
    facing_direction = sf::Vector2f(1.0f, 0.0f);  // 默认面向右侧
}

void Enemy::loadTextures() {
    if (!ghost_texture.loadFromFile("assets/images/enemy/ghost.png")) {
        std::cout << "Warning: Could not load enemy texture, using red rectangle instead" << std::endl;
    }
}

void Enemy::update(const Common::FrameInfo::EnemyInfo& info) {
    position = info.position;
    size = info.size;
    facing_direction = info.facing_direction;  // 更新面向方向
    
    shape.setSize(size);
    shape.setPosition(position);
    
    // 更新精灵位置和缩放
    if (ghost_texture.getSize().x > 0) {
        sprite.setPosition(position);
        
        // 根据敌人大小缩放精灵
        sf::Vector2u texture_size = ghost_texture.getSize();
        float scale_x = size.x / texture_size.x;
        float scale_y = size.y / texture_size.y;
        
        // 如果面向左侧，水平镜像
        if (facing_direction.x < 0) {
            sprite.setScale(-scale_x, scale_y);  // 负值缩放实现镜像
            sprite.setPosition(position.x + size.x, position.y);  // 调整位置
        } else {
            sprite.setScale(scale_x, scale_y);
        }
    }
}

void Enemy::render() {
    // 如果有纹理，绘制精灵；否则绘制形状
    if (ghost_texture.getSize().x > 0) {
        window.draw(sprite);
    } else {
        window.draw(shape);
    }
}
