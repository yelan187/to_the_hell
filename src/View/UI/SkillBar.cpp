#include "View/UI/SkillBar.h"

using namespace View::UI;

SkillBar::SkillBar(sf::Vector2f position, sf::Vector2f skill_size) 
    : position(position), skill_size(skill_size), skill_spacing(skill_size.y + 10.0f) { // 改为垂直间距
    
    // 创建两个技能：ARROW_SHOT 和 SPRINT，垂直排列在左侧
    skills.emplace_back(SkillType::ARROW_SHOT, position, skill_size);
    skills.emplace_back(SkillType::SPRINT, 
                       sf::Vector2f(position.x, position.y + skill_spacing), // 垂直排列
                       skill_size);
    skills.emplace_back(SkillType::GROUND_PENETRATION,
                       sf::Vector2f(position.x, position.y + 2 * skill_spacing), // 垂直排列
                       skill_size);
}

void SkillBar::updateSkills(const std::vector<Common::FrameInfo::SkillInfo>& skills_info) {
    for (size_t i = 0; i < skills.size() && i < skills_info.size(); ++i) {
        skills[i].update(skills_info[i].cooldown_progress, skills_info[i].is_available);
    }
}

void SkillBar::render(sf::RenderWindow& window) {
    for (auto& skill : skills) {
        skill.render(window);
    }
}

int SkillBar::getClickedSkill(sf::Vector2f mouse_pos) const {
    for (size_t i = 0; i < skills.size(); ++i) {
        if (skills[i].isClicked(mouse_pos)) {
            return static_cast<int>(i);
        }
    }
    return -1; // 没有点击到任何技能
}
