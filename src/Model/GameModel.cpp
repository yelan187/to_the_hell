#include <iostream>
#include "Model/GameModel.h"

using Model::GameModel;
using Model::Entities::PlatformType;

GameModel::GameModel(sf::Vector2u window_size) : 
    Model(window_size), init(false) {
    initGame();
    init = true;
}

GameModel::~GameModel() {
    if (player) {
        delete player;
        player = nullptr;
    }
    for (auto& pair : platforms) {
        if (pair.second) {
            delete pair.second;
        }
    }
    platforms.clear();
}

void GameModel::fire() {
    Common::_ChangeGameFrameParam* param = new Common::_ChangeGameFrameParam();
    param->id = Common::NotificationId::_ChangeGameFrame;
    param->value.player_info.position = player->getPosition();
    param->value.player_info.size = player->getSize();
    param->value.player_info.state = player->getState();
    std::vector<int> platforms_id;
    std::map<int, Common::_FrameInfo::PlatformInfo> platforms_info;
    for (const auto& pair:platforms) {
        platforms_id.push_back(pair.first);
        platforms_info[pair.first].position = pair.second->getPosition();
        platforms_info[pair.first].type = pair.second->type;
    }
    param->value.platforms_id = platforms_id;
    param->value.platforms_info = platforms_info;
    trigger.fire(param);
}

void GameModel::gameOver() {
    Common::GameOverNotificationParam* param = new Common::GameOverNotificationParam();
    param->id = Common::NotificationId::GameOver;
    param->value.total_score = total_score;
    param->value.game_time = std::chrono::seconds(static_cast<int>(game_time));
    trigger.fire(param);
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
    // std::cout << "GameModel update start" << std::endl;
    game_time += delta_time;

    platform_generate_interval -= delta_time;
    if (platform_generate_interval <= 0.0f) {
        resetPlatformGenerateInterval();
        generatePlatform();
    }

    for (auto it = platforms.begin(); it != platforms.end(); ) {
        Entities::Platform* platform = it->second;
        platform->update(delta_time);
        std::cout << "Platform " << it->first << " position: "
                  << platform->getPosition().x << ", "
                  << platform->getPosition().y << std::endl;
        if (platform->outOfWindow(window_size)) {
            delete platform;
            // std::cout << "Platform " << it->first << " removed." << std::endl;
            it = platforms.erase(it);
        } else {
            ++it;
        }
    }
    // std::cout << "player update start" << std::endl;
    player->update(delta_time);
    // std::cout << "player update end" << std::endl;
    fire();
    if (player->getPosition().y <= 0 || player->getPosition().y + player->getSize().y >= window_size.y) {
        // Player is out of bounds, reset the game
        std::cout << "Player out of bounds, resetting game." << std::endl;
        gameOver();
    }
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
    player = new Entities::Player(player_position,player_size, this);
}

void GameModel::resetPlatformGenerateInterval() {
    platform_generate_interval = 1.0f + static_cast<float>(rand() % 1 - 0.5) / 2;
}

void GameModel::initGame() {
    total_score = 0;
    scroll_speed = 100.0f;
    game_time = 0;
    resetPlatformGenerateInterval();
    initPlatforms();
    initPlayer();
}