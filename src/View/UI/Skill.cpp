#include "View/UI/Skill.h"
#include <iostream>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace View::UI;

Skill::Skill(SkillType type, sf::Vector2f position, sf::Vector2f size) 
    : type(type), position(position), size(size), skill_texture(nullptr), is_available(true), cooldown_progress(0.0f) {
    
    // 设置背景圆圈
    background.setRadius(size.x / 2);
    background.setPosition(position);
    background.setFillColor(sf::Color(100, 100, 100, 200)); // 半透明灰色
    background.setOutlineThickness(2.0f);
    background.setOutlineColor(sf::Color::White);
    
    // 设置冷却遮罩
    cooldown_overlay.setRadius(size.x / 2);
    cooldown_overlay.setPosition(position);
    cooldown_overlay.setFillColor(sf::Color(0, 0, 0, 150)); // 半透明黑色
    
    loadTexture();
    createProgrammaticIcon();
}

void Skill::loadTexture() {
    skill_texture = new sf::Texture();
    bool loaded = false;
    
    switch (type) {
        case SkillType::ARROW_SHOT:
            loaded = skill_texture->loadFromFile("assets/images/skill/wide-arrow-dunk.png");
            break;
        case SkillType::SPRINT:
            loaded = skill_texture->loadFromFile("assets/images/skill/sprint.png");
            break;
    }
    
    if (loaded) {
        skill_sprite.setTexture(*skill_texture);
        
        // 缩放纹理以适应技能图标大小
        sf::Vector2u texture_size = skill_texture->getSize();
        float scale_x = (size.x - 10.0f) / texture_size.x; // 留10像素边距
        float scale_y = (size.y - 10.0f) / texture_size.y;
        float scale = std::min(scale_x, scale_y);
        
        skill_sprite.setScale(scale, scale);
        
        // 居中对齐
        sf::Vector2f sprite_size(texture_size.x * scale, texture_size.y * scale);
        skill_sprite.setPosition(
            position.x + (size.x - sprite_size.x) / 2,
            position.y + (size.y - sprite_size.y) / 2
        );
    } else {
        std::cout << "Warning: Could not load skill texture, using programmatic icon" << std::endl;
        delete skill_texture;
        skill_texture = nullptr;
    }
}

void Skill::createProgrammaticIcon() {
    sf::Vector2f center(position.x + size.x / 2, position.y + size.y / 2);
    
    switch (type) {
        case SkillType::ARROW_SHOT:
        {
            // 创建箭头形状
            arrow_shape.setPointCount(7);
            float arrow_size = size.x * 0.3f;
            
            // 箭头的7个点（箭头指向右）
            arrow_shape.setPoint(0, sf::Vector2f(center.x + arrow_size, center.y)); // 箭头尖端
            arrow_shape.setPoint(1, sf::Vector2f(center.x, center.y - arrow_size * 0.3f)); // 上侧
            arrow_shape.setPoint(2, sf::Vector2f(center.x, center.y - arrow_size * 0.15f)); // 上杆
            arrow_shape.setPoint(3, sf::Vector2f(center.x - arrow_size, center.y - arrow_size * 0.15f)); // 左上杆
            arrow_shape.setPoint(4, sf::Vector2f(center.x - arrow_size, center.y + arrow_size * 0.15f)); // 左下杆
            arrow_shape.setPoint(5, sf::Vector2f(center.x, center.y + arrow_size * 0.15f)); // 下杆
            arrow_shape.setPoint(6, sf::Vector2f(center.x, center.y + arrow_size * 0.3f)); // 下侧
            
            arrow_shape.setFillColor(sf::Color::Yellow);
            break;
        }
        case SkillType::SPRINT:
        {
            // 创建两个重叠的圆圈表示冲刺
            sprint_shape.setRadius(size.x * 0.15f);
            sprint_shape.setPosition(center.x - size.x * 0.1f, center.y - size.x * 0.15f);
            sprint_shape.setFillColor(sf::Color::Cyan);
            break;
        }
    }
}

void Skill::update(float cooldown_progress, bool is_available) {
    this->cooldown_progress = cooldown_progress;
    this->is_available = is_available;
    
    // 更新背景颜色
    if (is_available) {
        background.setFillColor(sf::Color(100, 100, 100, 200));
        background.setOutlineColor(sf::Color::White);
    } else {
        background.setFillColor(sf::Color(50, 50, 50, 200));
        background.setOutlineColor(sf::Color::Red);
    }
}

void Skill::render(sf::RenderWindow& window) {
    // 绘制背景
    window.draw(background);
    
    // 绘制技能图标
    if (skill_texture) {
        window.draw(skill_sprite);
    } else {
        // 使用程序化图标
        switch (type) {
            case SkillType::ARROW_SHOT:
                window.draw(arrow_shape);
                break;
            case SkillType::SPRINT:
                window.draw(sprint_shape);
                // 绘制第二个圆圈（稍微偏移）
                sf::CircleShape second_circle = sprint_shape;
                second_circle.move(size.x * 0.2f, 0);
                window.draw(second_circle);
                break;
        }
    }
    
    // 绘制扇形冷却遮罩（顺时针消失的阴影）
    if (cooldown_progress > 0.0f) {
        sf::ConvexShape cooldown_sector;
        createCooldownSector(cooldown_sector, cooldown_progress);
        window.draw(cooldown_sector);
    }
}

bool Skill::isClicked(sf::Vector2f mouse_pos) const {
    sf::Vector2f center(position.x + size.x / 2, position.y + size.y / 2);
    float distance = std::sqrt(std::pow(mouse_pos.x - center.x, 2) + std::pow(mouse_pos.y - center.y, 2));
    return distance <= size.x / 2;
}

void Skill::createCooldownSector(sf::ConvexShape& sector, float progress) {
    if (progress <= 0.0f) return;
    
    sf::Vector2f center(position.x + size.x / 2, position.y + size.y / 2);
    float radius = size.x / 2;
    
    // 计算扇形的角度（顺时针，从顶部开始）
    float angle = progress * 360.0f; // 冷却进度对应的角度
    int segments = std::max(3, static_cast<int>(angle / 10.0f)); // 根据角度计算分段数
    
    sector.setPointCount(segments + 2); // 中心点 + 分段点 + 终点
    sector.setPoint(0, center); // 中心点
    
    // 从顶部开始（-90度）顺时针绘制
    for (int i = 0; i <= segments; ++i) {
        float current_angle = -90.0f + (angle * i / segments); // 从顶部开始顺时针
        float radian = current_angle * M_PI / 180.0f;
        sf::Vector2f point(
            center.x + radius * std::cos(radian),
            center.y + radius * std::sin(radian)
        );
        sector.setPoint(i + 1, point);
    }
    
    sector.setFillColor(sf::Color(0, 0, 0, 150)); // 半透明黑色遮罩
}
