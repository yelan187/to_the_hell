#include "View/UI/SkillBar.h"
#include <iostream>
#include <algorithm>
#include <cmath>

using View::UI::SkillBar;

SkillBar::SkillBar(sf::Vector2f position) 
    : position(position), skill_icon_size(50.0f, 50.0f), skill_icon_spacing(60.0f), arrow_texture(nullptr), sprint_texture(nullptr) {
    loadTextures();
}

SkillBar::~SkillBar() {
    if (arrow_texture) {
        delete arrow_texture;
    }
    if (sprint_texture) {
        delete sprint_texture;
    }
}

void SkillBar::loadTextures() {
    // 加载冲刺技能纹理
    sprint_texture = new sf::Texture();
    if (!sprint_texture->loadFromFile("assets/images/skill/sprint.png")) {
        std::cerr << "ERROR: Failed to load sprint skill texture: assets/images/skill/sprint.png" << std::endl;
        delete sprint_texture;
        sprint_texture = nullptr;
    }
    
    // 加载箭矢技能纹理
    arrow_texture = new sf::Texture();
    if (!arrow_texture->loadFromFile("assets/images/skill/wide-arrow-dunk.png")) {
        std::cerr << "ERROR: Failed to load arrow skill texture: assets/images/skill/wide-arrow-dunk.png" << std::endl;
        delete arrow_texture;
        arrow_texture = nullptr;
    }
}

void SkillBar::updateSkills(const std::vector<Model::Entities::Skill*>& skills) {
    // 清空之前的技能UI元素，为新的技能状态做准备
    skill_icons.clear();    // 技能图标背景圆圈
    skill_sprites.clear();  // 技能纹理图标
    skill_shapes.clear();   // 程序化图标（备用）
    cooldown_overlays.clear(); // 冷却时间遮罩
    
    // 为每个技能创建对应的UI元素
    for (size_t i = 0; i < skills.size(); ++i) {
        Model::Entities::Skill* skill = skills[i];
        
        // 创建技能图标的背景圆圈
        sf::CircleShape icon(skill_icon_size.x / 2); // 半径为图标尺寸的一半
        icon.setPosition(position.x, position.y + i * skill_icon_spacing);
        icon.setFillColor(sf::Color(40, 40, 40));    // 深灰色背景
        icon.setOutlineColor(sf::Color::White);      // 白色边框
        icon.setOutlineThickness(2.0f);
        skill_icons.push_back(icon);
        
        // 根据技能类型创建对应的图标内容
        sf::Sprite sprite;
        sf::ConvexShape skill_shape;
        
        switch (skill->getType()) {
            case Model::Entities::SkillType::ARROW_SHOT:
                // 尝试加载箭矢技能纹理图标
                if (arrow_texture) {
                    sprite.setTexture(*arrow_texture);
                    // 计算缩放比例以适应图标大小
                    float scale_x = skill_icon_size.x / arrow_texture->getSize().x;
                    float scale_y = skill_icon_size.y / arrow_texture->getSize().y;
                    float scale = std::min(scale_x, scale_y) * 0.8f; // 留出边距
                    sprite.setScale(scale, scale);
                    
                    // 设置纹理居中显示
                    sf::FloatRect bounds = sprite.getLocalBounds();
                    sprite.setOrigin(bounds.width / 2, bounds.height / 2);
                    sprite.setPosition(
                        position.x + skill_icon_size.x / 2,
                        position.y + i * skill_icon_spacing + skill_icon_size.y / 2
                    );
                } else {
                    // 箭矢纹理加载失败，显示空的形状
                    skill_shape.setPointCount(0);
                }
                break;
                
            case Model::Entities::SkillType::SPRINT:
                // 尝试加载冲刺技能纹理图标
                if (sprint_texture) {
                    sprite.setTexture(*sprint_texture);
                    // 计算缩放比例以适应图标大小
                    float scale_x = skill_icon_size.x / sprint_texture->getSize().x;
                    float scale_y = skill_icon_size.y / sprint_texture->getSize().y;
                    float scale = std::min(scale_x, scale_y) * 0.8f; // 留出边距
                    sprite.setScale(scale, scale);
                    
                    // 设置纹理居中显示
                    sf::FloatRect bounds = sprite.getLocalBounds();
                    sprite.setOrigin(bounds.width / 2, bounds.height / 2);
                    sprite.setPosition(
                        position.x + skill_icon_size.x / 2,
                        position.y + i * skill_icon_spacing + skill_icon_size.y / 2
                    );
                } else {
                    // 冲刺纹理加载失败，显示空的形状
                    skill_shape.setPointCount(0);
                }
                break;
        }
        
        // 将创建的图标元素添加到对应容器中
        skill_sprites.push_back(sprite);
        skill_shapes.push_back(skill_shape);
        
        // 创建技能冷却时间的可视化遮罩
        // 使用扇形表示剩余冷却时间，从12点钟位置开始顺时针消失
        sf::ConvexShape overlay;
        float cooldown_progress = skill->getCooldownProgress(); // 0.0表示完全可用，1.0表示完全冷却
        
        if (cooldown_progress > 0.0f) {
            // 创建扇形遮罩来显示剩余冷却时间
            int point_count = 32; // 使用32个点来近似圆弧，确保平滑效果
            overlay.setPointCount(point_count + 2); // +2为圆心点和起始点
            
            // 计算技能图标的中心位置和半径
            float center_x = position.x + skill_icon_size.x / 2;
            float center_y = position.y + i * skill_icon_spacing + skill_icon_size.y / 2;
            float radius = skill_icon_size.x / 2;
            
            // 设置扇形的圆心为第一个点
            overlay.setPoint(0, sf::Vector2f(center_x, center_y));
            
            float start_angle = -90.0f; // 从12点钟位置开始（-90度）
            // 计算当前冷却边界的角度位置
            float current_angle = start_angle + 360.0f * (1.0f - cooldown_progress);
            
            // 生成扇形边界上的点，从当前角度开始绘制剩余的冷却区域
            for (int j = 0; j <= point_count; ++j) {
                float progress = static_cast<float>(j) / point_count;
                float angle = current_angle + 360.0f * cooldown_progress * progress;
                float radian = angle * 3.14159f / 180.0f;
                overlay.setPoint(j + 1, sf::Vector2f(
                    center_x + std::cos(radian) * radius,
                    center_y + std::sin(radian) * radius
                ));
            }
            
            // 设置遮罩为半透明黑色，覆盖冷却中的技能图标
            overlay.setFillColor(sf::Color(0, 0, 0, 200));
            overlay.setOutlineThickness(0);
        } else {
            // 技能完全可用时，不显示任何遮罩
            overlay.setPointCount(0);
        }
        
        cooldown_overlays.push_back(overlay);
    }
}

void SkillBar::draw(sf::RenderWindow& window) const {
    // 绘制技能图标背景圆圈
    for (const auto& icon : skill_icons) {
        window.draw(icon);
    }
    
    // 绘制技能纹理图标
    for (const auto& sprite : skill_sprites) {
        if (sprite.getTexture()) {
            window.draw(sprite);
        }
    }
    
    // 绘制程序化技能图标（如果纹理加载失败）
    for (const auto& shape : skill_shapes) {
        if (shape.getPointCount() > 0) {
            window.draw(shape);
        }
    }
    
    // 绘制技能冷却遮罩
    for (const auto& overlay : cooldown_overlays) {
        if (overlay.getPointCount() > 0) {
            window.draw(overlay);
        }
    }
}

bool SkillBar::isSkillClicked(sf::Vector2f mouse_pos, int& skill_index) const {
    // 遍历所有技能图标，检查鼠标点击位置是否在图标范围内
    for (size_t i = 0; i < skill_icons.size(); ++i) {
        // 计算当前技能图标的中心位置
        sf::Vector2f icon_center = sf::Vector2f(
            position.x + skill_icon_size.x / 2,
            position.y + i * skill_icon_spacing + skill_icon_size.y / 2
        );
        
        // 使用圆形碰撞检测（技能图标是圆形的）
        float dx = mouse_pos.x - icon_center.x;
        float dy = mouse_pos.y - icon_center.y;
        float distance_squared = dx * dx + dy * dy;
        float radius_squared = (skill_icon_size.x / 2) * (skill_icon_size.x / 2);
        
        // 如果鼠标位置在圆形图标范围内
        if (distance_squared <= radius_squared) {
            skill_index = static_cast<int>(i); // 返回被点击的技能索引
            return true;
        }
    }
    return false; // 没有点击到任何技能图标
}
