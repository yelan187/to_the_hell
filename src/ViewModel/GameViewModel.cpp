#include <iostream>
#include "ViewModel/GameViewModel.h"
#include "Common/Config/Config.h"

using ViewModel::GameViewModel;

GameViewModel::GameViewModel(sf::Vector2u window_size) : 
    ViewModel(window_size),
    model(nullptr),
    playerLeft_command(this),
    playerRight_command(this),
    playerJump_command(this),
    playerDown_command(this),
    playerStopLeft_command(this),
    playerStopRight_command(this),
    playerStopJump_command(this),
    playerStopDown_command(this),
    update_command(this),
    playerSkill_command(this),
    choose_command(this)
{
    init_keystate();
    loadPlayerTextures();
    loadPlatformTextures();
}

void GameViewModel::updateGameTimeText() {
    std::chrono::seconds duration = model->getDuration();
    int seconds = duration.count() % 60;
    int minutes = (duration.count() / 60) % 60;
    int hours = duration.count() / 3600;
    std::stringstream ss;
    ss << "Time: ";
    if (hours > 0) {
        ss << std::setw(2) << std::setfill('0') << hours << "h";
    }
    if (hours > 0 || minutes > 0) {
        ss << std::setw(2) << std::setfill('0') << minutes << "m";
    } else {
        ss << "00m";
    }
    ss << std::setw(2) << std::setfill('0') << seconds << "s";
    game_time = ss.str();
}

void GameViewModel::updateTotalScoreText() {
    total_score = "Score: " + std::to_string(model->getTotalScore());
}

void GameViewModel::playerJump() {
    // 防止按键重复触发，实现按键状态管理
    if (!key_state[sf::Keyboard::W]) {
        model->playerJump();
        key_state[sf::Keyboard::W] = true;
    }
}

void GameViewModel::playerDown() {
    // 防止连续下落命令，实现单次按键效果
    if (!key_state[sf::Keyboard::S]) {
        model->playerDown();
        key_state[sf::Keyboard::S] = true;
    }
}

void GameViewModel::playerWalkLeft() {
    // 管理左右移动状态，确保方向正确性
    if (!key_state[sf::Keyboard::A]) {
        key_state[sf::Keyboard::A] = true;
        // 只有在不按右键时才设置为向左，避免冲突
        if (!key_state[sf::Keyboard::D]) {
            player_towards = towards::LEFT;
        }
        model->playerWalkLeft();
    }
}

void GameViewModel::playerWalkRight() {
    // 管理左右移动状态，确保方向正确性
    if (!key_state[sf::Keyboard::D]) {
        key_state[sf::Keyboard::D] = true;
        // 只有在不按左键时才设置为向右，避免冲突
        if (!key_state[sf::Keyboard::A]) {
            player_towards = towards::RIGHT;
        }
        model->playerWalkRight();
    }
}

void GameViewModel::playerStopLeft() {
    // 释放左键状态，如果右键仍按着则切换方向
    if (key_state[sf::Keyboard::A]) {
        key_state[sf::Keyboard::A] = false;
        if (key_state[sf::Keyboard::D]) {
            player_towards = towards::RIGHT;
        }
        model->playerStopLeft();
    }
}

void GameViewModel::playerStopRight() {
    // 释放右键状态，如果左键仍按着则切换方向
    if (key_state[sf::Keyboard::D]){
        key_state[sf::Keyboard::D] = false; 
        if (key_state[sf::Keyboard::A]) {
            player_towards = towards::LEFT;
        }
        model->playerStopRight();
    }
}

void GameViewModel::playerStopJump() {
    // 释放跳跃键状态，允许再次跳跃
    if (key_state[sf::Keyboard::W]) {
        // 只有在跳跃状态下才需要处理
        key_state[sf::Keyboard::W] = false;
    }
}

void GameViewModel::playerStopDown() {
    if (key_state[sf::Keyboard::S]) {
        key_state[sf::Keyboard::S] = false;
    }
}

void GameViewModel::playerUseSkill(Common::SkillID skill_id) {
    // 将技能使用请求委托给Model层处理
    model->playerUseSkill(skill_id);
}

void GameViewModel::loadPlayerTextures() {
    // std::cout << "Loading player textures..." << std::endl;
    // Load textures for different player states
    try {
        sf::Texture idle_l_texture;
        idle_l_texture.loadFromFile("assets/images/player/player_idle_l.png");
        player_textures[PlayerState::IDLE_L] = idle_l_texture;
    } catch (const std::exception& e) {
        std::cout << "Error loading player textures: " << e.what() << std::endl;
    }
    try {
        sf::Texture idle_r_texture;
        idle_r_texture.loadFromFile("assets/images/player/player_idle_r.png");
        player_textures[PlayerState::IDLE_R] = idle_r_texture;
    } catch (const std::exception& e) {
        std::cout << "Error loading player textures: " << e.what() << std::endl;
    }
    try {
        sf::Texture walk_l_texture;
        walk_l_texture.loadFromFile("assets/images/player/player_walk_l.png");
        player_textures[PlayerState::WALKING_L] = walk_l_texture;
    } catch (const std::exception& e) {
        std::cout << "Error loading player textures: " << e.what() << std::endl;
    }
    try {
        sf::Texture walk_r_texture;
        walk_r_texture.loadFromFile("assets/images/player/player_walk_r.png");
        player_textures[PlayerState::WALKING_R] = walk_r_texture;
    } catch (const std::exception& e) {
        std::cout << "Error loading player textures: " << e.what() << std::endl;
    }
    try {
        sf::Texture jump_l_texture;
        jump_l_texture.loadFromFile("assets/images/player/player_jump_l.png");
        player_textures[PlayerState::JUMPING_L] = jump_l_texture;
    } catch (const std::exception& e) {
        std::cout << "Error loading player textures: " << e.what() << std::endl;
    }
    try {
        sf::Texture jump_r_texture;
        jump_r_texture.loadFromFile("assets/images/player/player_jump_r.png");
        player_textures[PlayerState::JUMPING_R] = jump_r_texture;
    } catch (const std::exception& e) {
        std::cout << "Error loading player textures: " << e.what() << std::endl;
    }
    try {
        sf::Texture sprint_l_texture;
        sprint_l_texture.loadFromFile("assets/images/player/player_sprint_l.png");
        player_textures[PlayerState::SPRINTING_L] = sprint_l_texture;
    } catch (const std::exception& e) {
        std::cout << "Error loading player textures: " << e.what() << std::endl;
    }
    try {
        sf::Texture sprint_r_texture;
        sprint_r_texture.loadFromFile("assets/images/player/player_sprint_r.png");
        player_textures[PlayerState::SPRINTING_R] = sprint_r_texture;
    } catch (const std::exception& e) {
        std::cout << "Error loading player textures: " << e.what() << std::endl;
    }
    // std::cout << "Player textures loaded successfully." << std::endl;
}

sf::Texture* GameViewModel::getPlayerTexture(Model::Entities::PlayerState state) {
    switch (state) {
        case Model::Entities::PlayerState::IDLE:
            if (player_towards == towards::LEFT) {
                return &player_textures[PlayerState::IDLE_L];
            } else {
                return &player_textures[PlayerState::IDLE_R];
            }
        case Model::Entities::PlayerState::WALKING:
            if (player_towards == towards::LEFT) {
                return &player_textures[PlayerState::WALKING_L];
            } else {
                return &player_textures[PlayerState::WALKING_R];
            }
        case Model::Entities::PlayerState::JUMPING_IDLE:
        case Model::Entities::PlayerState::JUMPING_WALKING:
            if (player_towards == towards::LEFT) {
                return &player_textures[PlayerState::JUMPING_L];
            } else {
                return &player_textures[PlayerState::JUMPING_R];
            }
        case Model::Entities::PlayerState::SPRINTING:
            if (player_towards == towards::LEFT) {
                return &player_textures[PlayerState::SPRINTING_L];
            } else {
                return &player_textures[PlayerState::SPRINTING_R];
            }
    }
    return &player_textures[PlayerState::IDLE_R]; // Default to idle right texture
}

void GameViewModel::loadPlatformTextures() {
    try {
        sf::Texture normal_texture;
        normal_texture.loadFromFile("assets/images/platform/platform_normal.png");
        platform_textures[Common::PlatformType::NORMAL] = normal_texture;
    } catch (const std::exception& e) {
        std::cout << "Error loading normal platform texture: " << e.what() << std::endl;
    }
    try {
        sf::Texture spiked_texture;
        spiked_texture.loadFromFile("assets/images/platform/platform_spiked.png");
        platform_textures[Common::PlatformType::SPIKED] = spiked_texture;
    } catch (const std::exception& e) {
        std::cout << "Error loading spiked platform texture: " << e.what() << std::endl;
    }
    try {
        sf::Texture rolling_texture;
        rolling_texture.loadFromFile("assets/images/platform/platform_rolling_r.png");
        platform_textures[Common::PlatformType::ROLLING] = rolling_texture;
    } catch (const std::exception& e) {
        std::cout << "Error loading rolling platform texture: " << e.what() << std::endl;
    }
    try {
        sf::Texture rolling_l_texture;
        rolling_l_texture.loadFromFile("assets/images/platform/platform_rolling_l.png");
        platform_textures[Common::PlatformType::ROLLING_L] = rolling_l_texture;
    } catch (const std::exception& e) {
        std::cout << "Error loading left rolling platform texture: " << e.what() << std::endl;
    }
    try {
        sf::Texture fragile_texture;
        fragile_texture.loadFromFile("assets/images/platform/platform_fragile.png");
        platform_textures[Common::PlatformType::FRAGILE] = fragile_texture;
    } catch (const std::exception& e) {
        std::cout << "Error loading fragile platform texture: " << e.what() << std::endl;
    }
    try {
        sf::Texture bouncy_texture;
        bouncy_texture.loadFromFile("assets/images/platform/platform_bouncy.png");
        platform_textures[Common::PlatformType::BOUNCY] = bouncy_texture;
    } catch (const std::exception& e) {
        std::cout << "Error loading bouncy platform texture: " << e.what() << std::endl;
    }
}

sf::Texture* GameViewModel::getPlatformTexture(Common::PlatformType type, bool rolling_r) {
    if (type == Common::PlatformType::ROLLING) {
        // 滚动平台需要根据方向返回不同的纹理
        if (rolling_r) {
            return &platform_textures[Common::PlatformType::ROLLING];
        } else {
            return &platform_textures[Common::PlatformType::ROLLING_L];
        }
    }
    auto it = platform_textures.find(type);
    if (it != platform_textures.end()) {
        return &it->second;
    }
}

void GameViewModel::updatePlatformsInfo() {
    std::map<int, Model::Entities::Platform*> platforms = model->getPlatforms();
    
    std::vector<int> platforms_id;
    std::map<int, Common::FrameInfo::PlatformInfo> platform_info;
    for (const auto& pair : platforms) {
        platforms_id.push_back(pair.first);
        platform_info[pair.first].position = pair.second->getPosition();
        platform_info[pair.first].size = Common::Config::GameConfig::PLATFORM_SIZE;
        platform_info[pair.first].platform_type = static_cast<int>(pair.second->type);
        switch (pair.second->type) {
            case Model::Entities::PlatformType::NORMAL:
                platform_info[pair.first].color = sf::Color::Green; // 普通平台 - 绿色
                break;
            case Model::Entities::PlatformType::SPIKED:
                platform_info[pair.first].color = sf::Color::Red; // 带刺平台 - 红色
                break;
            case Model::Entities::PlatformType::ROLLING:
                platform_info[pair.first].color = sf::Color::Green; // 滚动平台 - 绿色底色，View层添加方向箭头
                break;
            case Model::Entities::PlatformType::FRAGILE:
                platform_info[pair.first].color = sf::Color::Transparent; // 脆弱平台 - 透明，View层添加虚线边框
                break;
            case Model::Entities::PlatformType::BOUNCY:
                platform_info[pair.first].color = sf::Color::Green; // 弹跳平台 - 绿色椭圆形状
                break;
        }
        bool rolling_direction = pair.second->getRollingDirection();
        platform_info[pair.first].rolling_direction = rolling_direction;
        platform_info[pair.first].texture = getPlatformTexture(pair.second->type,rolling_direction);
    }
    frame_info.platforms_id = platforms_id;
    frame_info.platforms_info = platform_info;
}

void GameViewModel::notification_callback(Common::NotificationId id, void* view_model) {
    if (!view_model) return;
    GameViewModel* game_view_model = static_cast<GameViewModel*>(view_model);
    switch (id) {
        case Common::NotificationId::ChangeGameFrame:
            game_view_model->forwarding();
            break;
        case Common::NotificationId::GameOver: 
            game_view_model->trigger.fire(Common::NotificationId::GameOver);
            break;
        case Common::NotificationId::Choose:
            {
                game_view_model->choices_info.clear();
                for (int i = 0 ; i < Common::Config::GameConfig::MAX_CHOICES; i++){
                    Common::choiceInfo info;
                    info.name = game_view_model->model->getEffectName(i);
                    info.description = game_view_model->model->getEffectDescription(i);
                    game_view_model->choices_info.push_back(info);
                }
                game_view_model->trigger.fire(Common::NotificationId::Choose);
            }
            break;
        case Common::NotificationId::EndChoose:
            game_view_model->trigger.fire(Common::NotificationId::EndChoose);
            break;
    }
}

void GameViewModel::forwarding() {
    updateGameTimeText();
    updateTotalScoreText();
    updatePlatformInfoText();
    
    // 检查player是否已初始化，避免空指针访问
    if (model->getPlayer() != nullptr) {
        frame_info.player_info.position = model->getPlayer()->getPosition();
        frame_info.player_info.size = model->getPlayer()->getSize();
        frame_info.player_info.texture = getPlayerTexture(model->getPlayer()->getState());
        frame_info.player_info.hp = model->getPlayer()->getHP();
        frame_info.player_info.max_hp = model->getPlayer()->getMaxHP();
    } else {
        // 如果player未初始化，设置默认值
        frame_info.player_info.position = sf::Vector2f(0, 0);
        frame_info.player_info.size = Common::Config::GameConfig::PLAYER_SIZE;
        frame_info.player_info.texture = nullptr;
        frame_info.player_info.hp = 0;
        frame_info.player_info.max_hp = Common::Config::GameConfig::PLAYER_MAX_HP;
    }  
    // 平台信息转换
    updatePlatformsInfo();
    
    // 敌人信息转换
    std::vector<int> enemies_id;
    std::map<int, Common::FrameInfo::EnemyInfo> enemies_info;
    for (const auto& pair : model->getEnemies()) {
        enemies_id.push_back(pair.first);
        enemies_info[pair.first].position = pair.second->getPosition();
        enemies_info[pair.first].size = pair.second->getSize();
        enemies_info[pair.first].color = sf::Color::Red;
        enemies_info[pair.first].facing_direction = pair.second->getFacingDirection();
    }
    frame_info.enemies_info = enemies_info;
    frame_info.enemies_id = enemies_id;

    // 子弹信息转换
    std::vector<int> bullets_id;
    std::map<int, Common::FrameInfo::BulletInfo> bullets_info;
    for (const auto& pair : model->getBullets()) {
        bullets_id.push_back(pair.first);
        bullets_info[pair.first].position = pair.second->getPosition();
        bullets_info[pair.first].size = pair.second->getSize();
        bullets_info[pair.first].is_player_bullet = pair.second->isPlayerBullet();
        bullets_info[pair.first].velocity = pair.second->getVelocity();
        // 玩家箭矢为黄色，敌人子弹为白色
        bullets_info[pair.first].color = bullets_info[pair.first].is_player_bullet ? sf::Color::Yellow : sf::Color::White;
    }
    frame_info.bullets_info = bullets_info;
    frame_info.bullets_id = bullets_id;

    // 豆子信息转换
    std::vector<int> pickups_id;
    std::map<int, Common::FrameInfo::PickupInfo> pickups_info;
    for (const auto& pair : model->getPickups()) {
        pickups_id.push_back(pair.first);
        pickups_info[pair.first].position = pair.second->getPosition();
        pickups_info[pair.first].size = pair.second->getSize();
        pickups_info[pair.first].color = sf::Color::Yellow; // 豆子显示为黄色
        pickups_info[pair.first].pickup_type = static_cast<int>(pair.second->getType()); // 转换枚举为整数
    }
    frame_info.pickups_info = pickups_info;
    frame_info.pickups_id = pickups_id;

    // 技能信息转换
    std::vector<Common::FrameInfo::SkillInfo> skills_info;
    if (model->getPlayer()) {
        auto& player_skills = model->getPlayer()->getSkills();
        for (const auto& skill_pair : player_skills) {
            Common::FrameInfo::SkillInfo skill_info;
            skill_info.skill_id = skill_pair.first;
            skill_info.cooldown_progress = skill_pair.second->getCooldownProgress();
            skill_info.is_available = skill_pair.second->isAvailable();
            skills_info.push_back(skill_info);
        }
    }
    frame_info.skills_info = skills_info;

    // 背景信息转换
    frame_info.background_info.background_file = model->getCurrentBackground();
    frame_info.background_info.changed = model->isBackgroundChanged();
    if (model->isBackgroundChanged()) {
        model->markBackgroundAsLoaded(); // 标记为已加载
    }

    trigger.fire(Common::NotificationId::ChangeGameFrame);
}

void GameViewModel::UpdateCommand::execute(Common::CommandParam& delta_time) {
    auto& update_param = dynamic_cast<Common::UpdateParam&>(delta_time);
    view_model->model->update(update_param.value);
}