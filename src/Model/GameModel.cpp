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
    int max_attempts = Common::Config::GameConfig::PLATFORM_GENERATION_MAX_ATTEMPTS;
    
    for (int attempt = 0; attempt < max_attempts; ++attempt) {
        // 随机生成位置
        sf::Vector2f position(
            static_cast<float>(rand() % static_cast<int>(window_size.x - platform_size.x)),
            window_size.y
        );
        
        // 检查位置是否有效（无碰撞）
        if (isPlatformPositionValid(position, platform_size)) {
            PlatformType type = getPlatformTypeRand();
            
            platforms[next_platform_id++] = new Entities::Platform(
                next_platform_id, type, position, platform_size, scroll_speed);
            return; // 成功生成平台，退出
        }
    }
    
    // 如果所有尝试都失败，本次不生成平台
    // 可以在这里添加日志记录，表示平台生成失败
}

void GameModel::initPlatforms() {
    const int initial_platforms = Common::Config::GameConfig::INITIAL_PLATFORM_COUNT;
    for (int i = 0; i < initial_platforms; ++i) {
        int id = i;
        
        // 调整初始平台位置到页面中间部分，增加垂直间隔
        sf::Vector2f position(
            static_cast<float>(rand() % static_cast<int>(window_size.x - platform_size.x)),
            static_cast<float>(window_size.y / 2 + (window_size.y * 2 / 3) / initial_platforms * i)
        );
        
        // 初始平台都设为普通平台，确保玩家安全开始游戏
        PlatformType type = PlatformType::NORMAL;

        platforms[id] = new Entities::Platform(id, type, position, platform_size, scroll_speed);
    }
    next_platform_id = initial_platforms;
}

void GameModel::initPlayer() {
    sf::Vector2f platform_pos = platforms[0]->getPosition();
    sf::Vector2f platform_size = platforms[0]->getSize();
    
    // 玩家应该站在平台的顶部
    sf::Vector2f player_position = sf::Vector2f(
        platform_pos.x + platform_size.x / 2 - player_size.x / 2,
        platform_pos.y - player_size.y  // 玩家底部与平台顶部对齐
    );
    
    std::cout << "Platform 0 position: (" << platform_pos.x << ", " << platform_pos.y << ")" << std::endl;
    std::cout << "Platform 0 size: (" << platform_size.x << ", " << platform_size.y << ")" << std::endl;
    std::cout << "Player initial position: (" << player_position.x << ", " << player_position.y << ")" << std::endl;
    std::cout << "Player size: (" << player_size.x << ", " << player_size.y << ")" << std::endl;
    
    player = new Entities::Player(player_position, player_size, this);
    
    
    // 重要：设置玩家的初始速度为平台速度，避免相对运动
    player->setVelocity(platforms[0]->getVelocity());
    
    std::cout << "Player set on platform 0 with velocity: (" << platforms[0]->getVelocity().x << ", " << platforms[0]->getVelocity().y << ")" << std::endl;
}

void GameModel::resetPlatformGenerateInterval() {
    platform_generate_interval = Common::Config::GameConfig::PLATFORM_GENERATE_INTERVAL + 
                                 static_cast<float>(rand()) / RAND_MAX * (2 * Common::Config::GameConfig::GENERATE_INTERVAL_VARIANCE) - Common::Config::GameConfig::GENERATE_INTERVAL_VARIANCE; // ±配置的随机变化
}

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

void GameModel::createBullet(sf::Vector2f position, sf::Vector2f velocity, bool is_player_bullet) {
    sf::Vector2f bullet_size;
    if (is_player_bullet) {
        // 玩家箭矢：使用配置的子弹尺寸
        bullet_size = Common::Config::GameConfig::BULLET_SIZE;
    } else {
        // 敌人子弹：小圆形，稍小一些
        bullet_size = sf::Vector2f(Common::Config::GameConfig::BULLET_SIZE.x * Common::Config::GameConfig::BULLET_SIZE_SCALE, Common::Config::GameConfig::BULLET_SIZE.y * Common::Config::GameConfig::BULLET_SIZE_SCALE);
    }
    bullets[next_bullet_id] = new Entities::Bullet(next_bullet_id, position, velocity, bullet_size, is_player_bullet);
    next_bullet_id++;
}

void GameModel::generateEnemy() {
    sf::Vector2f enemy_size = Common::Config::GameConfig::ENEMY_SIZE;
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
    sf::Vector2f pickup_size = Common::Config::GameConfig::PICKUP_SIZE;
    
    int random_chance = rand() % 100;
    bool is_star = random_chance < (Common::Config::GameConfig::PICKUP_STAR_PROBABILITY * 100); // 使用配置的概率
    
    std::cout << "Pickup generation: random=" << random_chance << ", is_star=" << is_star << std::endl;
    
    if (is_star) {
        // 星形豆子从下方进入页面，像平台一样
        sf::Vector2f position(
            rand() % static_cast<int>(window_size.x - pickup_size.x),
            window_size.y  // 从页面底部进入
        );
        pickups[next_pickup_id] = new Entities::Pickup(
            next_pickup_id, Entities::PickupType::STAR_DOT, position, pickup_size, this, -1
        );
        next_pickup_id++;
        std::cout << "Generated star pickup at (" << position.x << ", " << position.y << ") with ID " << (next_pickup_id-1) << std::endl;
    } else {
        if (!platforms.empty()) {
            // 寻找位置最低的平台（y坐标最大的）
            int selected_platform_id = -1;
            float lowest_y = -1.0f;
            
            for (const auto& platform_pair : platforms) {
                sf::Vector2f platform_pos = platform_pair.second->getPosition();
                // 选择y坐标最大的平台（位置最低）
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
                std::cout << "Generated normal pickup at (" << position.x << ", " << position.y << ") with ID " << (next_pickup_id-1) << std::endl;
            } else {
                std::cout << "No platform found for normal pickup" << std::endl;
            }
        } else {
            std::cout << "No platforms available for pickup generation" << std::endl;
        }
    }
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
                    total_score += Common::Config::GameConfig::ENEMY_SCORE_VALUE;  // 击杀敌人得分
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
                player_pos.y + player_size.y / 2 - Common::Config::GameConfig::BULLET_SIZE.y / 2  // 箭矢高度的一半
            );
            
            // 箭矢必须水平飞行，Y速度为0，抵消滚动影响
            sf::Vector2f arrow_velocity = sf::Vector2f(player_facing.x * Common::Config::GameConfig::BULLET_SPEED, 0.0f);
            createBullet(arrow_pos, arrow_velocity, true);  // 玩家箭矢
            break;
        }
        case 1: // SPRINT - 冲刺技能
        {
            sf::Vector2f player_pos = player->getPosition();
            float sprint_distance = Common::Config::GameConfig::SKILL_SPRINT_DISTANCE; // 冲刺距离
            
            // 获取玩家当前所在的平台ID（如果有的话）
            int current_platform_id = player->getOnPlatformId();
            
            // 分步检测冲刺路径上的碰撞
            float step_size = 2.0f; // 每步2像素，更精确的检测
            float current_distance = 0.0f;
            sf::Vector2f final_pos = player_pos;
            
            while (current_distance < sprint_distance) {
                sf::Vector2f test_pos = sf::Vector2f(
                    player_pos.x + player_facing.x * (current_distance + step_size),
                    player_pos.y
                );
                
                // 检查窗口边界
                if (test_pos.x < 0 || test_pos.x + player_size.x > window_size.x) {
                    break;
                }
                
                // 检查与平台的碰撞（忽略当前站立的平台）
                bool collision = false;
                
                for (const auto& platform_pair : platforms) {
                    Entities::Platform* platform = platform_pair.second;
                    
                    // 忽略当前站立的平台，允许在其上移动
                    if (current_platform_id != -1 && platform->id == current_platform_id) {
                        continue;
                    }
                    
                    // 使用Player的碰撞检测方法来检查是否会发生碰撞
                    if (player->collisionDetection(platform, test_pos)) {
                        collision = true;
                        break;
                    }
                }
                
                if (collision) {
                    break;
                }
                
                final_pos = test_pos;
                current_distance += step_size;
            }
            
            // 确保最终位置不会超出窗口边界
            if (final_pos.x < 0) {
                final_pos.x = 0;
            } else if (final_pos.x + player_size.x > window_size.x) {
                final_pos.x = window_size.x - player_size.x;
            }
            
            if (final_pos.y < 0) {
                final_pos.y = 0;
            } else if (final_pos.y + player_size.y > window_size.y) {
                final_pos.y = window_size.y - player_size.y;
            }
            
            player->setPosition(final_pos);
            break;
        }
        default:
            break;
    }
}

// 实现缺失的方法
Model::Entities::PlatformType GameModel::getPlatformTypeRand() {
    float random = static_cast<float>(rand()) / RAND_MAX; // 0.0 - 1.0
    
    // 累积概率计算
    float cumulative = 0.0f;
    
    cumulative += Common::Config::GameConfig::PLATFORM_NORMAL_PROBABILITY;
    if (random < cumulative) {
        return Entities::PlatformType::NORMAL;
    }
    
    cumulative += Common::Config::GameConfig::PLATFORM_ROLLING_PROBABILITY;
    if (random < cumulative) {
        return Entities::PlatformType::ROLLING;
    }
    
    cumulative += Common::Config::GameConfig::PLATFORM_BOUNCY_PROBABILITY;
    if (random < cumulative) {
        return Entities::PlatformType::BOUNCY;
    }
    
    cumulative += Common::Config::GameConfig::PLATFORM_FRAGILE_PROBABILITY;
    if (random < cumulative) {
        return Entities::PlatformType::FRAGILE;
    }
    
    // 剩余情况为带刺平台
    return Entities::PlatformType::SPIKED;
}

// 初始化技能系统
void GameModel::initSkills() {
    // 清理旧技能
    for (auto* skill : skills) {
        delete skill;
    }
    skills.clear();
    
    // 创建两个技能：箭矢射击和冲刺
    skills.push_back(new Entities::Skill(Entities::SkillType::ARROW_SHOT, Common::Config::GameConfig::SKILL_ARROW_COOLDOWN));
    skills.push_back(new Entities::Skill(Entities::SkillType::SPRINT, Common::Config::GameConfig::SKILL_SPRINT_COOLDOWN));
}

bool GameModel::isPlatformPositionValid(sf::Vector2f position, sf::Vector2f size) {
    // 检查新平台与现有平台的间距，确保不重叠且有足够间距
    // 垂直间距 = 玩家高度 + 安全距离(20像素)
    float min_vertical_spacing = Common::Config::GameConfig::PLAYER_SIZE.y + 20.0f;
    // 水平间距 = 玩家宽度 + 安全距离(30像素)，确保玩家站在一个平台上无法同时接触另一个平台
    float min_horizontal_spacing = Common::Config::GameConfig::PLAYER_SIZE.x + 30.0f;
    
    // 定义新平台的边界
    float new_left = position.x;
    float new_right = position.x + size.x;
    float new_top = position.y;
    float new_bottom = position.y + size.y;
    
    // 检查与现有平台的碰撞
    for (const auto& [id, platform] : platforms) {
        if (!platform) continue;
        
        sf::Vector2f existing_pos = platform->getPosition();
        sf::Vector2f existing_size = platform->getSize();
        
        // 定义现有平台的边界
        float existing_left = existing_pos.x;
        float existing_right = existing_pos.x + existing_size.x;
        float existing_top = existing_pos.y;
        float existing_bottom = existing_pos.y + existing_size.y;
        
        // 检查垂直间距
        float vertical_distance = 0.0f;
        bool vertical_overlap = false;
        
        if (new_bottom < existing_top) {
            // 新平台在现有平台上方
            vertical_distance = existing_top - new_bottom;
        } else if (new_top > existing_bottom) {
            // 新平台在现有平台下方
            vertical_distance = new_top - existing_bottom;
        } else {
            // 垂直重叠
            vertical_overlap = true;
        }
        
        // 检查水平间距
        float horizontal_distance = 0.0f;
        bool horizontal_overlap = false;
        
        if (new_right < existing_left) {
            // 新平台在现有平台左侧
            horizontal_distance = existing_left - new_right;
        } else if (new_left > existing_right) {
            // 新平台在现有平台右侧
            horizontal_distance = new_left - existing_right;
        } else {
            // 水平重叠
            horizontal_overlap = true;
        }
        
        // 如果两个方向都重叠，则位置无效
        if (vertical_overlap && horizontal_overlap) {
            return false;
        }
        
        // 如果垂直重叠但水平有间距，检查水平间距是否足够
        if (vertical_overlap && horizontal_distance < min_horizontal_spacing) {
            return false;
        }
        
        // 如果水平重叠但垂直有间距，检查垂直间距是否足够
        if (horizontal_overlap && vertical_distance < min_vertical_spacing) {
            return false;
        }
    }
    
    return true;
}


