#include <iostream>
#include <algorithm>
#include <vector>
#include "Model/GameModel.h"
#include "Model/Entities/Platform.h"
#include "Model/Entities/Player.h"
#include "Model/GameConfig.h"
#include "Core/Engine.h"

using Model::GameModel;
using Model::Entities::PlatformType;

GameModel::GameModel(Core::Engine &engine, sf::Vector2u window_size) : 
    Model(engine, window_size), init(false) {
    // 初始化管理器
    entity_manager = std::make_unique<Managers::EntityManager>(window_size);
    generator_manager = std::make_unique<Managers::GeneratorManager>(window_size, entity_manager.get(), this);
    collision_manager = std::make_unique<Managers::CollisionManager>(entity_manager.get());
    skill_manager = std::make_unique<Managers::SkillManager>(entity_manager.get(), this);
    
    initGame();
    init = true;
}

GameModel::~GameModel() {
    if (player) {
        delete player;
        player = nullptr;
    }
    // 管理器会自动清理，使用智能指针
}

void GameModel::update(float delta_time) {
    /*
     * This function is called every frame to update the game state (after handling input).
     * 
     * Now it uses managers to handle different aspects:
     * 1. GeneratorManager handles entity generation
     * 2. EntityManager handles entity updates
     * 3. CollisionManager handles collision detection
     * 4. Player update is handled separately
     */
    if (!init) {
        initGame();
        init = true;
        return;
    }
    game_time += delta_time;

    // 使用生成器管理器更新实体生成
    generator_manager->update(delta_time);
    
    // 使用实体管理器更新所有实体
    entity_manager->updateAll(delta_time);
    
    // 更新技能冷却
    skill_manager->updateSkills(delta_time);

    // 更新玩家
    player->update(delta_time);
    
    // 使用碰撞管理器检查碰撞
    int collision_result = collision_manager->checkAllCollisions(player);
    if (collision_result == -1) {
        // 玩家被击中，游戏结束
        engine.requestEndGame(total_score, getDuration());
        return;
    } else if (collision_result > 0) {
        // 获得分数
        total_score += collision_result;
    }

    // 检查玩家是否死亡（如踩到带刺平台）
    if (player->isDead()) {
        engine.requestEndGame(total_score, getDuration());
        return;
    }

    // 检查玩家是否超出窗口边界
    if (player->getPosition().y < 0 || player->getPosition().y > window_size.y) {
        engine.requestEndGame(total_score, getDuration());
    }
}

void GameModel::initPlayer() {
    // 获取第一个平台来放置玩家
    auto& platforms = entity_manager->getPlatforms();
    if (!platforms.empty()) {
        auto first_platform = platforms.begin()->second;
        sf::Vector2f player_position = sf::Vector2f(
            first_platform->getPosition().x + first_platform->getSize().x / 2 - GameConfig::PLAYER_SIZE.x / 2,
            window_size.y / 5
        );
        player = new Entities::Player(player_position, GameConfig::PLAYER_SIZE, this);
    } else {
        // 如果没有平台，使用默认位置
        sf::Vector2f player_position = sf::Vector2f(window_size.x / 2, window_size.y / 2);
        player = new Entities::Player(player_position, GameConfig::PLAYER_SIZE, this);
    }
}

void GameModel::initGame() {
    total_score = 0;
    scroll_speed = GameConfig::DEFAULT_SCROLL_SPEED;
    game_time = 0;
    
    // 初始化生成器管理器
    generator_manager->initialize();
    generator_manager->setScrollSpeed(scroll_speed);
    
    // 生成初始平台
    generator_manager->generateInitialPlatforms();
    
    // 初始化玩家
    initPlayer();
    
    // 初始化技能
    skill_manager->initializeSkills();
}