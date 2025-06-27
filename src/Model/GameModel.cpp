#include <iostream>
#include "Model/GameModel.h"

using Model::GameModel;
using Model::Entities::PlatformType;

GameModel::GameModel(Core::Engine &engine, sf::Vector2u window_size) : 
    Model(engine, window_size), init(false) {
}

void GameModel::update(float delta_time) {
    /*
     *
     * This function is called every frame to update the game state (after handling input).
     * 
     * It will check if the game is initialized, and if not, it will initialize the game.
     * 
     * Then :
     * 
     *  1. update the platform generation interval
     * 
     *  2. update platform position
     *      2.1 if the platform is moving, update its position
     *      2.2 if the platform is not moving, check if it should start moving
     *
     *  3. update player position
     *      3.1 if the player is on a platform, associate the player's velocity with the platform's velocity
     *      3.2 if the player is not on any platform, add gravity to the player's acceleration
     *      3.3 after updating the platforms and player, the player may be stuck in a platform ,
     *          so we need to check if the player is in any platform again
     *      3.4 if the player is on a platform, adjust the player's position
     * 
     */
    if (!init) {
        initGame();
        init = true;
        return;
    }

    game_time += delta_time;

    platform_generate_interval -= delta_time;
    if (platform_generate_interval <= 0.0f) {
        resetPlatformGenerateInterval();
        generatePlatform();
    }

    for (auto& platform_pair : platforms) {
        Entities::Platform* platform = platform_pair.second;
        platform->update(delta_time);
        if (platform->outOfWindow(window_size)) {
            // remove platform if it is out of window
            delete platform;
            platforms.erase(platform_pair.first);
        }
    }

    player->update(delta_time, platforms);
}

PlatformType GameModel::getPlatformTypeRand() {
    return PlatformType::NORMAL;
}

void GameModel::generatePlatform() {
    sf::Vector2f position(
        static_cast<float>(rand() % static_cast<int>(window_size.x - platform_size.x)),
        window_size.y
    );
    
    PlatformType type = getPlatformTypeRand();
    
    platforms[next_platform_id++] = new Entities::Platform(
                                        next_platform_id, type, position, platform_size, scroll_speed);
}

void GameModel::initPlatforms() {
    const int initial_platforms = 3;
    for (int i = 0; i < initial_platforms; ++i) {
        int id = i;
        
        sf::Vector2f position(
            static_cast<float>(rand() % static_cast<int>(window_size.x - platform_size.x)),
            static_cast<float>(window_size.y / 3 + window_size.y / 2 / initial_platforms * i)
        );
        
        PlatformType type = getPlatformTypeRand();

        platforms[id] = new Entities::Platform(id, type, position, platform_size, scroll_speed);
    }
    next_platform_id = initial_platforms;
}

void GameModel::initPlayer() {
    sf::Vector2f player_position  = sf::Vector2f(
        platforms[0]->getPosition().x + platforms[0]->getSize().x / 2 - player_size.x / 2,
        window_size.y / 5
    );
    // std::cout << "Player position: " << player_position.x << ", " << player_position.y << std::endl;
    player = new Entities::Player(player_position,player_size);
}

void GameModel::resetPlatformGenerateInterval() {
    platform_generate_interval = 1.0f + static_cast<float>(rand() % 1 - 0.5);
}

void GameModel::initGame() {
    total_score = 0;
    scroll_speed = 100.0f;
    game_time = 0;
    resetPlatformGenerateInterval();
    initPlatforms();
    initPlayer();
}