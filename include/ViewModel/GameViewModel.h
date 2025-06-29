#pragma once

#include <ViewModel/ViewModel.h>
#include <Model/GameModel.h>
#include <functional>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>

namespace View{
namespace UI{
    class Player;
    class Platform;
    class Enemy;
    class Bullet;
    
    enum class PlayerState;
    enum class PlatformType;
    enum class EnemyType;
}
}

namespace ViewModel {

enum class towards {
    LEFT,
    RIGHT
};
class GameViewModel : public ViewModel {
public:
    GameViewModel(Core::Engine& engine, sf::Vector2u windowSize);
    
    void update(float delta_time) { model->update(delta_time); }

    std::string getTotalScore();
    std::string getGameTime();
    std::string getDebugInfo() {
        return model->getDebugInfo();
    }

    View::UI::PlayerState getPlayerState();
    sf::Vector2f getPlayerPosition(sf::Vector2f view_playersize);
    sf::Vector2f getPlayerSize() { return model->player_size; }
    sf::Vector2f getPlatformSize() { return model->platform_size; }
    sf::Vector2f getEnemySize() { return model->enemy_size; }
    sf::Vector2f getBulletSize() { return model->bullet_size; }

    void playerJump();
    void playerDown();
    void playerWalkLeft();
    void playerWalkRight();
    void playerStopLeft();
    void playerStopRight();
    void playerStopJump();
    void playerStopDown();

    std::vector<int> getPlatformsId();
    View::UI::PlatformType getPlatformTypeById(int id);
    sf::Vector2f getPlatformPositionById(int id);
    
    std::vector<int> getEnemiesId();
    View::UI::EnemyType getEnemyTypeById(int id);
    sf::Vector2f getEnemyPositionById(int id);
    
    std::vector<int> getBulletsId();
    sf::Vector2f getBulletPositionById(int id);    

private:
    std::shared_ptr<Model::GameModel> model;
    towards player_towards = towards::RIGHT;
    std::map<sf::Keyboard::Key, bool> key_state;
    void init_keystate() {
        key_state[sf::Keyboard::W] = false;
        key_state[sf::Keyboard::A] = false;
        key_state[sf::Keyboard::S] = false;
        key_state[sf::Keyboard::D] = false;
    }
};

}