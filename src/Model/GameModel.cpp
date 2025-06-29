#include <iostream>
#include <algorithm>
#include <vector>
#include "Model/GameModel.h"
#include "Core/Engine.h"

using Model::GameModel;
using Model::Entities::PlatformType;

GameModel::GameModel(Core::Engine &engine, sf::Vector2u window_size) : 
    Model(engine, window_size), init(false) {
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
    
    for (auto& pair : enemies) {
        if (pair.second) {
            delete pair.second;
        }
    }
    enemies.clear();
    
    for (auto& pair : bullets) {
        if (pair.second) {
            delete pair.second;
        }
    }
    bullets.clear();
    
    for (auto& pair : pickups) {
        if (pair.second) {
            delete pair.second;
        }
    }
    pickups.clear();
    
    for (auto& pair : arrows) {
        if (pair.second) {
            delete pair.second;
        }
    }
    arrows.clear();
    
    for (auto* skill : skills) {
        if (skill) {
            delete skill;
        }
    }
    skills.clear();
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

    // 更新敌人生成
    enemy_generate_interval -= delta_time;
    if (enemy_generate_interval <= 0.0f) {
        resetEnemyGenerateInterval();
        generateEnemy();
    }

    // 更新豆子生成
    pickup_generate_interval -= delta_time;
    if (pickup_generate_interval <= 0.0f) {
        resetPickupGenerateInterval();
        generatePickup();
    }

    for (auto it = platforms.begin(); it != platforms.end(); ) {
        Entities::Platform* platform = it->second;
        platform->update(delta_time);
        if (platform->outOfWindow(window_size)) {
            delete platform;
            // std::cout << "Platform " << it->first << " removed." << std::endl;
            it = platforms.erase(it);
        } else {
            ++it;
        }
    }
    
    // 更新敌人
    for (auto it = enemies.begin(); it != enemies.end(); ) {
        Entities::Enemy* enemy = it->second;
        enemy->update(delta_time);
        
        // 敌人射击
        if (enemy->canShoot()) {
            enemy->shoot();
        }
        
        if (enemy->outOfWindow(window_size)) {
            delete enemy;
            it = enemies.erase(it);
        } else {
            ++it;
        }
    }
    
    // 更新子弹
    for (auto it = bullets.begin(); it != bullets.end(); ) {
        Entities::Bullet* bullet = it->second;
        bullet->update(delta_time);
        if (bullet->outOfWindow(window_size)) {
            delete bullet;
            it = bullets.erase(it);
        } else {
            ++it;
        }
    }
    
    // 更新豆子
    for (auto it = pickups.begin(); it != pickups.end(); ) {
        Entities::Pickup* pickup = it->second;
        pickup->update(delta_time);
        if (pickup->outOfWindow(window_size)) {
            delete pickup;
            it = pickups.erase(it);
        } else {
            ++it;
        }
    }
    
    // 更新箭矢
    for (auto it = arrows.begin(); it != arrows.end(); ) {
        Entities::Arrow* arrow = it->second;
        arrow->update(delta_time);
        if (arrow->outOfWindow(window_size) || arrow->isExpired()) {
            delete arrow;
            it = arrows.erase(it);
        } else {
            ++it;
        }
    }
    
    // 更新技能
    for (auto* skill : skills) {
        skill->update(delta_time);
    }

    // std::cout << "player update start" << std::endl;
    player->update(delta_time);
    // std::cout << "player update end" << std::endl;
    
    // 检查碰撞
    checkCollisions();

    if (player->getPosition().y < 0 || player->getPosition().y > window_size.y) {
        engine.requestEndGame(total_score, getDuration());
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
    next_enemy_id = 0;
    next_bullet_id = 0;
    next_pickup_id = 0;
    next_arrow_id = 0;
    resetPlatformGenerateInterval();
    resetEnemyGenerateInterval();
    resetPickupGenerateInterval();
    initPlatforms();
    initPlayer();
    initSkills();
}

void GameModel::generateEnemy() {
    // 从窗口边界外随机生成敌人
    sf::Vector2f position;
    int side = rand() % 4; // 0:上, 1:右, 2:下, 3:左
    
    switch (side) {
        case 0: // 上边
            position = sf::Vector2f(rand() % window_size.x, -enemy_size.y);
            break;
        case 1: // 右边
            position = sf::Vector2f(window_size.x, rand() % window_size.y);
            break;
        case 2: // 下边
            position = sf::Vector2f(rand() % window_size.x, window_size.y);
            break;
        case 3: // 左边
            position = sf::Vector2f(-enemy_size.x, rand() % window_size.y);
            break;
    }
    
    enemies[next_enemy_id] = new Entities::Enemy(
        next_enemy_id, Entities::EnemyType::GHOST, position, enemy_size, this
    );
    next_enemy_id++;
}

void GameModel::resetEnemyGenerateInterval() {
    enemy_generate_interval = 3.0f + static_cast<float>(rand() % 2); // 3-5秒
}

void GameModel::createBullet(sf::Vector2f position, sf::Vector2f velocity) {
    bullets[next_bullet_id] = new Entities::Bullet(
        next_bullet_id, position, velocity, bullet_size
    );
    next_bullet_id++;
}

void GameModel::generatePickup() {
    Entities::PickupType type;
    sf::Vector2f position;
    int target_platform_id = -1;
    
    if (rand() % 100 < 75) {
        // 普通豆子 - 优先选择ID值高的平台
        type = Entities::PickupType::NORMAL_DOT;
        
        if (!platforms.empty()) {
            // 收集所有平台ID并按降序排序
            std::vector<int> platform_ids;
            for (const auto& platform_pair : platforms) {
                platform_ids.push_back(platform_pair.first);
            }
            
            std::sort(platform_ids.begin(), platform_ids.end(), 
                     [](int a, int b) { return a > b; });
            
            // 从前30%的高ID平台中随机选择一个
            int top_count = std::max(1, static_cast<int>(platform_ids.size() * 0.3));
            int selected_index = rand() % top_count;
            target_platform_id = platform_ids[selected_index];
            
            // 在选中的平台上方生成豆子
            auto platform_it = platforms.find(target_platform_id);
            if (platform_it != platforms.end()) {
                sf::Vector2f platform_pos = platform_it->second->getPosition();
                sf::Vector2f platform_size = platform_it->second->getSize();
                
                position = sf::Vector2f(
                    platform_pos.x + rand() % static_cast<int>(platform_size.x - pickup_size.x),
                    platform_pos.y - pickup_size.y - 10
                );
            }
        } else {
            position = sf::Vector2f(
                rand() % static_cast<int>(window_size.x - pickup_size.x),
                window_size.y + pickup_size.y
            );
        }
    } else {
        // 五角星豆子 - 从底部空中生成
        type = Entities::PickupType::STAR_DOT;
        position = sf::Vector2f(
            rand() % static_cast<int>(window_size.x - pickup_size.x),
            window_size.y + pickup_size.y
        );
    }
    
    pickups[next_pickup_id] = new Entities::Pickup(
        next_pickup_id, type, position, pickup_size, this, target_platform_id
    );
    next_pickup_id++;
}

void GameModel::resetPickupGenerateInterval() {
    pickup_generate_interval = 0.8f + static_cast<float>(rand() % 10) / 10.0f; // 0.8-1.8秒
}

void GameModel::checkCollisions() {
    // 检查子弹与玩家的碰撞
    for (auto& bullet_pair : bullets) {
        Entities::Bullet* bullet = bullet_pair.second;
        if (bullet->collidesWith(player->getPosition(), player->getSize())) {
            // 玩家被击中，游戏结束
            engine.requestEndGame(total_score, getDuration());
            return;
        }
    }
    
    // 检查豆子与玩家的碰撞
    for (auto it = pickups.begin(); it != pickups.end(); ) {
        Entities::Pickup* pickup = it->second;
        if (pickup->collidesWith(player->getPosition(), player->getSize())) {
            // 玩家吃到豆子，加分
            total_score += pickup->getScore();
            delete pickup;
            it = pickups.erase(it);
        } else {
            ++it;
        }
    }
    
    // 检查箭矢与敌人的碰撞
    for (auto arrow_it = arrows.begin(); arrow_it != arrows.end(); ) {
        Entities::Arrow* arrow = arrow_it->second;
        bool arrow_hit = false;
        
        for (auto enemy_it = enemies.begin(); enemy_it != enemies.end(); ) {
            Entities::Enemy* enemy = enemy_it->second;
            if (arrow->collidesWith(enemy->getPosition(), enemy->getSize())) {
                // 箭矢击中敌人，删除敌人和箭矢
                total_score += 5; // 击杀敌人得5分
                delete enemy;
                enemy_it = enemies.erase(enemy_it);
                arrow_hit = true;
                break;
            } else {
                ++enemy_it;
            }
        }
        
        if (arrow_hit) {
            delete arrow;
            arrow_it = arrows.erase(arrow_it);
        } else {
            ++arrow_it;
        }
    }
}

void GameModel::createArrow(sf::Vector2f position, sf::Vector2f velocity) {
    arrows[next_arrow_id] = new Entities::Arrow(
        next_arrow_id, position, velocity, arrow_size, this
    );
    next_arrow_id++;
}

void GameModel::initSkills() {
    // 初始化箭矢技能，冷却时间3秒
    skills.push_back(new Entities::Skill(Entities::SkillType::ARROW_SHOT, 3.0f));
}

void GameModel::useSkill(int skill_index) {
    if (skill_index >= 0 && skill_index < skills.size()) {
        Entities::Skill* skill = skills[skill_index];
        if (skill->canUse()) {
            skill->use();
            
            // 根据技能类型执行对应效果
            switch (skill->getType()) {
                case Entities::SkillType::ARROW_SHOT: {
                    // 从玩家位置发射箭矢
                    sf::Vector2f player_pos = player->getPosition();
                    sf::Vector2f arrow_pos;
                    sf::Vector2f arrow_velocity;
                    
                    // 根据玩家朝向决定箭矢发射方向
                    if (player->getFacingDirection() == Entities::FacingDirection::RIGHT) {
                        arrow_pos = sf::Vector2f(
                            player_pos.x + player_size.x,
                            player_pos.y + player_size.y / 2 - arrow_size.y / 2
                        );
                        arrow_velocity = sf::Vector2f(300.0f, 0.0f); // 水平向右
                    } else {
                        arrow_pos = sf::Vector2f(
                            player_pos.x - arrow_size.x,
                            player_pos.y + player_size.y / 2 - arrow_size.y / 2
                        );
                        arrow_velocity = sf::Vector2f(-300.0f, 0.0f); // 水平向左
                    }
                    
                    createArrow(arrow_pos, arrow_velocity);
                    break;
                }
            }
        }
    }
}