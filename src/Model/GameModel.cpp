#include <iostream>
#include "Model/GameModel.h"

using Model::GameModel;
using Model::Entities::PlatformType;

GameModel::GameModel(Core::Engine &engine) : 
    Model(engine), init(false) {
}

GameModel::GameModel(Core::Engine &engine, sf::Vector2f window_size) : 
    Model(engine, window_size), init(false) {
}

void GameModel::update(float delta_time) {
    if (!init) {
        initGame();
        init = true;
        return;
    }

    platform_generate_interval -= delta_time;
    if (platform_generate_interval <= 0.0f) {
        resetPlatformGenerateInterval();
        generatePlatform();
    }

    for (auto& platform_pair : platforms) {
        Entities::Platform* platform = platform_pair.second;
        
    }
}

PlatformType GameModel::getPlatformTypeRand() {
    return PlatformType::NORMAL;
}

void GameModel::generatePlatform() {
    sf::Vector2f platform_size(100, 20);
    sf::Vector2f position(
        static_cast<float>(rand() % static_cast<int>(window_size.x - platform_size.x)),
        window_size.y
    );
    
    PlatformType type = getPlatformTypeRand();
    
    platforms[next_platform_id++] = new Entities::Platform(
                                        next_platform_id, type, position, platform_size, scroll_speed);
}

void GameModel::initPlatforms() {
    const int initial_platforms = 5;
    sf::Vector2f platform_size(100, 20);
    for (int i = 0; i < initial_platforms; ++i) {
        int id = i;
        
        sf::Vector2f position(
            static_cast<float>(rand() % static_cast<int>(window_size.x - platform_size.x)),
            static_cast<float>(window_size.y / 2 + window_size.y / 2 / initial_platforms * i)
        );
        
        PlatformType type = getPlatformTypeRand();

        platforms[id] = new Entities::Platform(id, type, position, platform_size, scroll_speed);
    }
    next_platform_id = initial_platforms;
}

void GameModel::initPlayer() {
    sf::Vector2f player_size = sf::Vector2f(50, 50);
    sf::Vector2f player_position  = sf::Vector2f(
        platforms[0]->getPosition().x + platforms[0]->getSize().x / 2 - player_size.x / 2,
        window_size.y / 4
    );
    player = new Entities::Player(player_position,player_size);
}

void GameModel::resetPlatformGenerateInterval() {
    platform_generate_interval = 1.0f + static_cast<float>(rand() % 1 - 0.5);
}

void GameModel::initGame() {
    total_score = 0;
    game_time = std::chrono::seconds(0);
    resetPlatformGenerateInterval();
    initPlatforms();
    initPlayer();
}