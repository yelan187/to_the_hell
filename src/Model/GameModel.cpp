#include <iostream>
#include "Model/GameModel.h"
#include "Model/Entities/Enemy.h"
#include "Model/Entities/Bullet.h"
#include "Model/Entities/Pickup.h"
#include "Model/Entities/Skill.h"

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
    
    // 清理技能
    for (auto* skill : skills) {
        if (skill) {
            delete skill;
        }
    }
    skills.clear();
}

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

void GameModel::update(float delta_time) {
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

    for (auto it = platforms.begin(); it != platforms.end(); ) {
        Entities::Platform* platform = it->second;
        platform->update(delta_time);
        
        // 检查脆弱平台是否破碎
        if (platform->isBroken()) {
            // 如果玩家在这个平台上，让玩家掉落
            if (player && player->getOnPlatformId() == platform->id) {
                player->fall(); // 让玩家开始下落
            }
            delete platform;
            it = platforms.erase(it);
        } else if (platform->outOfWindow(window_size)) {
            delete platform;
            it = platforms.erase(it);
        } else {
            ++it;
        }
    }
    
    player->update(delta_time);
    
    // 更新敌人
    enemy_generate_interval -= delta_time;
    if (enemy_generate_interval <= 0.0f) {
        generateEnemy();
        enemy_generate_interval = 3.0f + static_cast<float>(rand() % 3);
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
    
    // 更新子弹
    for (auto it = bullets.begin(); it != bullets.end(); ) {
        Entities::Bullet* bullet = it->second;
        bullet->update(delta_time, scroll_speed);  // 传入滚动速度
        if (bullet->outOfWindow(window_size)) {
            delete bullet;
            it = bullets.erase(it);
        } else {
            ++it;
        }
    }
    
    // 更新豆子
    pickup_generate_interval -= delta_time;
    if (pickup_generate_interval <= 0.0f) {
        generatePickup();
        pickup_generate_interval = 2.0f + static_cast<float>(rand() % 2);
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
    
    // 更新技能冷却
    for (auto* skill : skills) {
        skill->update(delta_time);
    }
    
    // 碰撞检测
    if (checkBulletPlayerCollisions()) {
        player->setDead(true);
        gameOver();
        return;
    }
    
    // 检查玩家是否死亡（例如踩到带刺平台）
    if (player && player->isDead()) {
        gameOver();
        return;
    }
    
    // 检查玩家箭矢击中敌人
    checkPlayerBulletEnemyCollisions();
    
    int score_gained = checkPickupPlayerCollisions();
    total_score += score_gained;
    
    fire();
    if (player->getPosition().y <= 0 || player->getPosition().y + player->getSize().y >= window_size.y) {
        std::cout << "Player out of bounds, resetting game." << std::endl;
        gameOver();
    }
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
        
        // 初始平台都设为普通平台，确保玩家安全开始游戏
        PlatformType type = PlatformType::NORMAL;

        platforms[id] = new Entities::Platform(id, type, position, platform_size, scroll_speed);
    }
    next_platform_id = initial_platforms;
}

void GameModel::initPlayer() {
    sf::Vector2f player_position = sf::Vector2f(
        platforms[0]->getPosition().x + platforms[0]->getSize().x / 2 - player_size.x / 2,
        window_size.y / 5
    );
    player = new Entities::Player(player_position, player_size, this);
}

void GameModel::resetPlatformGenerateInterval() {
    platform_generate_interval = 1.0f + static_cast<float>(rand() % 1 - 0.5) / 2;
}

void GameModel::initGame() {
    total_score = 0;
    scroll_speed = 100.0f;
    game_time = 0;
    resetPlatformGenerateInterval();
    enemy_generate_interval = 5.0f;
    pickup_generate_interval = 3.0f;
    initPlatforms();
    initPlayer();
    initSkills();
}

void GameModel::createBullet(sf::Vector2f position, sf::Vector2f velocity, bool is_player_bullet) {
    sf::Vector2f bullet_size;
    if (is_player_bullet) {
        // 玩家箭矢：更长更薄的形状
        bullet_size = sf::Vector2f(16, 4);
    } else {
        // 敌人子弹：小圆形
        bullet_size = sf::Vector2f(6, 6);
    }
    bullets[next_bullet_id] = new Entities::Bullet(next_bullet_id, position, velocity, bullet_size, is_player_bullet);
    next_bullet_id++;
}

void GameModel::generateEnemy() {
    sf::Vector2f enemy_size(40, 40);
    sf::Vector2f position;
    
    int side = rand() % 4;
    switch (side) {
        case 0:
            position = sf::Vector2f(rand() % window_size.x, -enemy_size.y);
            break;
        case 1:
            position = sf::Vector2f(window_size.x, rand() % window_size.y);
            break;
        case 2:
            position = sf::Vector2f(rand() % window_size.x, window_size.y);
            break;
        case 3:
            position = sf::Vector2f(-enemy_size.x, rand() % window_size.y);
            break;
    }
    
    enemies[next_enemy_id] = new Entities::Enemy(next_enemy_id, Entities::EnemyType::GHOST, position, enemy_size, this);
    next_enemy_id++;
}

void GameModel::generatePickup() {
    sf::Vector2f pickup_size(15, 15);
    
    bool is_star = (rand() % 100) < 20;
    
    if (is_star) {
        sf::Vector2f position(
            rand() % static_cast<int>(window_size.x - pickup_size.x),
            window_size.y + pickup_size.y
        );
        pickups[next_pickup_id] = new Entities::Pickup(
            next_pickup_id, Entities::PickupType::STAR_DOT, position, pickup_size, this, -1
        );
    } else {
        if (!platforms.empty()) {
            auto it = platforms.begin();
            std::advance(it, rand() % platforms.size());
            int platform_id = it->first;
            
            sf::Vector2f platform_pos = it->second->getPosition();
            sf::Vector2f platform_size = it->second->getSize();
            sf::Vector2f position(
                platform_pos.x + rand() % static_cast<int>(platform_size.x - pickup_size.x),
                platform_pos.y - pickup_size.y
            );
            
            pickups[next_pickup_id] = new Entities::Pickup(
                next_pickup_id, Entities::PickupType::NORMAL_DOT, position, pickup_size, this, platform_id
            );
        }
    }
    next_pickup_id++;
}

bool GameModel::checkBulletPlayerCollisions() {
    for (const auto& bullet_pair : bullets) {
        // 只检查敌人的子弹，不检查玩家的箭矢
        if (!bullet_pair.second->isPlayerBullet() && 
            bullet_pair.second->collidesWith(player->getPosition(), player->getSize())) {
            return true;
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
                    // 玩家箭矢击中敌人，删除敌人和子弹
                    total_score += 50;  // 击杀敌人得分
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

// 玩家控制方法实现
void GameModel::playerJump() {
    player->jump(scroll_speed);
}

void GameModel::playerDown() {
    player->fall();
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

// 技能系统实现：支持箭矢射击和冲刺技能
void GameModel::playerUseSkill(int skill_id, sf::Vector2f direction) {
    if (!player) return;
    
    // 检查技能是否存在且可用
    if (skill_id < 0 || skill_id >= static_cast<int>(skills.size())) return;
    if (!skills[skill_id]->canUse()) return;
    
    // 使用技能（启动冷却）
    skills[skill_id]->use();
    
    // 使用玩家的面向方向而不是传入的固定方向
    sf::Vector2f player_facing = player->getFacingDirection();
    
    switch (skill_id) {
        case 0: // ARROW_SHOT - 箭矢射击技能
        {
            sf::Vector2f player_pos = player->getPosition();
            sf::Vector2f arrow_pos = sf::Vector2f(
                player_pos.x + player_size.x / 2, 
                player_pos.y + player_size.y / 2 - 4.0f  // 箭矢高度的一半
            );
            
            // 箭矢必须水平飞行，Y速度为0，抵消滚动影响
            sf::Vector2f arrow_velocity = sf::Vector2f(player_facing.x * 400.0f, 0.0f);
            createBullet(arrow_pos, arrow_velocity, true);  // 玩家箭矢
            break;
        }
        case 1: // SPRINT - 冲刺技能
        {
            sf::Vector2f player_pos = player->getPosition();
            float sprint_distance = 100.0f; // 冲刺距离
            
            // 根据玩家面向方向进行冲刺
            sf::Vector2f target_pos = sf::Vector2f(
                player_pos.x + player_facing.x * sprint_distance,
                player_pos.y
            );
            
            // 确保不超出窗口边界
            if (target_pos.x < 0) {
                target_pos.x = 0;
            } else if (target_pos.x + player_size.x > window_size.x) {
                target_pos.x = window_size.x - player_size.x;
            }
            
            player->setPosition(target_pos);
            break;
        }
        default:
            break;
    }
}

// 实现缺失的方法
Model::Entities::PlatformType GameModel::getPlatformTypeRand() {
    int random = rand() % 100;
    
    if (random < 50) {
        return Entities::PlatformType::NORMAL;      // 50% 普通平台
    } else if (random < 65) {
        return Entities::PlatformType::ROLLING;     // 15% 滚动平台
    } else if (random < 80) {
        return Entities::PlatformType::BOUNCY;      // 15% 弹跳平台
    } else if (random < 95) {
        return Entities::PlatformType::FRAGILE;     // 15% 脆弱平台
    } else {
        return Entities::PlatformType::SPIKED;      // 5% 带刺平台（最危险，概率最低）
    }
}

// 初始化技能系统
void GameModel::initSkills() {
    // 清理旧技能
    for (auto* skill : skills) {
        delete skill;
    }
    skills.clear();
    
    // 创建两个技能：箭矢射击和冲刺
    skills.push_back(new Entities::Skill(Entities::SkillType::ARROW_SHOT, 2.0f));  // 2秒冷却
    skills.push_back(new Entities::Skill(Entities::SkillType::SPRINT, 5.0f));      // 5秒冷却
}


