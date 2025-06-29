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
            std::vector<sf::Sprite> skill_sprites;
            std::vector<sf::ConvexShape> skill_shapes;  // 程序化技能图标形状
            std::vector<sf::ConvexShape> cooldown_overlays;
            sf::Texture* arrow_texture;
            sf::Texture* sprint_texture;
            
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
