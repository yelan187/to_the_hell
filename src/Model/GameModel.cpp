#include <iostream>
#include "Model/GameModel.h"

using Model::GameModel;
using Model::Entities::PlatformType;

/*  GameModel.cpp - 游戏核心逻辑模型实现
    
    本文件实现了SFML游戏的核心业务逻辑，包括实体管理、事件系统、
    音频控制、动态背景切换等功能。遵循MVVM架构模式，负责数据处理和状态管理。
    
    重构后的架构特点：
    - Player类集成了技能系统和碰撞检测
    - GameModel负责全局状态管理和实体协调
    - 严格遵循MVVM分层，Model不依赖View层

==================== 构造函数和析构函数 ====================
- GameModel()           // 构造函数，初始化窗口大小和背景状态
- ~GameModel()          // 析构函数，清理所有资源（玩家、平台、敌人等）

==================== 游戏初始化方法 ====================
- initGame()            // 初始化游戏状态，重置配置，启动背景音乐
- initPlatforms()       // 创建初始平台（包括边界平台和游戏平台）
- initPlayer()          // 创建玩家并设置在平台上的初始位置
- initSkills()          // 初始化玩家技能系统（委托给Player类）
- initEvents()          // 初始化游戏事件序列（难度递增、背景切换）
- resetPlatformGenerateInterval()  // 重置平台生成间隔（含随机变化）

==================== 主要更新方法 ====================
- update()              // 主更新循环：处理事件、实体更新、Player碰撞检测、边界检查

==================== 实体生成方法 ====================
- generatePlatform()    // 生成新平台（验证位置有效性，随机类型）
- generateEnemy()       // 从窗口四边随机生成敌人
- generatePickup()      // 生成拾取物（星星豆子或平台豆子）
- createBullet()        // 创建子弹/箭矢（区分玩家和敌人子弹）

==================== 碰撞检测方法 ====================
- checkPlayerBulletsHitEnemies()    // 检测玩家箭矢击中敌人（加分、击杀计数）
注：Player相关的碰撞检测已移至Player类中

==================== 玩家控制方法 ====================
- playerJump()          // 玩家跳跃
- playerDown()          // 玩家下落/在平台上时使用穿透技能
- playerWalkLeft()      // 玩家左移
- playerWalkRight()     // 玩家右移
- playerStopLeft()      // 停止左移动作
- playerStopRight()     // 停止右移动作

==================== 技能系统方法 ====================
- playerUseSkill()      // 委托给Player类的技能系统

==================== 工具方法 ====================
- getPlatformTypeRand() // 根据配置概率随机获取平台类型
- isPlatformPositionValid() // 验证平台位置有效性（避免重叠、保持间距）

==================== 音频系统方法 ====================
- startBackgroundMusic() // 启动循环背景音乐（自动音量控制）
- stopBackgroundMusic()  // 停止背景音乐播放
- isBackgroundMusicPlaying() // 检查背景音乐播放状态

==================== 动态背景系统方法 ====================
- setBackground()       // 设置新背景图片路径（触发背景切换）
- getCurrentBackground() // 获取当前背景图片路径
- isBackgroundChanged() // 检查背景是否需要切换
- markBackgroundAsLoaded() // 标记背景已加载完成

==================== 通知和生命周期方法 ====================
- fire()                // 发送游戏帧更新通知到ViewModel
- gameOver()            // 游戏结束处理：停止音乐、发送结束通知

*/

// ==================== 构造函数和析构函数 ====================

GameModel::GameModel(sf::Vector2u window_size) : 
    Model(window_size), init(false) {
    // 在构造函数中不要调用initGame，等到第一次update时调用
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
    
    // 技能现在在Player类中管理，不需要在这里清理
    
    // 清理事件容器
    for (auto* event : events) {
        delete event;
    }
    events.clear();
}

// ==================== 游戏初始化方法 ====================

void GameModel::initGame() {
    // 重置所有配置为初始值
    Common::Config::GameConfig::resetToInitialValues();
    
    // 初始化游戏状态
    total_score = 0;
    game_time = 0;
    resetPlatformGenerateInterval();
    enemy_generate_interval = Common::Config::GameConfig::ENEMY_GENERATE_INTERVAL;
    pickup_generate_interval = Common::Config::GameConfig::PICKUP_GENERATE_INTERVAL;
    
    // 初始化背景系统
    current_background = "assets/images/background/misty_forest.png";
    background_changed = false;
    
    // 初始化游戏实体
    initPlatforms();
    initPlayer();
    initSkills();
    initEvents();
    
    startBackgroundMusic();
}

void GameModel::initPlatforms() {
    platforms[next_platform_id++] = new Entities::Platform(next_platform_id, PlatformType::WALL, 
        sf::Vector2f(-200, -500), sf::Vector2f(200,window_size.y + 500));
    platforms[next_platform_id++] = new Entities::Platform(next_platform_id, PlatformType::WALL, 
        sf::Vector2f(window_size.x, -500), sf::Vector2f(200,window_size.y + 500));
    
    const int initial_platforms = 3;
    for (int i = 0; i < initial_platforms; ++i) {
        sf::Vector2f position(
            static_cast<float>(rand() % static_cast<int>(window_size.x - Common::Config::GameConfig::PLATFORM_SIZE.x)),
            static_cast<float>(window_size.y / 2 + (window_size.y * 2 / 3) / initial_platforms * i)
        );
        
        PlatformType type = PlatformType::NORMAL;
        platforms[next_platform_id] = new Entities::Platform(next_platform_id, type, position, Common::Config::GameConfig::PLATFORM_SIZE);
    }
    next_platform_id += initial_platforms;
}

void GameModel::initPlayer() {
    sf::Vector2f platform_pos = platforms[2]->getPosition();
    sf::Vector2f platform_size = platforms[2]->getSize();
    
    sf::Vector2f player_position = sf::Vector2f(
        platform_pos.x + platform_size.x / 2 - Common::Config::GameConfig::PLAYER_SIZE.x / 2,
        platform_pos.y - Common::Config::GameConfig::PLAYER_SIZE.y
    );
    
    player = new Entities::Player(player_position, Common::Config::GameConfig::PLAYER_SIZE, this);
    player->setVelocity(platforms[2]->getVelocity());
}

void GameModel::initSkills() {
    // 玩家技能初始化现在在Player类中处理
    if (player) {
        player->initSkills();
    }
}

void GameModel::initEvents() {
    // 清理现有事件
    for (auto* event : events) {
        delete event;
    }
    events.clear();
    next_event_id = 0;
    
    // 创建游戏变化事件序列 - 使用简化的Event构造函数
    // 5秒: 滚动速度增加
    events.push_back(new Entities::Event(5.0f, "Scroll Speed Increased", []() {
        Common::Config::GameConfig::SCROLL_SPEED *= 1.2f;
    }));
    
    // 10秒: 敌人生成频率增加
    events.push_back(new Entities::Event(10.0f, "Enemy Spawn Rate Increased", [this]() {
        Common::Config::GameConfig::ENEMY_SPAWN_MIN_INTERVAL /= 1.3f;
        Common::Config::GameConfig::ENEMY_SPAWN_MAX_INTERVAL /= 1.3f;

        // 切换到更危险的背景
        setBackground("assets/images/background/misty_forest_2.png");
    }));
    
    // 20秒: 危险平台概率增加 + 背景切换到更暗的场景
    events.push_back(new Entities::Event(20.0f, "More Dangerous Platforms", [this]() {
        Common::Config::GameConfig::PLATFORM_SPIKED_PROBABILITY *= 1.5f;
        Common::Config::GameConfig::PLATFORM_FRAGILE_PROBABILITY *= 1.3f;
        // 调整普通平台概率保持平衡
        float non_normal = Common::Config::GameConfig::PLATFORM_ROLLING_PROBABILITY + 
                            Common::Config::GameConfig::PLATFORM_BOUNCY_PROBABILITY + 
                            Common::Config::GameConfig::PLATFORM_FRAGILE_PROBABILITY + 
                            Common::Config::GameConfig::PLATFORM_SPIKED_PROBABILITY;
        if (non_normal < 0.9f) {
            Common::Config::GameConfig::PLATFORM_NORMAL_PROBABILITY = 1.0f - non_normal;
        }
    }));
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

    // 更新事件系统
    for (auto* event : events) {
        event->update(game_time);
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
        bullet->update(delta_time);
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
    
    // 更新玩家技能
    if (player) {
        player->update(delta_time);
        
        // 使用Player类的碰撞检测系统
        int bullet_id = player->checkBulletCollisions();
        if (bullet_id != -1) {
            player->beDamagedByBullet(bullet_id);
        }
        
        // 我觉得碰到鬼不应该掉血(
        // int enemy_id = player->checkEnemyCollisions();
        // if (enemy_id != -1) {
        //     player->beDamagedByEnemy(enemy_id);
        // }

        int pickup_id = player->checkPickupCollisions();
        if (pickup_id != -1) {
            player->pickup(pickup_id);
        }
    }
    
    if (player && player->isDead()) {
        gameOver();
        return;
    }
    
    // 玩家子弹击中敌人的检测（这个需要在Model层处理，因为涉及分数和敌人删除）
    checkPlayerBulletsHitEnemies();

    // 边界检查（上下边界都算失败）
    sf::Vector2f player_pos = player->getPosition();
    sf::Vector2f player_size = player->getSize();
    if (player_pos.y + player_size.y <= 0 || player_pos.y + player_size.y >= window_size.y) {
        gameOver();
        return;
    }
    
    trigger.fire(Common::NotificationId::ChangeGameFrame);
}

// ==================== 实体生成方法 ====================

void GameModel::generatePlatform() {
    int max_attempts = Common::Config::GameConfig::PLATFORM_GENERATION_MAX_ATTEMPTS;
    
    for (int attempt = 0; attempt < max_attempts; ++attempt) {
        sf::Vector2f position(
            static_cast<float>(rand() % static_cast<int>(window_size.x - Common::Config::GameConfig::PLATFORM_SIZE.x)),
            window_size.y
        );
        
        if (isPlatformPositionValid(position, Common::Config::GameConfig::PLATFORM_SIZE)) {
            PlatformType type = getPlatformTypeRand();
            platforms[next_platform_id++] = new Entities::Platform(
                next_platform_id, type, position, Common::Config::GameConfig::PLATFORM_SIZE);
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

void GameModel::createBullet(sf::Vector2f position, sf::Vector2f velocity, int damage, bool is_player_bullet) {
    sf::Vector2f bullet_size = is_player_bullet ? 
        Common::Config::GameConfig::SKILL_ARROW_SIZE :
        Common::Config::GameConfig::ENEMY_BULLET_SIZE; 
    
    bullets[next_bullet_id] = new Entities::Bullet(next_bullet_id, position, velocity, bullet_size, damage, is_player_bullet);
    next_bullet_id++;
}

// ==================== 玩家控制方法 ====================

void GameModel::playerJump() {
    player->jump();
}

void GameModel::playerDown() {
    if (!player->isOnPlatform()) {
        player->fall();
    } else {
        // 在平台上时使用地面穿透技能
        auto& skills = player->getSkills();
        auto ground_penetration_it = skills.find(Common::SkillID::GROUND_PENETRATION);
        if (ground_penetration_it != skills.end()) {
            ground_penetration_it->second->use();
        }
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

void GameModel::playerUseSkill(Common::SkillID skill_id) {
    if (!player) return;
    
    auto& skills = player->getSkills();
    auto skill_it = skills.find(skill_id);
    if (skill_it == skills.end()) return;
    
    auto skill = skill_it->second;
    if (skill->canUse()) {
        skill->use(); // 这会调用execute()方法
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

// ==================== 音频系统方法 ====================

void GameModel::startBackgroundMusic() {
    // 停止当前音乐（如果正在播放）
    stopBackgroundMusic();
    
    // 加载背景音乐文件
    if (!background_music.openFromFile("assets/music/Things That Scheme in the Dark - Evan Call.mp3")) {
        std::cout << "Warning: Failed to load background music file" << std::endl;
        return;
    }
    
    // 设置循环播放
    background_music.setLoop(true);
    
    // 设置音量（范围：0-100）
    background_music.setVolume(50.0f);
    
    // 开始播放
    background_music.play();
    
    std::cout << "Background music started" << std::endl;
}

void GameModel::stopBackgroundMusic() {
    if (background_music.getStatus() == sf::Music::Playing) {
        background_music.stop();
        std::cout << "Background music stopped" << std::endl;
    }
}

bool GameModel::isBackgroundMusicPlaying() const {
    return background_music.getStatus() == sf::Music::Playing;
}

// ==================== 背景系统方法 ====================

void GameModel::setBackground(const std::string& background_file) {
    if (current_background != background_file) {
        current_background = background_file;
        background_changed = true;
        std::cout << "Background changed to: " << background_file << std::endl;
    }
}

void GameModel::removeBullet(int id) {
    auto it = bullets.find(id);
    if (it != bullets.end()) {
        delete it->second;
        bullets.erase(it);
    }
}

void GameModel::removePickup(int id) {
    auto it = pickups.find(id);
    if (it != pickups.end()) {
        delete it->second;
        pickups.erase(it);
    }
}

// ==================== 玩家子弹击中敌人检测 ====================

void GameModel::checkPlayerBulletsHitEnemies() {
    for (auto bullet_it = bullets.begin(); bullet_it != bullets.end(); ) {
        if (bullet_it->second->isPlayerBullet()) {
            bool hit_enemy = false;
            for (auto enemy_it = enemies.begin(); enemy_it != enemies.end(); ) {
                if (bullet_it->second->collidesWith(enemy_it->second->getPosition(), enemy_it->second->getSize())) {
                    // 增加分数
                    total_score += Common::Config::GameConfig::ENEMY_SCORE_VALUE;
                    enemy_it->second->hp -= bullet_it->second->getDamage();
                    if (enemy_it->second->hp <= 0) {
                        delete enemy_it->second;
                        enemy_it = enemies.erase(enemy_it);
                    }
                    hit_enemy = true;
                    // 增加玩家击杀计数
                    if (player) {
                        player->addKillCount();
                    }
                    break;
                } else {
                    ++enemy_it;
                }
            }
            
            if (hit_enemy) {
                // 删除击中的子弹
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