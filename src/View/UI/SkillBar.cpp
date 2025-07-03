#include "View/UI/SkillBar.h"
#include "Common/SkillID.h"

using namespace View::UI;

SkillBar::SkillBar(sf::Vector2f position, sf::Vector2f skill_size) 
    : position(position), skill_size(skill_size), skill_spacing(skill_size.y + 10.0f) { // 改为垂直间距
    
    // 创建三个技能：ARROW_SHOT、SPRINT、GROUND_PENETRATION，垂直排列在左侧
    skills.emplace_back(SkillType::ARROW_SHOT, position, skill_size);
    skills.emplace_back(SkillType::SPRINT, 
                       sf::Vector2f(position.x, position.y + skill_spacing), // 垂直排列
                       skill_size);
    skills.emplace_back(SkillType::GROUND_PENETRATION,
                       sf::Vector2f(position.x, position.y + 2 * skill_spacing), // 垂直排列
                       skill_size);
}

void SkillBar::updateSkills(const std::vector<Common::FrameInfo::SkillInfo>& skills_info) {
    // 根据技能ID映射到对应的UI技能
    for (auto& skill : skills) {
        // 找到对应的技能信息
        for (const auto& skill_info : skills_info) {
            if (mapSkillIDToType(skill_info.skill_id) == skill.getType()) {
                skill.update(skill_info.cooldown_progress, skill_info.is_available);
                break;
            }
        }
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

SkillType SkillBar::mapSkillIDToType(Common::SkillID skill_id) const {
    switch (skill_id) {
        case Common::SkillID::ARROW_SHOT:
            return SkillType::ARROW_SHOT;
        case Common::SkillID::SPRINT:
            return SkillType::SPRINT;
        case Common::SkillID::GROUND_PENETRATION:
            return SkillType::GROUND_PENETRATION;
        default:
            return SkillType::ARROW_SHOT; // 默认值
    }
}
