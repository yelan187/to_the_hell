#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <map>

namespace View {
namespace UI {

/**
 * Animation UI组件 - 用于渲染sprite动画
 * 处理动画纹理加载和帧渲染
 */
class Animation {
public:
    Animation(sf::RenderWindow& window);
    ~Animation();

    void loadTexture(const std::string& texture_path);
    void render(sf::Vector2f position, sf::Vector2f size, sf::IntRect frame_rect);
    void render(sf::Vector2f position, sf::Vector2f size, sf::IntRect frame_rect, float alpha);
    void render(sf::Vector2f position, sf::Vector2f size, sf::IntRect frame_rect, float alpha, const std::string& texture_path);

private:
    sf::RenderWindow& window;
    std::map<std::string, sf::Texture> texture_cache; // 纹理缓存
    sf::Sprite sprite;
    std::string current_texture_path; // 当前使用的纹理路径
};

}
}
