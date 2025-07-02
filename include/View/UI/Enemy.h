#pragma once
#include <SFML/Graphics.hpp>
#include "Common/NotificationBase.h"
#include "Common/FrameInfo.h"

namespace View {
namespace UI {

class Enemy {
public:
    Enemy(int id, sf::RenderWindow& window);
    void update(const Common::FrameInfo::EnemyInfo& info);
    void render();

private:
    int id;
    sf::RenderWindow& window;
    sf::RectangleShape shape;
    sf::Sprite sprite;  // 用于纹理渲染
    sf::Vector2f position;
    sf::Vector2f size;
    sf::Vector2f facing_direction;  // 面向方向
    static sf::Texture ghost_texture;
    static bool texture_loaded;
    
    static void loadTextures();
};

}
}
