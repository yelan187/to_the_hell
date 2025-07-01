#pragma once
#include <SFML/Graphics.hpp>

namespace View {
namespace UI {

enum class SkillType {
    ARROW_SHOT,
    SPRINT
};

class Skill {
public:
    Skill(SkillType type, sf::Vector2f position, sf::Vector2f size);
    void update(float cooldown_progress, bool is_available);
    void render(sf::RenderWindow& window);
    bool isClicked(sf::Vector2f mouse_pos) const;
    SkillType getType() const { return type; }

private:
    SkillType type;
    sf::Vector2f position;
    sf::Vector2f size;
    sf::CircleShape background;
    sf::CircleShape cooldown_overlay;
    sf::Texture* skill_texture;
    sf::Sprite skill_sprite;
    bool is_available;
    float cooldown_progress;
    
    void loadTexture();
    void createProgrammaticIcon();
    void createCooldownSector(sf::ConvexShape& sector, float progress);  // 创建扇形冷却遮罩
    sf::ConvexShape arrow_shape;
    sf::CircleShape sprint_shape;
};

}
}
