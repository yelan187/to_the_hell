#include <iostream>
#include "Model/GameModel.h"
#include "Model/Entities/Enemy.h"
#include "Model/Entities/Bullet.h"
#include "Model/Entities/Pickup.h"
#include "Model/Entities/Skill.h"
#include "Common/InternalNotification.h"
#include "Common/Config/Config.h"

using Model::GameModel;
using Model::Entities::PlatformType;

/*  本文件的各部分：

==================== 构造函数和析构函数 ====================
- GameModel()           // 构造函数，初始化游戏
- ~GameModel()          // 析构函数，清理所有资源（优化了重复代码）

==================== 游戏初始化方法 ====================
- initGame()            // 初始化游戏状态和参数
- initPlatforms()       // 创建初始平台
- initPlayer()          // 创建玩家并设置初始位置
- initSkills()          // 初始化技能系统
- resetPlatformGenerateInterval()  // 重置平台生成间隔

==================== 主要更新方法 ====================
- update()              // 主更新循环，处理所有游戏逻辑

==================== 实体生成方法 ====================
- generatePlatform()    // 生成新平台
- generateEnemy()       // 生成敌人
- generatePickup()      // 生成拾取物（豆子）
- createBullet()        // 创建子弹/箭矢

==================== 碰撞检测方法 ====================
- checkBulletPlayerCollisions()     // 检测子弹击中玩家
- checkPlayerBulletEnemyCollisions() // 检测玩家箭矢击中敌人
- checkPickupPlayerCollisions()     // 检测玩家拾取豆子

==================== 玩家控制方法 ====================
- playerJump()          // 玩家跳跃
- playerDown()          // 玩家下落
- playerWalkLeft()      // 玩家左移
- playerWalkRight()     // 玩家右移
- playerStopLeft()      // 停止左移
- playerStopRight()     // 停止右移

==================== 技能系统方法 ====================
- playerUseSkill()      // 使用技能（箭矢射击/冲刺）

==================== 工具方法 ====================
- getPlatformTypeRand() // 随机获取平台类型
- isPlatformPositionValid() // 验证平台位置有效性

==================== 通知方法 ====================
- fire()                // 发送游戏帧更新通知
- gameOver()            // 发送游戏结束通知

*/

// ==================== 构造函数和析构函数 ====================

GameModel::GameModel(sf::Vector2u window_size) : 
    Model(window_size), init(false) {
    initGame();
    init = true;
}

GameModel::~GameModel() {
    // 清理玩家
    delete player;
    player = nullptr;
    
    // 清理容器中的指针
    auto cleanup = [](auto& container) {
        for (auto& pair : container) {
            delete pair.second;
        }
        container.clear();
    };
    
    cleanup(platforms);
    cleanup(enemies);
    cleanup(bullets);
    cleanup(pickups);
    
    // 清理技能容器
    for (auto* skill : skills) {
        delete skill;
    }
    skills.clear();
}

// ==================== 游戏初始化方法 ====================

void GameModel::initGame() {
    total_score = 0;
    scroll_speed = Common::Config::GameConfig::INITIAL_SCROLL_SPEED;
    game_time = 0;
    resetPlatformGenerateInterval();
    enemy_generate_interval = Common::Config::GameConfig::ENEMY_GENERATE_INTERVAL;
    pickup_generate_interval = Common::Config::GameConfig::PICKUP_GENERATE_INTERVAL;
    initPlatforms();
    initPlayer();
    initSkills();
}

void GameModel::initPlatforms() {
    platforms[next_platform_id++] = new Entities::Platform(next_platform_id, PlatformType::NORMAL, 
        sf::Vector2f(-200, -500), sf::Vector2f(200,window_size.y + 500), 0);
    platforms[next_platform_id++] = new Entities::Platform(next_platform_id, PlatformType::NORMAL, 
        sf::Vector2f(window_size.x, -500), sf::Vector2f(200,window_size.y + 500), 0);
    
    const int initial_platforms = 3;
    for (int i = 0; i < initial_platforms; ++i) {
        sf::Vector2f position(
            static_cast<float>(rand() % static_cast<int>(window_size.x - platform_size.x)),
            static_cast<float>(window_size.y / 2 + (window_size.y * 2 / 3) / initial_platforms * i)
        );
        
        PlatformType type = PlatformType::NORMAL;
        platforms[next_platform_id] = new Entities::Platform(next_platform_id, type, position, platform_size, scroll_speed);
    }
    next_platform_id += initial_platforms;
}

void GameModel::initPlayer() {
    sf::Vector2f platform_pos = platforms[2]->getPosition();
    sf::Vector2f platform_size = platforms[2]->getSize();
    
    sf::Vector2f player_position = sf::Vector2f(
        platform_pos.x + platform_size.x / 2 - player_size.x / 2,
        platform_pos.y - player_size.y
    );
    
    player = new Entities::Player(player_position, player_size, this);
    player->setVelocity(platforms[2]->getVelocity());
}

void GameModel::initSkills() {
    for (auto* skill : skills) {
        delete skill;
    }
    skills.clear();
    
    skills.push_back(new Entities::Skill(Entities::SkillType::ARROW_SHOT, Common::Config::GameConfig::SKILL_ARROW_COOLDOWN));
    skills.push_back(new Entities::Skill(Entities::SkillType::SPRINT, Common::Config::GameConfig::SKILL_SPRINT_COOLDOWN));
    skills.push_back(new Entities::Skill(Entities::SkillType::GROUND_PENETRATION, Common::Config::GameConfig::SKILL_GROUND_PENETRATION_COOLDOWN));
}

void GameModel::resetPlatformGenerateInterval() {
    platform_generate_interval = Common::Config::GameConfig::PLATFORM_GENERATE_INTERVAL + 
                                 static_cast<float>(rand()) / RAND_MAX * (2 * Common::Config::GameConfig::PLATFORM_GENERATE_INTERVAL_VARIANCE) - Common::Config::GameConfig::PLATFORM_GENERATE_INTERVAL_VARIANCE;
}

// ==================== 主要更新方法 ====================

void GameModel::update(float delta_time) {
    if (!init) {
        initGame();
        init = true;
        return;
    }
    auto prev_time = game_time;
    game_time += delta_time;
    if (static_cast<int>(game_time) % Common::Config::GameConfig::SCORE_UPDATE_INTERVAL 
        - static_cast<int>(prev_time)%Common::Config::GameConfig::SCORE_UPDATE_INTERVAL >= 1) 
    {
        total_score += Common::Config::GameConfig::SCORE_INCREMENT;
    }

    // 平台生成和更新
    platform_generate_interval -= delta_time;
    if (platform_generate_interval <= 0.0f) {
        resetPlatformGenerateInterval();
        generatePlatform();
    }

    for (auto it = platforms.begin(); it != platforms.end(); ) {
        Entities::Platform* platform = it->second;
        platform->update(delta_time);
        
        if (platform->isBroken()) {
            delete platform;
            it = platforms.erase(it);
        } else if (platform->outOfWindow(window_size)) {
            delete platform;
            it = platforms.erase(it);
        } else {
            ++it;
        }
    }
    
    // 玩家更新
    player->update(delta_time);
    
    // 敌人生成和更新
    enemy_generate_interval -= delta_time;
    if (enemy_generate_interval <= 0.0f) {
        generateEnemy();
        enemy_generate_interval = Common::Config::GameConfig::ENEMY_SPAWN_MIN_INTERVAL + 
                                 static_cast<float>(rand() % static_cast<int>(Common::Config::GameConfig::ENEMY_SPAWN_MAX_INTERVAL - Common::Config::GameConfig::ENEMY_SPAWN_MIN_INTERVAL));
    }
    
    for (auto it = enemies.begin(); it != enemies.end(); ) {
        Entities::Enemy* enemy = it->second;
        enemy->update(delta_time);
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
    
    // 子弹更新
    for (auto it = bullets.begin(); it != bullets.end(); ) {
        Entities::Bullet* bullet = it->second;
        bullet->update(delta_time, scroll_speed);
        if (bullet->outOfWindow(window_size)) {
            delete bullet;
            it = bullets.erase(it);
        } else {
            ++it;
        }
    }
    
    // 豆子生成和更新
    pickup_generate_interval -= delta_time;
    if (pickup_generate_interval <= 0.0f) {
        generatePickup();
        pickup_generate_interval = Common::Config::GameConfig::PICKUP_SPAWN_MIN_INTERVAL + 
                                  static_cast<float>(rand()) / RAND_MAX * 
                                  (Common::Config::GameConfig::PICKUP_SPAWN_MAX_INTERVAL - Common::Config::GameConfig::PICKUP_SPAWN_MIN_INTERVAL);
    }
    
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
    
    // 技能更新
    for (auto* skill : skills) {
        skill->update(delta_time);
    }
    
    // 碰撞检测
    if (checkBulletPlayerCollisions()) {
        player->setDead(true);
        gameOver();
        return;
    }
    
    if (player && player->isDead()) {
        gameOver();
        return;
    }
    
    checkPlayerBulletEnemyCollisions();
    
    int score_gained = checkPickupPlayerCollisions();
    total_score += score_gained;
    
    // 边界检查
    if (player->getPosition().y + player->getSize().y >= window_size.y) {
        gameOver();
        return;
    }
    
    fire();
}

// ==================== 实体生成方法 ====================

void GameModel::generatePlatform() {
    int max_attempts = Common::Config::GameConfig::PLATFORM_GENERATION_MAX_ATTEMPTS;
    
    for (int attempt = 0; attempt < max_attempts; ++attempt) {
        sf::Vector2f position(
            static_cast<float>(rand() % static_cast<int>(window_size.x - platform_size.x)),
            window_size.y
        );
        
        if (isPlatformPositionValid(position, platform_size)) {
            PlatformType type = getPlatformTypeRand();
            platforms[next_platform_id++] = new Entities::Platform(
                next_platform_id, type, position, platform_size, scroll_speed);
            return;
        }
    }
}

void GameModel::generateEnemy() {
    sf::Vector2f enemy_size = Common::Config::GameConfig::ENEMY_SIZE;
    sf::Vector2f position;
    
    int side = rand() % 4;
    switch (side) {
        case 0: position = sf::Vector2f(rand() % window_size.x, -enemy_size.y); break;
        case 1: position = sf::Vector2f(window_size.x, rand() % window_size.y); break;
        case 2: position = sf::Vector2f(rand() % window_size.x, window_size.y); break;
        case 3: position = sf::Vector2f(-enemy_size.x, rand() % window_size.y); break;
    }
    
    enemies[next_enemy_id] = new Entities::Enemy(next_enemy_id, Entities::EnemyType::GHOST, position, enemy_size, this);
    next_enemy_id++;
}

void GameModel::generatePickup() {
    sf::Vector2f pickup_size = Common::Config::GameConfig::PICKUP_SIZE;
    int random_chance = rand() % 100;
    bool is_star = random_chance < (Common::Config::GameConfig::PICKUP_STAR_PROBABILITY * 100);
    
    if (is_star) {
        sf::Vector2f position(
            rand() % static_cast<int>(window_size.x - pickup_size.x),
            window_size.y
        );
        pickups[next_pickup_id] = new Entities::Pickup(
            next_pickup_id, Entities::PickupType::STAR_DOT, position, pickup_size, this, -1
        );
        next_pickup_id++;
    } else {
        if (!platforms.empty()) {
            int selected_platform_id = -1;
            float lowest_y = -1.0f;
            
            for (const auto& platform_pair : platforms) {
                sf::Vector2f platform_pos = platform_pair.second->getPosition();
                if (platform_pos.y > lowest_y) {
                    lowest_y = platform_pos.y;
                    selected_platform_id = platform_pair.first;
                }
            }
            
            if (selected_platform_id != -1) {
                sf::Vector2f platform_pos = platforms[selected_platform_id]->getPosition();
                sf::Vector2f platform_size = platforms[selected_platform_id]->getSize();
                sf::Vector2f position(
                    platform_pos.x + rand() % static_cast<int>(platform_size.x - pickup_size.x),
                    platform_pos.y - pickup_size.y
                );
                
                pickups[next_pickup_id] = new Entities::Pickup(
                    next_pickup_id, Entities::PickupType::NORMAL_DOT, position, pickup_size, this, selected_platform_id
                );
                next_pickup_id++;
            }
        }
    }
}

void GameModel::createBullet(sf::Vector2f position, sf::Vector2f velocity, bool is_player_bullet) {
    sf::Vector2f bullet_size = is_player_bullet ? 
        Common::Config::GameConfig::BULLET_SIZE : 
        sf::Vector2f(Common::Config::GameConfig::BULLET_SIZE.x * 0.6f, Common::Config::GameConfig::BULLET_SIZE.y * 0.6f);
    
    bullets[next_bullet_id] = new Entities::Bullet(next_bullet_id, position, velocity, bullet_size, is_player_bullet);
    next_bullet_id++;
}

// ==================== 碰撞检测方法 ====================

bool GameModel::checkBulletPlayerCollisions() {
    for (const auto& bullet_pair : bullets) {
        if (!bullet_pair.second->isPlayerBullet()) {
            sf::Vector2f player_pos = player->getPosition();
            sf::Vector2f player_size = player->getSize();
            
            float shrink_amount = player_size.x * Common::Config::GameConfig::PLAYER_COLLISION_SHRINK_RATIO;
            sf::Vector2f effective_player_pos(player_pos.x + shrink_amount, player_pos.y);
            sf::Vector2f effective_player_size(player_size.x - 2 * shrink_amount, player_size.y);
            
            if (bullet_pair.second->collidesWith(effective_player_pos, effective_player_size)) {
                return true;
            }
        }
    }
    return false;
}

void GameModel::checkPlayerBulletEnemyCollisions() {
    for (auto bullet_it = bullets.begin(); bullet_it != bullets.end(); ) {
        if (bullet_it->second->isPlayerBullet()) {
            bool hit_enemy = false;
            for (auto enemy_it = enemies.begin(); enemy_it != enemies.end(); ) {
                if (bullet_it->second->collidesWith(enemy_it->second->getPosition(), enemy_it->second->getSize())) {
                    total_score += Common::Config::GameConfig::ENEMY_SCORE_VALUE;
                    delete enemy_it->second;
                    enemy_it = enemies.erase(enemy_it);
                    hit_enemy = true;
                    break;
                } else {
                    ++enemy_it;
                }
            }
            
            if (hit_enemy) {
                delete bullet_it->second;
                bullet_it = bullets.erase(bullet_it);
            } else {
                ++bullet_it;
            }
        } else {
            ++bullet_it;
        }
    }
}

int GameModel::checkPickupPlayerCollisions() {
    int score_gained = 0;
    for (auto it = pickups.begin(); it != pickups.end(); ) {
        if (it->second->collidesWith(player->getPosition(), player->getSize())) {
            score_gained += it->second->getScore();
            delete it->second;
            it = pickups.erase(it);
        } else {
            ++it;
        }
    }
    return score_gained;
}

// ==================== 玩家控制方法 ====================

void GameModel::playerJump() {
    player->jump(scroll_speed);
}

void GameModel::playerDown() {
    if (!player->isOnPlatform()) {
        player->fall();
    } else {
        playerUseSkill(2); // 使用冲刺技能
    }
}

void GameModel::playerWalkLeft() {
    player->walkLeft();
}

void GameModel::playerWalkRight() {
    player->walkRight();
}

void GameModel::playerStopLeft() {
    player->stopLeft();
}

void GameModel::playerStopRight() {
    player->stopRight();
}

// ==================== 技能系统方法 ====================

void GameModel::playerUseSkill(int skill_id, sf::Vector2f direction) {
    if (!player) return;
    
    if (skill_id < 0 || skill_id >= static_cast<int>(skills.size())) return;
    if (!skills[skill_id]->canUse()) return;
    
    skills[skill_id]->use();
    sf::Vector2f player_facing = player->getFacingDirection();
    
    switch (skill_id) {
        case 0: // ARROW_SHOT
        {
            sf::Vector2f player_pos = player->getPosition();
            sf::Vector2f arrow_pos = sf::Vector2f(
                player_pos.x + player_size.x / 2, 
                player_pos.y + player_size.y / 2 - Common::Config::GameConfig::BULLET_SIZE.y / 2
            );
            
            sf::Vector2f arrow_velocity = sf::Vector2f(player_facing.x * Common::Config::GameConfig::BULLET_SPEED, 0.0f);
            createBullet(arrow_pos, arrow_velocity, true);
            break;
        }
        case 1: // SPRINT
        {
            sf::Vector2f sprint_replacement = sf::Vector2f(
                player_facing.x * Common::Config::GameConfig::SKILL_SPRINT_DISTANCE, 
                0.0f
            );
            player->updatePosition(0.0f, sprint_replacement);
            break;
        }
        case 2:
        {
            player->groundPenetration();
            break;
        }
        default:
            break;
    }
}

// ==================== 工具方法 ====================

Model::Entities::PlatformType GameModel::getPlatformTypeRand() {
    float random = static_cast<float>(rand()) / RAND_MAX;
    float cumulative = 0.0f;
    
    cumulative += Common::Config::GameConfig::PLATFORM_NORMAL_PROBABILITY;
    if (random < cumulative) return Entities::PlatformType::NORMAL;
    
    cumulative += Common::Config::GameConfig::PLATFORM_ROLLING_PROBABILITY;
    if (random < cumulative) return Entities::PlatformType::ROLLING;
    
    cumulative += Common::Config::GameConfig::PLATFORM_BOUNCY_PROBABILITY;
    if (random < cumulative) return Entities::PlatformType::BOUNCY;
    
    cumulative += Common::Config::GameConfig::PLATFORM_FRAGILE_PROBABILITY;
    if (random < cumulative) return Entities::PlatformType::FRAGILE;
    
    return Entities::PlatformType::SPIKED;
}

bool GameModel::isPlatformPositionValid(sf::Vector2f position, sf::Vector2f size) {
    float min_vertical_spacing = Common::Config::GameConfig::PLAYER_SIZE.y + 20.0f;
    float min_horizontal_spacing = Common::Config::GameConfig::PLAYER_SIZE.x + 30.0f;
    
    float new_left = position.x;
    float new_right = position.x + size.x;
    float new_top = position.y;
    float new_bottom = position.y + size.y;
    
    for (const auto& [id, platform] : platforms) {
        if (!platform) continue;
        
        sf::Vector2f existing_pos = platform->getPosition();
        sf::Vector2f existing_size = platform->getSize();
        
        float existing_left = existing_pos.x;
        float existing_right = existing_pos.x + existing_size.x;
        float existing_top = existing_pos.y;
        float existing_bottom = existing_pos.y + existing_size.y;
        
        float vertical_distance = 0.0f;
        bool vertical_overlap = false;
        
        if (new_bottom < existing_top) {
            vertical_distance = existing_top - new_bottom;
        } else if (new_top > existing_bottom) {
            vertical_distance = new_top - existing_bottom;
        } else {
            vertical_overlap = true;
        }
        
        float horizontal_distance = 0.0f;
        bool horizontal_overlap = false;
        
        if (new_right < existing_left) {
            horizontal_distance = existing_left - new_right;
        } else if (new_left > existing_right) {
            horizontal_distance = new_left - existing_right;
        } else {
            horizontal_overlap = true;
        }
        
        if (vertical_overlap && horizontal_overlap) {
            return false;
        }
        
        if (vertical_overlap && horizontal_distance < min_horizontal_spacing) {
            return false;
        }
        
        if (horizontal_overlap && vertical_distance < min_vertical_spacing) {
            return false;
        }
    }
    
    return true;
}

// ==================== 通知方法 ====================

void GameModel::fire() {
    Common::_ChangeGameFrameParam* param = new Common::_ChangeGameFrameParam();
    param->id = Common::NotificationId::_ChangeGameFrame;
    param->value.player_info.position = player->getPosition();
    param->value.player_info.size = player->getSize();
    param->value.player_info.state = player->getState();
    
    // 平台信息
    std::vector<int> platforms_id;
    std::map<int, Common::_FrameInfo::PlatformInfo> platforms_info;
    for (const auto& pair : platforms) {
        platforms_id.push_back(pair.first);
        platforms_info[pair.first].position = pair.second->getPosition();
        platforms_info[pair.first].type = pair.second->type;
        platforms_info[pair.first].rolling_direction = pair.second->getRollingDirection();
    }
    param->value.platforms_id = platforms_id;
    param->value.platforms_info = platforms_info;
    
    // 敌人信息
    std::vector<int> enemies_id;
    std::map<int, Common::_FrameInfo::EnemyInfo> enemies_info;
    for (const auto& pair : enemies) {
        enemies_id.push_back(pair.first);
        enemies_info[pair.first].position = pair.second->getPosition();
        enemies_info[pair.first].size = pair.second->getSize();
        enemies_info[pair.first].type = pair.second->getType();
        enemies_info[pair.first].facing_direction = pair.second->getFacingDirection();
    }
    param->value.enemies_id = enemies_id;
    param->value.enemies_info = enemies_info;
    
    // 子弹信息
    std::vector<int> bullets_id;
    std::map<int, Common::_FrameInfo::BulletInfo> bullets_info;
    for (const auto& pair : bullets) {
        bullets_id.push_back(pair.first);
        bullets_info[pair.first].position = pair.second->getPosition();
        bullets_info[pair.first].size = pair.second->getSize();
        bullets_info[pair.first].is_player_bullet = pair.second->isPlayerBullet();
        bullets_info[pair.first].velocity = pair.second->getVelocity();
    }
    param->value.bullets_id = bullets_id;
    param->value.bullets_info = bullets_info;
    
    // 豆子信息
    std::vector<int> pickups_id;
    std::map<int, Common::_FrameInfo::PickupInfo> pickups_info;
    for (const auto& pair : pickups) {
        pickups_id.push_back(pair.first);
        pickups_info[pair.first].position = pair.second->getPosition();
        pickups_info[pair.first].size = pair.second->getSize();
        pickups_info[pair.first].type = pair.second->getType();
    }
    param->value.pickups_id = pickups_id;
    param->value.pickups_info = pickups_info;
    
    // 技能信息
    std::vector<Common::_FrameInfo::SkillInfo> skills_info;
    for (size_t i = 0; i < skills.size(); ++i) {
        Common::_FrameInfo::SkillInfo skill_info;
        skill_info.skill_type = static_cast<int>(skills[i]->getType());
        skill_info.cooldown_progress = skills[i]->getCooldownProgress();
        skill_info.is_available = skills[i]->isAvailable();
        skills_info.push_back(skill_info);
    }
    param->value.skills_info = skills_info;
    
    trigger.fire(param);
    delete param;
}

void GameModel::gameOver() {
    Common::GameOverNotificationParam* param = new Common::GameOverNotificationParam();
    param->id = Common::NotificationId::GameOver;
    param->value.total_score = total_score;
    param->value.game_time = std::chrono::seconds(static_cast<int>(game_time));
    trigger.fire(param);
    delete param;
}
