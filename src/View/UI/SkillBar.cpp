#include "View/UI/SkillBar.h"
#include <iostream>
#include <algorithm>
#include <cmath>

using View::UI::SkillBar;

SkillBar::SkillBar(sf::Vector2f position) 
    : position(position), skill_icon_size(50.0f, 50.0f), skill_icon_spacing(60.0f), arrow_texture(nullptr) {
    loadTextures();
}

SkillBar::~SkillBar() {
    if (arrow_texture) {
        delete arrow_texture;
    }
}

void SkillBar::loadTextures() {
    arrow_texture = new sf::Texture();
    
    if (!arrow_texture->loadFromFile("assets/images/skill/wide-arrow-dunk.png")) {
        std::cout << "Warning: Failed to load skill texture, using default arrow shape" << std::endl;
        delete arrow_texture;
        arrow_texture = nullptr;
    }
}

void SkillBar::updateSkills(const std::vector<Model::Entities::Skill*>& skills) {
    skill_icons.clear();
    skill_sprites.clear();
    skill_arrows.clear();
    cooldown_overlays.clear();
    
    for (size_t i = 0; i < skills.size(); ++i) {
        Model::Entities::Skill* skill = skills[i];
        
        // 创建技能图标背景
        sf::CircleShape icon(skill_icon_size.x / 2);
        icon.setPosition(position.x, position.y + i * skill_icon_spacing);
        icon.setFillColor(sf::Color(40, 40, 40)); // 深灰色背景
        icon.setOutlineColor(sf::Color::White);
        icon.setOutlineThickness(2.0f);
        skill_icons.push_back(icon);
        
        // 创建技能图标内容
        sf::Sprite sprite;
        sf::ConvexShape arrow_shape;
        
        switch (skill->getType()) {
            case Model::Entities::SkillType::ARROW_SHOT:
                if (arrow_texture) {
                    sprite.setTexture(*arrow_texture);
                    // 缩放纹理以适应图标大小
                    float scale_x = skill_icon_size.x / arrow_texture->getSize().x;
                    float scale_y = skill_icon_size.y / arrow_texture->getSize().y;
                    float scale = std::min(scale_x, scale_y) * 0.8f;
                    sprite.setScale(scale, scale);
                    
                    // 居中显示
                    sf::FloatRect bounds = sprite.getLocalBounds();
                    sprite.setOrigin(bounds.width / 2, bounds.height / 2);
                    sprite.setPosition(
                        position.x + skill_icon_size.x / 2,
                        position.y + i * skill_icon_spacing + skill_icon_size.y / 2
                    );
                } else {
                    // 创建程序化箭头
                    arrow_shape.setPointCount(7);
                    float center_x = position.x + skill_icon_size.x / 2;
                    float center_y = position.y + i * skill_icon_spacing + skill_icon_size.y / 2;
                    float arrow_size = skill_icon_size.x * 0.6f;
                    
                    // 箭头形状 (向右的箭头)
                    arrow_shape.setPoint(0, sf::Vector2f(center_x - arrow_size/3, center_y - arrow_size/6)); // 箭身上
                    arrow_shape.setPoint(1, sf::Vector2f(center_x + arrow_size/6, center_y - arrow_size/6)); // 箭头连接点上
                    arrow_shape.setPoint(2, sf::Vector2f(center_x + arrow_size/6, center_y - arrow_size/3)); // 箭头尖上
                    arrow_shape.setPoint(3, sf::Vector2f(center_x + arrow_size/2, center_y)); // 箭头尖端
                    arrow_shape.setPoint(4, sf::Vector2f(center_x + arrow_size/6, center_y + arrow_size/3)); // 箭头尖下
                    arrow_shape.setPoint(5, sf::Vector2f(center_x + arrow_size/6, center_y + arrow_size/6)); // 箭头连接点下
                    arrow_shape.setPoint(6, sf::Vector2f(center_x - arrow_size/3, center_y + arrow_size/6)); // 箭身下
                    
                    arrow_shape.setFillColor(sf::Color::White);
                    arrow_shape.setOutlineColor(sf::Color(200, 200, 200));
                    arrow_shape.setOutlineThickness(1.0f);
                }
                break;
        }
        
        skill_sprites.push_back(sprite);
        skill_arrows.push_back(arrow_shape);
        
        // 创建冷却遮罩 - 黑色扇形覆盖图标，冷却时从12点钟位置开始顺时针消失
        sf::ConvexShape overlay;
        
        // 根据冷却进度创建扇形遮罩
        float cooldown_progress = skill->getCooldownProgress();
        if (cooldown_progress > 0.0f) {
            // 创建黑色扇形遮罩，覆盖技能图标，表示剩余冷却时间
            // 冷却进度从12点钟位置开始顺时针消失，直到完全可用
            int point_count = 32; // 用多边形近似圆弧
            overlay.setPointCount(point_count + 2); // +2为圆心和起始点
            
            // 计算图标中心和半径
            float center_x = position.x + skill_icon_size.x / 2;
            float center_y = position.y + i * skill_icon_spacing + skill_icon_size.y / 2;
            float radius = skill_icon_size.x / 2;
            
            // 设置圆心
            overlay.setPoint(0, sf::Vector2f(center_x, center_y));
            
            // 计算扇形角度（从12点钟位置开始，顺时针消失）
            float start_angle = -90.0f; // 12点钟位置
            float current_angle = start_angle + 360.0f * (1.0f - cooldown_progress); // 当前冷却边界位置
            
            // 设置扇形的边界点（从当前位置到一圈后的当前位置）
            for (int j = 0; j <= point_count; ++j) {
                // 从current_angle开始顺时针绘制到current_angle + 360 * cooldown_progress
                float progress = static_cast<float>(j) / point_count;
                float angle = current_angle + 360.0f * cooldown_progress * progress;
                float radian = angle * 3.14159f / 180.0f;
                overlay.setPoint(j + 1, sf::Vector2f(
                    center_x + std::cos(radian) * radius,
                    center_y + std::sin(radian) * radius
                ));
            }
            
            overlay.setFillColor(sf::Color(0, 0, 0, 200)); // 深黑色遮罩覆盖技能图标
            overlay.setOutlineThickness(0);
        } else {
            // 没有冷却时创建空的形状（完全可用，无黑色遮罩）
            overlay.setPointCount(0);
        }
        
        cooldown_overlays.push_back(overlay);
    }
}

void SkillBar::draw(sf::RenderWindow& window) const {
    // 绘制技能图标背景
    for (const auto& icon : skill_icons) {
        window.draw(icon);
    }
    
    // 绘制技能图标纹理
    for (const auto& sprite : skill_sprites) {
        if (sprite.getTexture()) {
            window.draw(sprite);
        }
    }
    
    // 绘制程序化箭头
    for (const auto& arrow : skill_arrows) {
        if (arrow.getPointCount() > 0) {
            window.draw(arrow);
        }
    }
    
    // 绘制冷却遮罩
    for (const auto& overlay : cooldown_overlays) {
        if (overlay.getPointCount() > 0) { // 只绘制有形状的遮罩
            window.draw(overlay);
        }
    }
}

bool SkillBar::isSkillClicked(sf::Vector2f mouse_pos, int& skill_index) const {
    for (size_t i = 0; i < skill_icons.size(); ++i) {
        sf::Vector2f icon_center = sf::Vector2f(
            position.x + skill_icon_size.x / 2,
            position.y + i * skill_icon_spacing + skill_icon_size.y / 2
        );
        
        float dx = mouse_pos.x - icon_center.x;
        float dy = mouse_pos.y - icon_center.y;
        float distance_squared = dx * dx + dy * dy;
        float radius_squared = (skill_icon_size.x / 2) * (skill_icon_size.x / 2);
        
        if (distance_squared <= radius_squared) {
            skill_index = static_cast<int>(i);
            return true;
        }
    }
    return false;
}
