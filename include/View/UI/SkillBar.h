#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Model/Entities/Skill.h"

namespace View {
    namespace UI {
        class SkillBar {
        private:
            sf::Vector2f position;
            sf::Vector2f skill_icon_size;
            float skill_icon_spacing;
            std::vector<sf::CircleShape> skill_icons;
            std::vector<sf::Sprite> skill_sprites; // 用于显示纹理图标
            std::vector<sf::ConvexShape> skill_arrows; // 用于显示程序化箭头
            std::vector<sf::ConvexShape> cooldown_overlays; // 改为ConvexShape以支持扇形
            sf::Texture* arrow_texture;
            
        public:
            SkillBar(sf::Vector2f position);
            ~SkillBar();
            
            void updateSkills(const std::vector<Model::Entities::Skill*>& skills);
            void draw(sf::RenderWindow& window) const;
            bool isSkillClicked(sf::Vector2f mouse_pos, int& skill_index) const;
            
        private:
            void loadTextures();
        };
    }
}
