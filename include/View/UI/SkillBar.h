#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "View/UI/Skill.h"
#include "Common/NotificationBase.h"

namespace View {
namespace UI {

// 技能栏UI组件 - 显示技能图标和冷却进度
class SkillBar {
public:
    SkillBar(sf::Vector2f position, sf::Vector2f skill_size = sf::Vector2f(50.0f, 50.0f));
    void updateSkills(const std::vector<Common::FrameInfo::SkillInfo>& skills_info);  // 更新技能状态
    void render(sf::RenderWindow& window);  // 渲染技能栏
    int getClickedSkill(sf::Vector2f mouse_pos) const;  // 检测鼠标点击的技能

private:
    sf::Vector2f position;
    sf::Vector2f skill_size;
    float skill_spacing;
    std::vector<Skill> skills;
};

}
}
