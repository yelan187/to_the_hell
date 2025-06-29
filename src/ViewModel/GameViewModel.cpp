#include <iostream>
#include "ViewModel/GameViewModel.h"
#include "Model/GameModel.h"
#include "View/GameView.h"

using ViewModel::GameViewModel;

GameViewModel::GameViewModel(Core::Engine& engine, sf::Vector2u window_size) : ViewModel(engine) {
    model = std::make_shared<Model::GameModel>(engine, window_size);
    init_keystate();
}

std::string GameViewModel::getGameTime() {
    std::chrono::seconds duration = model->getDuration();
    int seconds = duration.count() % 60;
    int minutes = (duration.count() / 60) % 60;
    int hours = duration.count() / 3600;

    std::stringstream ss;
    ss << "Time: ";
    if (hours > 0) {
        ss << std::setw(2) << std::setfill('0') << hours << "h";
    }
    if (hours > 0 || minutes > 0) {
        ss << std::setw(2) << std::setfill('0') << minutes << "m";
    } else {
        ss << "00m";
    }
    ss << std::setw(2) << std::setfill('0') << seconds << "s";

    return ss.str();
}

std::string GameViewModel::getTotalScore() {
    return "Score: " + std::to_string(model->getTotalScore());
}

View::UI::PlayerState GameViewModel::getPlayerState() {
    Model::Entities::PlayerState player_state = model->getPlayerState();
    switch (player_state) {
        case Model::Entities::PlayerState::IDLE:
            if (player_towards == towards::LEFT) {
                return View::UI::PlayerState::IDLE_L;
            } else {
                return View::UI::PlayerState::IDLE_R;
            }
        case Model::Entities::PlayerState::WALKING:
            if (player_towards == towards::LEFT) {
                return View::UI::PlayerState::WALKING_L;
            } else {
                return View::UI::PlayerState::WALKING_R;
            }
        case Model::Entities::PlayerState::JUMPING_IDLE:
        case Model::Entities::PlayerState::JUMPING_WALKING:
            if (player_towards == towards::LEFT) {
                return View::UI::PlayerState::JUMPING_L;
            } else {
                return View::UI::PlayerState::JUMPING_R;
            }
        
        // // Default
        // return View::UI::PlayerState::IDLE_R;
    }
}

sf::Vector2f GameViewModel::getPlayerPosition(sf::Vector2f view_playersize) {
    sf::Vector2f position = model->getPlayerPosition();
    sf::Vector2f real_pos = position - (view_playersize - getPlayerSize()) / 2.0f;

    return real_pos;
}

void GameViewModel::playerJump() {
    if (!key_state[sf::Keyboard::W]) {
        model->playerJump();
        key_state[sf::Keyboard::W] = true;
    }
}

void GameViewModel::playerDown() {
    if (!key_state[sf::Keyboard::S]) {
        // std::cout << "Press S to go down" << std::endl;
        model->playerDown();
        key_state[sf::Keyboard::S] = true;
    }
}

void GameViewModel::playerWalkLeft() {
    if (!key_state[sf::Keyboard::A]) {
        // std::cout << "Press A to walk left" << std::endl;
        key_state[sf::Keyboard::A] = true;
        if (!key_state[sf::Keyboard::D]) {
            player_towards = towards::LEFT;
        }
        model->playerWalkLeft();
    }
}

void GameViewModel::playerWalkRight() {
    if (!key_state[sf::Keyboard::D]) {
        // std::cout << "Press D to walk right" << std::endl;
        key_state[sf::Keyboard::D] = true;
        if (!key_state[sf::Keyboard::A]) {
            player_towards = towards::RIGHT;
        }
        model->playerWalkRight();
    }
}

void GameViewModel::playerStopLeft() {
    // std::cout << "Release A" << std::endl;
    key_state[sf::Keyboard::A] = false;
    if (key_state[sf::Keyboard::D]) {
        player_towards = towards::RIGHT;
    }
    model->playerStopLeft();
}

void GameViewModel::playerStopRight() {
    // std::cout << "Release D" << std::endl;
    key_state[sf::Keyboard::D] = false;
    if (key_state[sf::Keyboard::A]) {
        player_towards = towards::LEFT;
    }
    model->playerStopRight();
}

void GameViewModel::playerStopJump() {
    key_state[sf::Keyboard::W] = false;
}

void GameViewModel::playerStopDown() {
    key_state[sf::Keyboard::S] = false;
}

std::vector<int> GameViewModel::getPlatformsId() {
    std::map<int, Model::Entities::Platform*> platforms = model->getPlatforms();
    std::vector<int> platform_ids;
    for (const auto& pair : platforms) {
        platform_ids.push_back(pair.first);
    }
    return platform_ids;
}

View::UI::PlatformType GameViewModel::getPlatformTypeById(int id) {
    Model::Entities::Platform* platform = model->getPlatformById(id);
    if (platform) {
        return static_cast<View::UI::PlatformType>(platform->type);
    }
    return View::UI::PlatformType::NORMAL;
}

sf::Vector2f GameViewModel::getPlatformPositionById(int id) {
    Model::Entities::Platform* platform = model->getPlatformById(id);
    if (platform) {
        return platform->position;
    }
    return sf::Vector2f(0, 0);
}

bool GameViewModel::getPlatformRollingDirection(int id) {
    Model::Entities::Platform* platform = model->getPlatformById(id);
    if (platform) {
        return platform->getRollingDirection();
    }
    return true; // 默认向右
}

std::vector<int> GameViewModel::getEnemiesId() {
    std::map<int, Model::Entities::Enemy*> enemies = model->getEnemies();
    std::vector<int> enemy_ids;
    for (const auto& pair : enemies) {
        enemy_ids.push_back(pair.first);
    }
    return enemy_ids;
}

View::UI::EnemyType GameViewModel::getEnemyTypeById(int id) {
    Model::Entities::Enemy* enemy = model->getEnemies().at(id);
    if (enemy) {
        return static_cast<View::UI::EnemyType>(enemy->getType());
    }
    return View::UI::EnemyType::GHOST;
}

sf::Vector2f GameViewModel::getEnemyPositionById(int id) {
    Model::Entities::Enemy* enemy = model->getEnemies().at(id);
    if (enemy) {
        return enemy->getPosition();
    }
    return sf::Vector2f(0, 0);
}

std::vector<int> GameViewModel::getBulletsId() {
    std::map<int, Model::Entities::Bullet*> bullets = model->getBullets();
    std::vector<int> bullet_ids;
    for (const auto& pair : bullets) {
        bullet_ids.push_back(pair.first);
    }
    return bullet_ids;
}

sf::Vector2f GameViewModel::getBulletPositionById(int id) {
    Model::Entities::Bullet* bullet = model->getBullets().at(id);
    if (bullet) {
        return bullet->getPosition();
    }
    return sf::Vector2f(0, 0);
}

std::vector<int> GameViewModel::getPickupsId() {
    std::map<int, Model::Entities::Pickup*> pickups = model->getPickups();
    std::vector<int> pickup_ids;
    for (const auto& pair : pickups) {
        pickup_ids.push_back(pair.first);
    }
    return pickup_ids;
}

View::UI::PickupType GameViewModel::getPickupTypeById(int id) {
    Model::Entities::Pickup* pickup = model->getPickups().at(id);
    if (pickup) {
        return static_cast<View::UI::PickupType>(pickup->getType());
    }
    return View::UI::PickupType::NORMAL_DOT;
}

sf::Vector2f GameViewModel::getPickupPositionById(int id) {
    Model::Entities::Pickup* pickup = model->getPickups().at(id);
    if (pickup) {
        return pickup->getPosition();
    }
    return sf::Vector2f(0, 0);
}

std::vector<int> GameViewModel::getArrowsId() {
    std::map<int, Model::Entities::Arrow*> arrows = model->getArrows();
    std::vector<int> arrow_ids;
    for (const auto& pair : arrows) {
        arrow_ids.push_back(pair.first);
    }
    return arrow_ids;
}

sf::Vector2f GameViewModel::getArrowPosition(int id) {
    auto arrows = model->getArrows();
    if (arrows.find(id) != arrows.end()) {
        return arrows[id]->getPosition();
    }
    return sf::Vector2f(0, 0);
}

sf::Vector2f GameViewModel::getArrowSize(int id) {
    auto arrows = model->getArrows();
    if (arrows.find(id) != arrows.end()) {
        return arrows[id]->getSize();
    }
    return model->arrow_size;
}

bool GameViewModel::getArrowFacingRight(int id) {
    auto arrows = model->getArrows();
    if (arrows.find(id) != arrows.end()) {
        sf::Vector2f velocity = arrows[id]->getVelocity();
        return velocity.x > 0; // 如果水平速度大于0，则向右
    }
    return true; // 默认向右
}

std::vector<Model::Entities::Skill*> GameViewModel::getSkills() {
    return model->getSkills();
}

void GameViewModel::useSkill(int skill_index) {
    model->useSkill(skill_index);
}