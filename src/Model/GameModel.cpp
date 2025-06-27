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

    player->on_platform = -1;
    for (auto& platform_pair : platforms) {
        Entities::Platform* platform = platform_pair.second;
        if (player->onPlatform(platform)) {
            player->on_platform = platform->id;
            break;
        }
    }

    if (player->on_platform != -1) {
        player->assocatedVelocity(platforms[player->on_platform]->getVelocity());
    } else {
        player->addAcceleration(gravity);
    }

    for (auto& platform_pair : platforms) {
        platform_pair.second->update(delta_time);
    }
    player->update(delta_time);

    // check if player is in certain platform
    player->on_platform = -1;
    for (auto& platform_pair : platforms) {
        Entities::Platform* platform = platform_pair.second;
        if (player->onPlatform(platform)) {
            player->on_platform = platform->id;
            break;
        }
    }

    if (player->on_platform != -1) {
        adjustPlayerPosition();
    } else {
        player->addAcceleration(gravity);
    }
}

void GameModel::adjustPlayerPosition(){
    Entities::Platform* platform = platforms[player->on_platform];
    
    sf::Vector2f player_lt = player->getPosition();
    sf::Vector2f player_rb = player_lt + player->getSize();

    sf::Vector2f platform_lt = platform->getPosition();
    sf::Vector2f platform_rb = platform_lt + platform->getSize();

    // witch corner of the player is in the platform
    int lt = player_lt.x < platform_rb.x && player_lt.x > platform_lt.x &&
            player_lt.y < platform_rb.y && player_lt.y > platform_lt.y;

    int rb = player_rb.x < platform_rb.x && player_rb.x > platform_lt.x &&
            player_rb.y < platform_rb.y && player_rb.y > platform_lt.y;

    int lb = player_lt.x < platform_rb.x && player_lt.x > platform_lt.x &&
            player_rb.y < platform_rb.y && player_rb.y > platform_lt.y;
    
    int rt = player_rb.x < platform_rb.x && player_rb.x > platform_lt.x &&
            player_lt.y < platform_rb.y && player_lt.y > platform_lt.y;

    switch (lt*8 + lb*4 + rb*2 + rt) {
        case 0b1000: // left top
            player->setPosition(sf::Vector2f(platform_rb.x, player_lt.y));
            break;
        case 0b0100: // left bottom
            player->setPosition(sf::Vector2f(player_lt.x, platform_rb.y - player->getSize().y));
            break;
        case 0b0010: // right bottom
            player->setPosition(sf::Vector2f(player_lt.x, platform_rb.y - player->getSize().y));
            break;
        case 0b0001: // right top
            player->setPosition(sf::Vector2f(platform_rb.x - player->getSize().x, player_lt.y));
            break;
        case 0b0110: // right bottom and left bottom
            player->setPosition(sf::Vector2f(player_lt.x, platform_lt.y - player->getSize().y));
            break;
        case 0b1001: // left top and right top
            player->setPosition(sf::Vector2f(player_lt.x, player_rb.y));
            break;
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
    gravity += sf::Vector2f(0,2.0f);
    resetPlatformGenerateInterval();
    initPlatforms();
    initPlayer();
}