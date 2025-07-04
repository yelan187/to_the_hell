#include <iostream>
#include <algorithm>
#include <string>
#include "View/UI/Choice.h"

using View::UI::Choice;

void Choice::init() {
    if (!font.loadFromFile("assets/fonts/fusion.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
        return;
    }
    sf::Vector2u window_size = window.getSize();
    rect.setSize(sf::Vector2f(window_size.x * 0.25f, window_size.y * 0.6f));
    rect.setFillColor(sf::Color(0, 0, 0, 180)); // 半透明黑色背景
    rect.setOutlineThickness(3.0f);
    rect.setOutlineColor(sf::Color::White); // 白色边框
    rect.setOrigin(rect.getSize().x / 2, rect.getSize().y / 2);
    rect.setPosition((window_size.x * 0.25f / 4)*(index + 1) + (rect.getSize().x / 2)*(index * 2 + 1), window_size.y / 2.0f);

    name.setCharacterSize(36);
    name.setFillColor(sf::Color::White);
    name.setFont(font);

    description.setCharacterSize(26);
    description.setFillColor(sf::Color::White);
    description.setFont(font);
}

void Choice::update(Common::ChoiceInfo info) {
    name.setString(info.name);
    name.setOrigin(name.getLocalBounds().width / 2, name.getLocalBounds().height / 2);
    name.setPosition(rect.getPosition().x, rect.getPosition().y - rect.getSize().y / 2 + 40);

    description.setString(info.description);
    
    // 处理文字换行 - 支持最多三行显示
    std::string desc_text = info.description;
    float max_width = rect.getSize().x * 0.9f; // 选择框宽度的90%

    description.setString(desc_text);
    if (description.getLocalBounds().width > max_width) {
        // 将文字分成三段进行换行
        size_t text_len = desc_text.length();
        size_t first_break = text_len / 3;
        size_t second_break = text_len * 2 / 3;
        
        // 寻找第一个换行位置
        size_t break_pos1 = desc_text.find_last_of(" .,，。", first_break + 8);
        if (break_pos1 == std::string::npos || break_pos1 < first_break - 8) {
            break_pos1 = desc_text.find_first_of(" .,，。", first_break - 8);
        }
        
        if (break_pos1 != std::string::npos && break_pos1 < text_len - 10) {
            desc_text.insert(break_pos1 + 1, "\n");
            
            // 寻找第二个换行位置（需要考虑第一个换行符的偏移）
            size_t break_pos2 = desc_text.find_last_of(" .,，。", second_break + 10);
            if (break_pos2 == std::string::npos || break_pos2 < second_break - 10 || break_pos2 <= break_pos1 + 5) {
                break_pos2 = desc_text.find_first_of(" .,，。", second_break - 5);
            }
            
            if (break_pos2 != std::string::npos && break_pos2 > break_pos1 + 5 && break_pos2 < desc_text.length() - 5) {
                desc_text.insert(break_pos2 + 1, "\n");
            }
        }
        
        description.setString(desc_text);
    }
    
    description.setOrigin(description.getLocalBounds().width / 2, description.getLocalBounds().height / 2);
    description.setPosition(rect.getPosition().x, rect.getPosition().y + rect.getSize().y / 2 - 50);

    // 加载并设置图标
    if (!info.icon_path.empty() && icon_texture.loadFromFile(info.icon_path)) {
        icon_sprite.setTexture(icon_texture);
        
        // 计算图标缩放比例，使其适合选择框中间区域
        sf::Vector2u texture_size = icon_texture.getSize();
        float max_icon_width = rect.getSize().x * 0.6f;  // 选择框宽度的60%
        float max_icon_height = rect.getSize().y * 0.4f; // 选择框高度的40%
        
        float scale_x = max_icon_width / texture_size.x;
        float scale_y = max_icon_height / texture_size.y;
        float scale = std::min(scale_x, scale_y); // 保持纵横比
        
        icon_sprite.setScale(scale, scale);
        
        // 设置图标位置（选择框中心）
        sf::FloatRect icon_bounds = icon_sprite.getLocalBounds();
        icon_sprite.setOrigin(icon_bounds.width / 2, icon_bounds.height / 2);
        icon_sprite.setPosition(rect.getPosition().x, rect.getPosition().y);
    }
}

void Choice::render(){
    window.draw(rect);
    window.draw(icon_sprite);  // 绘制图标
    window.draw(name);
    window.draw(description);
}