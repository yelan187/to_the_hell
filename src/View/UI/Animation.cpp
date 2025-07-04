#include "View/UI/Animation.h"
#include <iostream>

using namespace View::UI;

Animation::Animation(sf::RenderWindow& window) 
    : window(window), current_texture_path("") {
}

Animation::~Animation() {
}

void Animation::loadTexture(const std::string& texture_path) {
    // 检查是否已经缓存了这个纹理
    if (texture_cache.find(texture_path) == texture_cache.end()) {
        sf::Texture texture;
        if (!texture.loadFromFile(texture_path)) {
            std::cerr << "Failed to load animation texture: " << texture_path << std::endl;
            return;
        }
        texture_cache[texture_path] = texture;
    }
    
    current_texture_path = texture_path;
    sprite.setTexture(texture_cache[texture_path]);
}

void Animation::render(sf::Vector2f position, sf::Vector2f size, sf::IntRect frame_rect) {
    render(position, size, frame_rect, 255.0f);
}

void Animation::render(sf::Vector2f position, sf::Vector2f size, sf::IntRect frame_rect, float alpha) {
    render(position, size, frame_rect, alpha, current_texture_path);
}

void Animation::render(sf::Vector2f position, sf::Vector2f size, sf::IntRect frame_rect, float alpha, const std::string& texture_path) {
    // 如果指定了纹理路径且该纹理已缓存，使用指定的纹理
    if (!texture_path.empty() && texture_cache.find(texture_path) != texture_cache.end()) {
        sprite.setTexture(texture_cache[texture_path]);
    } else if (current_texture_path.empty() || texture_cache.find(current_texture_path) == texture_cache.end()) {
        return;
    }
    
    sprite.setTextureRect(frame_rect);
    sprite.setPosition(position);
    
    // 设置缩放以匹配所需大小
    float scale_x = size.x / frame_rect.width;
    float scale_y = size.y / frame_rect.height;
    sprite.setScale(scale_x, scale_y);
    
    // 设置透明度
    sf::Color color = sprite.getColor();
    color.a = static_cast<sf::Uint8>(alpha);
    sprite.setColor(color);
    
    window.draw(sprite);
}
