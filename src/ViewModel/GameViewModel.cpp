#include <iostream>
#include "ViewModel/GameViewModel.h"
#include "Common/InternalNotification.h"
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
    playerSkill_command(this)
{
    init_keystate();
    loadPlayerTextures();
}

std::string GameViewModel::getGameTimeText() {
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
    return ss.str();
}

std::string GameViewModel::getTotalScoreText() {
    return "Score: " + std::to_string(model->getTotalScore());
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
    key_state[sf::Keyboard::A] = false;
    if (key_state[sf::Keyboard::D]) {
        player_towards = towards::RIGHT;
    }
    model->playerStopLeft();
}

void GameViewModel::playerStopRight() {
    // 释放右键状态，如果左键仍按着则切换方向
    key_state[sf::Keyboard::D] = false;
    if (key_state[sf::Keyboard::A]) {
        player_towards = towards::LEFT;
    }
    model->playerStopRight();
}

void GameViewModel::playerStopJump() {
    // 释放跳跃键状态，允许再次跳跃
    key_state[sf::Keyboard::W] = false;
}

void GameViewModel::playerStopDown() {
    // 释放下落键状态，允许再次快速下落
    key_state[sf::Keyboard::S] = false;
}

void GameViewModel::playerUseSkill(int skill_id, sf::Vector2f direction) {
    // 将技能使用请求委托给Model层处理
    model->playerUseSkill(skill_id, direction);
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
    }
    return &player_textures[PlayerState::IDLE_R]; // Default to idle right texture
}

Common::FrameInfo::PlatformInfo GameViewModel::getPlatformInfo(Common::_FrameInfo::PlatformInfo info) {
    Common::FrameInfo::PlatformInfo platform_info;
    platform_info.position = info.position;
    platform_info.size = Common::Config::GameConfig::PLATFORM_SIZE;
    platform_info.platform_type = static_cast<int>(info.type);
    platform_info.rolling_direction = info.rolling_direction; // 使用Model层传递的真实滚动方向
    
    // 根据平台类型设置颜色和属性
    switch (info.type) {
        case Model::Entities::PlatformType::NORMAL:
            platform_info.color = sf::Color::Green; // 普通平台 - 绿色
            break;
        case Model::Entities::PlatformType::SPIKED:
            platform_info.color = sf::Color::Red; // 带刺平台 - 红色
            break;
        case Model::Entities::PlatformType::ROLLING:
            platform_info.color = sf::Color::Green; // 滚动平台 - 绿色底色，View层添加方向箭头
            break;
        case Model::Entities::PlatformType::FRAGILE:
            platform_info.color = sf::Color::Transparent; // 脆弱平台 - 透明，View层添加虚线边框
            break;
        case Model::Entities::PlatformType::BOUNCY:
            platform_info.color = sf::Color::Green; // 弹跳平台 - 绿色椭圆形状
            break;
    }
    return platform_info;
}

void GameViewModel::notification_callback(Common::NotificationParam* param, void* view_model) {
    if (!view_model) return;
    GameViewModel* game_view_model = static_cast<GameViewModel*>(view_model);
    switch (param->id) {
        case Common::NotificationId::_ChangeGameFrame:
            game_view_model->forwarding(dynamic_cast<Common::_ChangeGameFrameParam*>(param)->value);
            break;
        case Common::NotificationId::GameOver: 
            game_view_model->trigger.fire(param);
            break;
    }
}

void GameViewModel::forwarding(const Common::_FrameInfo& frame_info) {
    Common::ChangeGameFrameParam* change_frame_param = new Common::ChangeGameFrameParam();
    change_frame_param->id = Common::NotificationId::ChangeGameFrame;
    change_frame_param->value.total_score_text = getTotalScoreText();
    change_frame_param->value.game_time_text = getGameTimeText();
    change_frame_param->value.debug_info_text = getDebugInfoText();
    change_frame_param->value.player_info.position = frame_info.player_info.position;
    change_frame_param->value.player_info.size = frame_info.player_info.size;
    change_frame_param->value.player_info.texture = getPlayerTexture(frame_info.player_info.state);
    
    // 平台信息转换
    std::map<int, Common::FrameInfo::PlatformInfo> platforms_info;
    for (const auto& [id, info] : frame_info.platforms_info) {
        platforms_info[id] = getPlatformInfo(info);
    }
    change_frame_param->value.platforms_info = platforms_info;
    change_frame_param->value.platforms_id = frame_info.platforms_id;
    
    // 敌人信息转换
    std::map<int, Common::FrameInfo::EnemyInfo> enemies_info;
    for (const auto& [id, info] : frame_info.enemies_info) {
        enemies_info[id].position = info.position;
        enemies_info[id].size = info.size;
        enemies_info[id].color = sf::Color::Red; // 敌人显示为红色
        enemies_info[id].facing_direction = info.facing_direction; // 传递面向方向
    }
    change_frame_param->value.enemies_info = enemies_info;
    change_frame_param->value.enemies_id = frame_info.enemies_id;
    
    // 子弹信息转换
    std::map<int, Common::FrameInfo::BulletInfo> bullets_info;
    for (const auto& [id, info] : frame_info.bullets_info) {
        bullets_info[id].position = info.position;
        bullets_info[id].size = info.size;
        bullets_info[id].is_player_bullet = info.is_player_bullet;
        bullets_info[id].velocity = info.velocity;
        // 玩家箭矢为黄色，敌人子弹为白色
        bullets_info[id].color = info.is_player_bullet ? sf::Color::Yellow : sf::Color::White;
    }
    change_frame_param->value.bullets_info = bullets_info;
    change_frame_param->value.bullets_id = frame_info.bullets_id;
    
    // 豆子信息转换
    std::map<int, Common::FrameInfo::PickupInfo> pickups_info;
    for (const auto& [id, info] : frame_info.pickups_info) {
        pickups_info[id].position = info.position;
        pickups_info[id].size = info.size;
        pickups_info[id].color = sf::Color::Yellow; // 豆子显示为黄色
        pickups_info[id].pickup_type = static_cast<int>(info.type); // 转换枚举为整数
    }
    change_frame_param->value.pickups_info = pickups_info;
    change_frame_param->value.pickups_id = frame_info.pickups_id;
    
    // 技能信息转换
    std::vector<Common::FrameInfo::SkillInfo> skills_info;
    auto skills = model->getSkills();
    for (auto* skill : skills) {
        Common::FrameInfo::SkillInfo skill_info;
        skill_info.skill_type = static_cast<int>(skill->getType());
        skill_info.cooldown_progress = skill->getCooldownProgress();
        skill_info.is_available = skill->isAvailable();
        skills_info.push_back(skill_info);
    }
    change_frame_param->value.skills_info = skills_info;
    
    trigger.fire(change_frame_param);
    delete change_frame_param;
}

void GameViewModel::UpdateCommand::execute(Common::CommandParam& delta_time) {
    auto& update_param = dynamic_cast<Common::UpdateParam&>(delta_time);
    view_model->model->update(update_param.value);
}