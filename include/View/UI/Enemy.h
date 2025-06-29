#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <memory>

namespace ViewModel {
    class GameViewModel;
}

namespace View{
namespace UI {

enum class EnemyType {
    GHOST
};

class Enemy {
public:
    void init(std::shared_ptr<ViewModel::GameViewModel> view_model, int id);
    void render(sf::RenderWindow& window);
    void update(float delta_time);

private:
    int id;
    std::shared_ptr<ViewModel::GameViewModel> view_model;
    sf::Vector2f size;
    sf::Vector2f last_position;
    bool facing_left;
    static std::map<EnemyType, sf::Texture> texture_map;
    static bool textures_loaded;
    sf::RectangleShape rect;
    
    static void loadTextures();
    void setTexture(EnemyType type);
    void updateTextureTransform();
};

}
}
